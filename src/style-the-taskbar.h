#ifndef STYLE_TASKBAR_H
#define STYLE_TASKBAR_H

#include <Windows.h>
#include <nlohmann/json.hpp>

void style_the_taskbar(HWND taskbar, nlohmann::json config);

#endif