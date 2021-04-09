#include "color.h"
#include "logger.h"
#include "strings.h"
#include <array>
#include <fmt/format.h>
#include <string>

using namespace std;
using namespace utils;

array<uint8_t, 4> color::rgba_from_hex_str(string hex) {
    // trim white spaces
    const string trimmed_hex = strings::trim(hex);
    // remove the `#`
    const string hex_no_hash = trimmed_hex.starts_with("#") ? trimmed_hex.substr(1) : trimmed_hex;
    // if hex is 3-digit `ex: FFF`, convert it to 6-digit, otherwise leave as is.
    const string full_hex = hex_no_hash.length() == 3
        ? fmt::format("{0}{1}{2}{3}{4}{5}", hex_no_hash[0], hex_no_hash[0], hex_no_hash[1],
                      hex_no_hash[1], hex_no_hash[2], hex_no_hash[2])
        : hex_no_hash;

    const bool correct_hex_format = full_hex.length() == 6 || full_hex.length() == 8;
    if (correct_hex_format) {
        // if 6-digit hex provided then add `FF` to its alpha channel
        const string hex_8_digit = full_hex.length() == 6 ? "FF" + full_hex : full_hex;

        // split channels
        const string alpha = hex_8_digit.substr(0, 2);
        const string red   = hex_8_digit.substr(2, 2);
        const string green = hex_8_digit.substr(4, 2);
        const string blue  = hex_8_digit.substr(6, 2);

        // parse the channels string values into uint8_t
        const uint8_t r = stoi(red.c_str(), 0, 16);
        const uint8_t g = stoi(green.c_str(), 0, 16);
        const uint8_t b = stoi(blue.c_str(), 0, 16);
        const uint8_t a = stoi(alpha.c_str(), 0, 16);

        return array<uint8_t, 4> { r, g, b, a };
    } else {
        return array<uint8_t, 4> { 0, 0, 0, 255 };
    }
}
