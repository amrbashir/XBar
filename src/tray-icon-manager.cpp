#include "tray-icon-manager.h"
#include "constants.h"
#include "utils/logger.h"
#include <Windows.h>

using namespace utils;

const HICON LOGO_ICON = (HICON)LoadImage(NULL, "assets\\XBar.ico", IMAGE_ICON, 48, 48,
                                         LR_LOADFROMFILE | LR_DEFAULTSIZE);

void Tray_Icon_Manager::register_tray_icon(HWND hwnd) {

    NOTIFYICONDATAA icon_data  = {};
    icon_data.cbSize           = sizeof(icon_data);
    icon_data.hWnd             = hwnd;
    icon_data.uFlags           = NIF_ICON | NIF_MESSAGE;
    icon_data.hIcon            = LOGO_ICON;
    icon_data.uCallbackMessage = WM_USER_SHELLICON;
    icon_data.uVersion         = NOTIFYICON_VERSION_4;
    if (Shell_NotifyIcon(NIM_ADD, &icon_data) == FALSE) {
        logger::error("Failed to add tray icon");
    };

    if (Shell_NotifyIcon(NIM_SETVERSION, &icon_data) == FALSE) {
        logger::error("Failed to set tray icon version");
    }
}

void Tray_Icon_Manager::unregister_tray_icon(HWND hwnd) {
    NOTIFYICONDATAA icon_data = {};
    icon_data.cbSize          = sizeof(icon_data);
    icon_data.hWnd            = hwnd;
    Shell_NotifyIcon(NIM_DELETE, &icon_data);
}
