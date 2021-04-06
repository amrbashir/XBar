#ifndef CONFIG_H
#define CONFIG_H
#include <toml++/toml.h>

namespace Config_Manager {
    /**
     * Parses the config file and creates it if it doesn't exist
     */
    toml::table parse_config_file();
} // namespace Config_Manager
#endif