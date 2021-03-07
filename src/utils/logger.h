#ifndef LOOGER_H
#define LOGGER_H

#include <string>

namespace utils::logger {
    void info(std::string message);
    void deug(std::string message);
    void error(std::string message);
} // namespace utils::logger

#endif