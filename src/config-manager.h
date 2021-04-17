#ifndef CONFIG_H
#define CONFIG_H
#include <nlohmann/json.hpp>

namespace Config_Manager {
    /**
     * Parses the config file and creates it if it doesn't exist
     */
    nlohmann::json parse_config_file();
} // namespace Config_Manager
#endif