
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

ACCENT_STATE get_accent_state(string t_state) {
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

// A rule is expected to be `color/accentState`, ex: `#ffffff/opaque`
tuple<ACCENT_STATE, array<uint8_t, 4>> parse_rule(string t_rule) {
    const string rule             = strings::trim(t_rule);
    const string color_str        = rule.substr(0, rule.find_first_of("/"));
    const string accent_state_str = rule.substr(rule.find_first_of("/") + 1, string::npos);

    const array<uint8_t, 4> color = color::rgba_from_hex_str(color_str);
    const ACCENT_STATE      state = get_accent_state(accent_state_str);

    return tuple<ACCENT_STATE, array<uint8_t, 4>> { state, color };
}

void apply_style_for_taskbar_state(HWND taskbar, nlohmann::json config, HWND window,
                                   string taskbar_state) {
    const auto   rules    = config[taskbar_state]["rules"];
    const string exe_path = window::get_exe_path(window);
    const string exe_name
        = strings::toLower(exe_path.substr(exe_path.find_last_of("\\") + 1, string::npos));

    // a rule has been found for the current window
    if (rules.contains(exe_name)) {
        const auto [accent_state, color] = parse_rule(rules[exe_name].get<string>());
        window::set_swca_style(taskbar, accent_state, color);
    } else {
        const auto color_str        = config[taskbar_state]["color"].get<string>();
        const auto accent_state_str = config[taskbar_state]["accentState"].get<string>();

        const array<uint8_t, 4> color        = color::rgba_from_hex_str(color_str);
        const ACCENT_STATE      accent_state = get_accent_state(accent_state_str);

        window::set_swca_style(taskbar, accent_state, color);
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
            && window::is_maximized(window)
            // makes sure we set the top_most_maximized_window only once
            && !maximized_window_exists) {
            top_most_maximized_window = window;
            maximized_window_exists   = true;
        }
    } while (window = GetWindow(window, GW_HWNDNEXT));

    if (maximized_window_exists) {
        apply_style_for_taskbar_state(taskbar, config, top_most_maximized_window, "maximized");
    } else {
        apply_style_for_taskbar_state(taskbar, config, top_most_window, "regular");
    }
}