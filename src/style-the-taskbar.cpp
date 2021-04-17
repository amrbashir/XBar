
#include "style-the-taskbar.h"
#include "utils/color.h"
#include "utils/logger.h"
#include "utils/strings.h"
#include "utils/window.h"
#include <Windows.h>
#include <array>
#include <nlohmann/json.hpp>
#include <string>
#include <tuple>

using namespace std;
using namespace utils;

ACCENT_STATE _parse_accent_state_from_string(string t_state) {
    const string state = strings::toLower(strings::trim(t_state));
    if (state == "normal")
        return ACCENT_STATE::ACCENT_NORMAL;
    if (state == "transparent")
        return ACCENT_STATE::ACCENT_ENABLE_TRANSPARENTGRADIENT;
    if (state == "blur")
        return ACCENT_STATE::ACCENT_ENABLE_BLURBEHIND;
    if (state == "fluent")
        return ACCENT_STATE::ACCENT_ENABLE_ACRYLICBLURBEHIND;
    if (state == "opaque")
        return ACCENT_STATE::ACCENT_ENABLE_GRADIENT;
    return ACCENT_STATE::ACCENT_NORMAL;
}

// rule are expected to be `color/accentState` like `#ffffff/discord.exe`
tuple<ACCENT_STATE, array<uint8_t, 4>> _parse_rule(string t_rule) {
    const string rule              = strings::trim(t_rule);
    const string rule_color        = rule.substr(0, rule.find_first_of("/"));
    const string rule_accent_state = rule.substr(rule.find_first_of("/") + 1, string::npos);

    const ACCENT_STATE      state = _parse_accent_state_from_string(rule_accent_state);
    const array<uint8_t, 4> color = color::rgba_from_hex_str(rule_color);

    return tuple<ACCENT_STATE, array<uint8_t, 4>> { state, color };
}

void _apply_style(HWND taskbar, nlohmann::json config, HWND window, string taskbar_state) {
    const auto   rules            = config[taskbar_state]["rules"];
    const string exe_path         = window::get_exe_path(window);
    const string exe_name         = exe_path.substr(exe_path.find_last_of("\\") + 1, string::npos);
    const string exe_name_lowered = strings::toLower(exe_name);

    // a rule has been found for the current window
    if (rules.contains(exe_name_lowered)) {
        const auto [accent_state, color] = _parse_rule(rules[exe_name_lowered].get<string>());
        window::set_style(taskbar, accent_state, color);

    } else {
        const auto accent_state_str = config[taskbar_state]["accentState"].get<string>();
        const auto color_str        = config[taskbar_state]["color"].get<string>();

        const array<uint8_t, 4> color        = color::rgba_from_hex_str(color_str);
        const ACCENT_STATE      accent_state = _parse_accent_state_from_string(accent_state_str);

        window::set_style(taskbar, accent_state, color);
    };
}

void style_the_taskbar(HWND taskbar, nlohmann::json config) {
    HWND top_most_window           = GetTopWindow(GetDesktopWindow());
    HWND top_most_maximized_window = nullptr;
    bool maximized_window_exists   = false;

    // loop over windows in z-order starting from the top most and
    // check if there is a maximized window
    HWND window = GetTopWindow(GetDesktopWindow());
    do {
        if (window::exists_in_taskbar(window)
            && window::is_window_maximized(window)
            // makes sure we set the top_most_maximized_window only once
            && !maximized_window_exists) {
            top_most_maximized_window = window;
            maximized_window_exists   = true;
        }
    } while (window = GetWindow(window, GW_HWNDNEXT));

    if (maximized_window_exists) {
        _apply_style(taskbar, config, top_most_maximized_window, "maximized");
    } else {
        _apply_style(taskbar, config, top_most_window, "regular");
    }
}