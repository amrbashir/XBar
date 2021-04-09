#ifndef WINDOW_H
#define WINDOW_H

#include "user32.h"
#include <Windows.h>
#include <array>
#include <string>

namespace utils::window {
    /**
     * Styles a window given its HWND
     *
     * @param hwnd is a handle to the window that nees to be styled
     * @param accent is used to give the window different appearances @see user32.h
     * @param rgba_array is that contains Red,Green,Blue,Alpha values respectively, each one has min
     * value of 0 and max of 255
     * @returns void
     */
    void set_style(HWND hwnd, ACCENT_STATE accent,
                          std::array<uint8_t, 4> rgba_array = { 0, 0, 0, 0 });

    /**
     * Retrieves the path of the executable associated with the window given its HWND
     *
     * @param hwnd is a HWND to the window
     * @returns std::string
     */
    std::string get_exe_path(HWND hwnd);

    /**
     * Checks if the window is maximized or not given its HWND
     *
     * @param hwnd is a HWND to the window
     * @returns bool
     */
    bool is_window_maximized(HWND hwnd);

    /**
     * Checks if the window is in Taskbar aka can be switched to by using Alt+Tab
     *
     * Credits of this function implementation @see http://www.dfcd.net/projects/switcher/switcher.c
     *
     * @param hwnd is a HWND to the window
     * @returns bool
     */
    bool exists_in_taskbar(HWND hwnd);
} // namespace utils::window

#endif
