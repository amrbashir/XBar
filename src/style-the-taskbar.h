#ifndef STYLE_TASKBAR_H
#define STYLE_TASKBAR_H

#include <Windows.h>
#include <toml++/toml.h>

void style_the_taskbar(HWND taskbar, toml::table config);

#endif