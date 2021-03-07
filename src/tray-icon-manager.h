#ifndef TRAY_ICON_MANGER_H
#define TRAY_ICON_MANGER_H

#include <Windows.h>

namespace Tray_Icon_Manager {
    void register_tray_icon(HWND hwnd);
    void unregister_tray_icon(HWND hwnd);
} // namespace Tray_Icon_Manager

#endif