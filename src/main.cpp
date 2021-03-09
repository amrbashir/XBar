#include "config.h"
#include "constants.h"
#include "context-menu-manager.h"
#include "style-the-taskbar.h"
#include "tray-icon-manager.h"
#include "utils/logger.h"
#include "utils/window.h"
#include <Windows.h>
#include <array>
#include <thread>
#include <toml++/toml.h>
#include <winreg.h>

using namespace std;
using namespace utils;

// initialize needed variables
const LPCSTR WINDOW_CLASS = "XBar window class";
const HWND   taskbar      = FindWindow("Shell_TrayWnd", nullptr);
toml::table  config       = Config_Manager::parse_config_file();

LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_USER_SHELLICON:
            switch (lParam) {
                case WM_CONTEXTMENU:
                    logger::deug("Context menu requested.");
                    // open context menu on right clicking the tray icon
                    Context_Menu_Manager::create_context_menu(hwnd);
                    break;
            }
            break;

        case WM_COMMAND:
            logger::deug("Context menu item clicked.");
            // a context menu item is clicked so we handle it in the Context_Menu_Manager
            Context_Menu_Manager::context_menu_item_click(hwnd, wParam);
            break;

        case WM_DESTROY:
            logger::info("Quitting XBar...");
            // reset taskbar to normal state before closing the app
            window::set_window_style(taskbar, ACCENT_STATE::ACCENT_NORMAL);
            logger::info("Taskbar style is reset to normal.");
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {

    // register a window class
    WNDCLASS wc      = {};
    wc.hInstance     = hInstance;
    wc.lpszClassName = WINDOW_CLASS;
    wc.lpfnWndProc   = windowProc;
    if (!RegisterClass(&wc)) {
        logger::error("Failed to register window Class.");
        return -1;
    };

    // create a hidden window to recieve events
    const HWND hwnd = CreateWindowEx(0, WINDOW_CLASS, "XBar", WS_TILEDWINDOW, 200, 200, 500, 500,
                                     nullptr, nullptr, hInstance, nullptr);
    if (hwnd == nullptr) {
        logger::error("Failed to create widnow and obtain a handle to it.");
        return -1;
    }

#ifndef _DEBUG
    // check `RunAtStartup` option
    // if true then add a run entry in registry if it doesn't exist
    // else remove the run entry from the registry if it exists
    const bool run_at_startup = config["General"]["RunAtStartup"].value<bool>().value();
    HKEY       hkey           = NULL;
    auto       error          = RegCreateKey(HKEY_CURRENT_USER,
                              "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey);
    if (error != ERROR_SUCCESS)
        logger::error("Failed to open the registry key");

    if (run_at_startup) {
        const string app_path = window::get_window_exe_path(hwnd);

        auto error = RegSetValueEx(hkey, "XBar", 0, REG_SZ, (BYTE *)app_path.c_str(),
                                   (DWORD)(app_path.size() + 1) * sizeof(wchar_t));
        if (error != ERROR_SUCCESS)
            logger::error("Failed to set the startup registry key value.");
    } else {
        auto error = RegDeleteValue(hkey, "XBar");
        if (error != ERROR_SUCCESS)
            logger::error("Failed to delete the startup registry key value.");
    }
    RegCloseKey(hkey);
#endif

    // add tray icon to the system tray
    Tray_Icon_Manager::register_tray_icon(hwnd);

    // set the procedure for active window changed event
    HWINEVENTHOOK hEvent = SetWinEventHook(
        EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, NULL,
        [](HWINEVENTHOOK hWinEventHook, DWORD dwEvent, HWND hwnd, LONG idObject, LONG idChild,
           DWORD dwEventThread, DWORD dwmsEventTime) { style_the_taskbar(taskbar, config); },
        0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);

    // start a new thread to style the taskbar every 14 millisecons
    thread taskbar_styling_thread([]() {
        while (true) {
            style_the_taskbar(taskbar, config);
            Sleep(14);
        }
    });

    // run the message loop
    MSG msg = { 0 };
    while (GetMessage(&msg, nullptr, NULL, NULL)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
