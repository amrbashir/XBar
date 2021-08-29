#ifndef TYPES_H
#define TYPES_H

#include <Windows.h>
#include <nlohmann/json.hpp>

namespace types {
    struct RunData {
        HWND           taskbar;
        nlohmann::json config;
        bool           running;
        LPCSTR         window_class;
        LPCSTR         tray_window_name;
        LPCSTR         instance_mutex_name;
    };

} // namespace types

#endif
