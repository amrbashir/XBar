#include "config-manager.h"
#include "constants.h"
#include "style-the-taskbar.h"
#include "tray-menu.h"
#include "types.h"
#include "utils/logger.h"
#include "utils/window.h"
#include <Windows.h>
#include <array>
#include <nlohmann/json.hpp>
#include <thread>
#include <winreg.h>

using namespace std;
using namespace utils;

types::RunData run_data;

void toggle_startup(bool run_at_startup, string app_path) {
    HKEY hkey  = nullptr;
    auto error = RegCreateKey(HKEY_CURRENT_USER,
                              "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey);
    if (error != ERROR_SUCCESS)
        logger::error("Failed to open the registry key");

    if (run_at_startup) {
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
}

void toggle_tray_icon(HWND hwnd, bool register_icon) {
    const HICON logo_hicon = (HICON)LoadImage(nullptr, "assets\\XBar_icon.ico", IMAGE_ICON, 48, 48,
                                              LR_LOADFROMFILE | LR_DEFAULTSIZE);

    NOTIFYICONDATAA icon_data  = {};
    icon_data.cbSize           = sizeof(icon_data);
    icon_data.hWnd             = hwnd;
    icon_data.uFlags           = NIF_ICON | NIF_MESSAGE;
    icon_data.hIcon            = logo_hicon;
    icon_data.uCallbackMessage = WM_USER_TRAYICON;
    icon_data.uVersion         = NOTIFYICON_VERSION_4;

    if (register_icon) {
        if (Shell_NotifyIcon(NIM_ADD, &icon_data) == FALSE) {
            logger::error("Failed to add tray icon.");
        };

        if (Shell_NotifyIcon(NIM_SETVERSION, &icon_data) == FALSE) {
            logger::error("Failed to set tray NIM_SETVERSION.");
        }
    } else {
        if (Shell_NotifyIcon(NIM_DELETE, &icon_data) == FALSE) {
            logger::error("Failed to unregister tray icon.");
        };
    }
}

LRESULT CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_USER_TRAYICON:
            switch (lParam) {
                case WM_CONTEXTMENU:
                    TrayMenu::show(hwnd);
                    break;
            }
            break;
        case WM_COMMAND: {
            TrayMenu::on_menu_item_click(hwnd, wParam, run_data);
            break;
        }
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    // check if another instance is runnning
    HANDLE instance_mutex = OpenMutex(MUTEX_ALL_ACCESS, 0, "XBar");
    if (instance_mutex) {
        MessageBox(nullptr, "Another instance is already running.", "XBar", MB_OK);
        return 0;
    } else {
        instance_mutex = CreateMutex(0, 0, "XBar");
    }

    // initialize our runtime data
    run_data.taskbar = FindWindow("Shell_TrayWnd", nullptr);
    run_data.config  = ConfigManager::parse_config_file();
    run_data.running = true;

    const LPCSTR WINDOW_CLASS = "XBar Window Class";

    WNDCLASS w_class      = {};
    w_class.hInstance     = hInstance;
    w_class.lpszClassName = WINDOW_CLASS;
    w_class.lpfnWndProc   = window_proc;
    if (!RegisterClass(&w_class)) {
        logger::error("Failed to register window Class.");
        return -1;
    };

    // create a hidden window to recieve tray events
    const HWND window_hwnd = CreateWindowEx(0, WINDOW_CLASS, "XBar", WS_TILEDWINDOW, 200, 200, 500,
                                            500, nullptr, nullptr, hInstance, nullptr);
    if (window_hwnd == nullptr) {
        logger::error("Failed to create widnow and obtain a handle to it.");
        return -1;
    }

#ifndef _DEBUG
    // add XBar to the startup programs or remove it based on the config
    const bool run_at_startup = config["general"]["runAtStartup"].get<bool>();
    toggle_startup(run_at_startup, window::get_exe_path(window_hwnd));
#endif

    // register the tray icon
    const bool show_tray_icon = run_data.config["general"]["showTrayIcon"].get<bool>();
    toggle_tray_icon(window_hwnd, show_tray_icon);

    // set the procedure for active window changed event
    SetWinEventHook(
        EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, nullptr,
        [](HWINEVENTHOOK hWinEventHook, DWORD dwEvent, HWND hwnd, LONG idObject, LONG idChild,
           DWORD dwEventThread,
           DWORD dwmsEventTime) { style_the_taskbar(run_data.taskbar, run_data.config); },
        0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);

    // start a new thread to style the taskbar every 14 millisecons
    thread taskbar_styling_thread([]() {
        while (run_data.running) {
            style_the_taskbar(run_data.taskbar, run_data.config);
            std::this_thread::sleep_for(std::chrono::milliseconds(14));
        }
    });

    // run the message loop
    MSG msg = { 0 };
    while (GetMessage(&msg, nullptr, NULL, NULL)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // exit logic when the message loop is over
    run_data.running = false;
    toggle_tray_icon(window_hwnd, false);
    taskbar_styling_thread.join();
    window::set_swca_style(run_data.taskbar, ACCENT_STATE::ACCENT_NORMAL);

    return 0;
}
