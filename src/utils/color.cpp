#include "color.h"
#include "logger.h"
#include "strings.h"
#include <array>
#include <fmt/format.h>
#include <string>

using namespace std;
using namespace utils;

array<uint8_t, 4> color::from_hex_str(string hex) {
    // trim white spaces
    const string trimmed_hex = strings::trim(hex);
    // remove the `#`
    const string hex_no_hash = trimmed_hex.starts_with("#") ? trimmed_hex.substr(1) : trimmed_hex;
    // convert short hex format FFF to full hex format FFFFFF
    const string full_hex = hex_no_hash.length() == 3
        ? fmt::format("{0}{1}{2}{3}{4}{5}", hex_no_hash[0], hex_no_hash[0], hex_no_hash[1],
                      hex_no_hash[1], hex_no_hash[2], hex_no_hash[2])
        : hex_no_hash;

    if (full_hex.length() == 6 || full_hex.length() == 8) {
        string alpha, red, green, blue;

        if (full_hex.length() == 6) {
            alpha = "FF";
            red   = full_hex.substr(0, 2);
            green = full_hex.substr(2, 2);
            blue  = full_hex.substr(4, 2);
        } else {
            alpha = full_hex.substr(0, 2);
            red   = full_hex.substr(2, 2);
            green = full_hex.substr(4, 2);
            blue  = full_hex.substr(6, 2);
        }

        const uint8_t a = stoi(alpha.c_str(), 0, 16);
        const uint8_t r = stoi(red.c_str(), 0, 16);
        const uint8_t g = stoi(green.c_str(), 0, 16);
        const uint8_t b = stoi(blue.c_str(), 0, 16);

        return array<uint8_t, 4> { r, g, b, a };
    } else {
        logger::error(fmt::format("Provided color `{0}` is not in the right format\n "
                                  "returning a default black color `#000000`.",
                                  hex));
        return array<uint8_t, 4> { 0, 0, 0, 255 };
    }
}
