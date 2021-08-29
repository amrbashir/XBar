#include "window.h"
#include "user32.h"
#include <Psapi.h>
#include <Windows.h>
#include <array>
#include <string>

using namespace std;
using namespace utils;

void window::set_swca_style(HWND hwnd, ACCENT_STATE accent, array<uint8_t, 4> rgba_array) {
    static const auto SetWindowCompositionAttribute
        = reinterpret_cast<PFN_SET_WINDOW_COMPOSITION_ATTRIBUTE>(
            GetProcAddress(GetModuleHandle("user32.dll"), "SetWindowCompositionAttribute"));

    // sending `WM_THEMECHANGED` to the given HWND makes it restore its original effect
    if (accent == ACCENT_STATE::ACCENT_NORMAL) {
        SendMessage(hwnd, WM_THEMECHANGED, 0, 0);
        return;
    }

    uint8_t red   = rgba_array[0];
    uint8_t green = rgba_array[1];
    uint8_t blue  = rgba_array[2];
    // Acrylic doesn't like 0 opacity/alpha => credit goes to TranslucentTB project.
    uint8_t alpha = accent == ACCENT_STATE::ACCENT_ENABLE_ACRYLICBLURBEHIND && rgba_array[3] == 0
        ? 1
        : rgba_array[3];

    ACCENT_POLICY policy = {
        accent,
        2,
        static_cast<uint32_t>((red << 0) | (green << 8) | (blue << 16) | (alpha << 24)),
        0,
    };

    const WINDOWCOMPOSITIONATTRIBDATA data = {
        WCA_ACCENT_POLICY,
        &policy,
        sizeof(policy),
    };

    SetWindowCompositionAttribute(hwnd, &data);
}

string window::get_exe_path(HWND hwnd) {
    DWORD proc_id;
    TCHAR path[MAX_PATH];

    GetWindowThreadProcessId(hwnd, &proc_id);
    HANDLE proc_handle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, proc_id);
    GetModuleFileNameEx(proc_handle, nullptr, path, MAX_PATH);
    CloseHandle(proc_handle);

    return path;
}

bool window::is_maximized(HWND hwnd) {
    WINDOWPLACEMENT wp = {};
    wp.length          = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(hwnd, &wp);

    return wp.showCmd == SW_SHOWMAXIMIZED;
}

bool window::exists_in_taskbar(HWND hwnd) {

    /**
     * Credits of this function implementation @see http://www.dfcd.net/projects/switcher/switcher.c
     */

    TITLEBARINFO ti;
    HWND         hwndTry, hwndWalk = nullptr;

    if (!IsWindowVisible(hwnd))
        return false;

    hwndTry = GetAncestor(hwnd, GA_ROOTOWNER);
    while (hwndTry != hwndWalk) {
        hwndWalk = hwndTry;
        hwndTry  = GetLastActivePopup(hwndWalk);
        if (IsWindowVisible(hwndTry))
            break;
    }
    if (hwndWalk != hwnd)
        return false;

    // the following removes some task tray programs and "Program Manager"
    ti.cbSize = sizeof(ti);
    GetTitleBarInfo(hwnd, &ti);
    if (ti.rgstate[0] & STATE_SYSTEM_INVISIBLE)
        return false;

    // Tool windows should not be displayed either, these do not appear in the
    // task bar.
    if (GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW)
        return true;

    return true;
}