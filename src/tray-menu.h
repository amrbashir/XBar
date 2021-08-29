
#ifndef TRAY_MENU_H
#define TRAY_MENU_H

#include <Windows.h>
#include <nlohmann/json.hpp>

namespace TrayMenu {
    void show(HWND hwnd);
    void on_menu_item_click(HWND hwnd, WPARAM wParam, nlohmann::json &config);
} // namespace TrayMenu
#endif