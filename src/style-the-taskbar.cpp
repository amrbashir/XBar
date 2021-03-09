
#include "style-the-taskbar.h"
#include "utils/color.h"
#include "utils/logger.h"
#include "utils/strings.h"
#include "utils/window.h"
#include <Windows.h>
#include <array>
#include <string>
#include <toml++/toml.h>
#include <tuple>

using namespace std;
using namespace utils;

ACCENT_STATE _parse_accent_state_from_string(string state) {
    const string trimmed_state = strings::trim(state);
    if (trimmed_state == "normal")
        return ACCENT_STATE::ACCENT_NORMAL;
    if (trimmed_state == "transparent")
        return ACCENT_STATE::ACCENT_ENABLE_TRANSPARENTGRADIENT;
    if (trimmed_state == "blur")
        return ACCENT_STATE::ACCENT_ENABLE_BLURBEHIND;
    if (trimmed_state == "fluent")
        return ACCENT_STATE::ACCENT_ENABLE_ACRYLICBLURBEHIND;
    if (trimmed_state == "opaque")
        return ACCENT_STATE::ACCENT_ENABLE_GRADIENT;
    return ACCENT_STATE::ACCENT_NORMAL;
}

// rule are expected to be `color/accentState` like `#ffffff/discord.exe`
tuple<ACCENT_STATE, array<uint8_t, 4>> _parse_rule(string rule) {
    const string trimmed_rule = strings::trim(rule);
    const string color        = trimmed_rule.substr(0, trimmed_rule.find_first_of("/"));
    const string accent_state
        = trimmed_rule.substr(trimmed_rule.find_first_of("/") + 1, string::npos);

    return tuple<ACCENT_STATE, array<uint8_t, 4>> { _parse_accent_state_from_string(accent_state),
                                                    color::from_hex_str(color) };
}

void _apply_style(HWND taskbar, toml::table config, HWND window, string taskbar_state) {
    const string path    = window::get_window_exe_path(window);
    const string exeName = strings::toLower(path.substr(path.find_last_of("\\") + 1, string::npos));

    const auto rules = config[taskbar_state]["Rules"];
    // a rule has been found for the current window
    if (rules[exeName]) {
        const auto [accent_state, color] = _parse_rule(rules[exeName].value<string>().value());
        window::set_window_style(taskbar, accent_state, color);
    } else {
        const auto accent_state_str = config[taskbar_state]["AccentState"].value<string>().value();
        const auto color_string     = config[taskbar_state]["Color"].value<string>().value();
        const array<uint8_t, 4> color        = color::from_hex_str(color_string);
        const ACCENT_STATE      accent_state = _parse_accent_state_from_string(accent_state_str);
        window::set_window_style(taskbar, accent_state, color);
    };
}

void style_the_taskbar(HWND taskbar, toml::table config) {
    bool maximized_window_exists = false;
    HWND top_most_window         = GetTopWindow(GetDesktopWindow());
    HWND top_most_maximized_window;

    // loop over windows in z-order starting from the top most and
    // check if there is a maximized window
    HWND window = GetTopWindow(GetDesktopWindow());
    do {
        if (window::exists_in_taskbar(window)
            && window::iw_window_maximized(window)
            // makes sure we set the top_most_maximized_window only once
            && !maximized_window_exists) {
            top_most_maximized_window = window;
            maximized_window_exists   = true;
        }
    } while (window = GetWindow(window, GW_HWNDNEXT));

    if (maximized_window_exists) {
        _apply_style(taskbar, config, top_most_maximized_window, "MaximizedWindow");
    } else {
        _apply_style(taskbar, config, top_most_window, "Regular");
    }
}