#ifndef STRNGS_H
#define STRNGS_H

#include <string>

namespace utils::strings {
    // white space characters
    const std::string WHITESPACE = " \n\r\t\f\v";

    /**
     * Trims white spaces on the left of a string
     *
     * @param str is the string to be trimmed
     * @returns std::string
     */
    std::string ltrim(const std::string &str);

    /**
     * Trims white spaces on the right of a string
     *
     * @param str is the string to be trimmed
     * @returns std::string
     */
    std::string rtrim(const std::string &str);

    /**
     * Trims white spaces on the right and left of a string
     *
     * @param str is the string to be trimmed
     * @returns std::string
     */
    std::string trim(const std::string &str);

    /**
     * Converts a string to lowercase
     *
     * @param str is the string to be lowered
     * @returns std::string
     */
    std::string toLower(const std::string &str);
} // namespace utils::strings

#endif