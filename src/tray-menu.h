
#ifndef TRAY_MENU_H
#define TRAY_MENU_H

#include "types.h"
#include <Windows.h>

namespace TrayMenu {
    void show(HWND hwnd);
    void on_menu_item_click(HWND hwnd, WPARAM wParam, types::RunData &run_data);
} // namespace TrayMenu
#endif