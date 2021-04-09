#ifndef COLOR_H
#define COLOR_H

#include <array>
#include <string>

namespace utils::color {
    /**
     * Converts a hex color string to an array of the RGBA values
     *
     * @param hex is a hex color string,
     * it can be shorthand like `#FFF`, full like `#FFFFFF` or with alpha values
     * like `#FFFFFFFF` it also can be with the hash symbole or without it
     * @returns std::array<uint8_t, 4>
     */
    std::array<uint8_t, 4> rgba_from_hex_str(std::string hex);

} // namespace utils::color

#endif
