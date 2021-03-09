
#ifndef CONTEXT_MENU_MANGER_H
#define CONTEXT_MENU_MANGER_H

#include <Windows.h>

namespace Context_Menu_Manager {
    void create_context_menu(HWND hwnd);
    void context_menu_item_click(HWND hwnd, WPARAM wParam);
} // namespace Context_Menu_Manager
#endif