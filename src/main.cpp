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

const LPCSTR WINDOW_CLASS = "XBar window class";
const HWND   taskbar      = FindWindow("Shell_TrayWnd", nullptr);
toml::table  config       = Config_Manager::parse_config_file();

LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_USER_SHELLICON:
            switch (lParam) {
                case WM_CONTEXTMENU:
                    Context_Menu_Manager::create_context_menu(hwnd);
                    break;
            }
            break;

        case WM_COMMAND:
            Context_Menu_Manager::execute_context_menu_item(hwnd, wParam);
            break;

        case WM_DESTROY:
            // reset taskbar to normal state before closing the app
            window::set_window_style(taskbar, ACCENT_STATE::ACCENT_NORMAL);
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    // Register the window class
    WNDCLASS wc      = {};
    wc.hInstance     = hInstance;
    wc.lpszClassName = WINDOW_CLASS;
    wc.lpfnWndProc   = windowProc;

    if (!RegisterClass(&wc)) {
        logger::error("Failed to register window Class");
        return -1;
    };

    // Create a hidden window to recieve events
    const HWND hwnd = CreateWindowEx(0, WINDOW_CLASS, "XBar", WS_TILEDWINDOW, 200, 200, 500, 500,
                                     nullptr, nullptr, hInstance, nullptr);
    if (hwnd == nullptr) {
        logger::error("Failed to create widnow and obtain a handle to it");
        return -1;
    }

#ifndef _DEBUG
    // Check RunAtStartup
    const bool run_at_startup = config["General"]["RunAtStartup"].value<bool>().value();
    if (run_at_startup) {
        // add a run entry in registry
        const string app_path = window::get_window_exe_path(hwnd);
        HKEY         hkey     = NULL;
        RegCreateKey(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey);
        RegSetValueEx(hkey, "XBar", 0, REG_SZ, (BYTE *)app_path.c_str(),
                      (DWORD)(app_path.size() + 1) * sizeof(wchar_t));
        RegCloseKey(hkey);

    } else {
        // remove the run entry from registry
        HKEY hkey = NULL;
        RegCreateKey(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey);
        RegDeleteValue(hkey, "XBar");
        RegCloseKey(hkey);
    }
#endif
    // Register the tray icon
    Tray_Icon_Manager::register_tray_icon(hwnd);

    // Set the procedure for active window changed event
    HWINEVENTHOOK hEvent = SetWinEventHook(
        EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, NULL,
        [](HWINEVENTHOOK hWinEventHook, DWORD dwEvent, HWND hwnd, LONG idObject, LONG idChild,
           DWORD dwEventThread, DWORD dwmsEventTime) { style_the_taskbar(taskbar, config); },
        0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);

    // Start the XBar thread
    thread taskbar_styling_thread([]() {
        while (true) {
            style_the_taskbar(taskbar, config);
            Sleep(14);
        }
    });

    MSG msg = { 0 };
    while (GetMessage(&msg, nullptr, NULL, NULL)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
