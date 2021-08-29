#include "config-manager.h"
#include "constants.h"
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

nlohmann::json migrate_config(nlohmann::json user_config) {
    bool modified = false;

    auto keys = vector<string> { "general", "regular", "maximized" };
    for (auto key : keys) {
        if (!user_config.contains(key)) {
            user_config[key] = nlohmann::json::object();
            modified         = true;
        }
    }

    auto general_keys          = vector<string> { "runAtStartup", "showTrayIcon" };
    auto general_keys_defaults = vector<string> { "true", "true" };
    for (int i = 0; i < general_keys.size(); i++) {
        if (!user_config["general"].contains(general_keys[i])) {
            if (general_keys_defaults[i] == "true") {
                user_config["general"][general_keys[i]] = true;
            } else if (general_keys_defaults[i] == "false") {
                user_config["general"][general_keys[i]] = false;
            } else {
                user_config["general"][general_keys[i]] = general_keys_defaults[i];
            }
            modified = true;
        }
    }

    auto states              = vector<string> { "regular", "maximized" };
    auto state_keys          = vector<string> { "color", "accentState", "rules" };
    auto state_keys_defaults = vector<string> { "#1f1f1f", "opaque", "<<object>>" };
    for (auto state : states) {
        for (int i = 0; i < state_keys.size(); i++) {
            if (!user_config[state].contains(state_keys[i])) {
                if (state_keys_defaults[i] == "<<object>>") {
                    user_config[state][state_keys[i]] = nlohmann::json::object();
                } else {
                    user_config[state][state_keys[i]] = state_keys_defaults[i];
                }
                modified = true;
            }
        }
    }

    if (modified) {
        std::ofstream json_file_output(CONFIG_FILE_PATH);
        json_file_output << std::setw(4) << user_config << std::endl;
    }

    return user_config;
}

nlohmann::json Config_Manager::parse_config_file() {
#ifndef _DEBUG
    // creates the app directory if it doesn't exist
    CreateDirectory(MY_APP_DATA_DIR.c_str(), nullptr);
#endif

    // copy the default config to the app data directoy if it doesn't exist
    if (!fs::exists(CONFIG_FILE_PATH)) {
        fs::copy_file(fs::relative("assets\\Defaults.json"), fs::absolute(CONFIG_FILE_PATH));
    }

    // read the config json
    std::ifstream  json_file_input(CONFIG_FILE_PATH);
    nlohmann::json json;
    json_file_input >> json;

    // migrate and return json
    nlohmann::json migrated_config = migrate_config(json);
    return migrated_config;
}
