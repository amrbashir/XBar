#ifndef TYPES_H
#define TYPES_H

#include <Windows.h>
#include <nlohmann/json.hpp>

namespace types {
    struct RunData {
        HWND           taskbar;
        nlohmann::json config;
        bool           running;
    };

} // namespace types

#endif
