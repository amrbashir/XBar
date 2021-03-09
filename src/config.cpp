#include "config.h"
#include "constants.h"
#include <filesystem>
#include <fstream> //required for toml::parse_file()
#include <toml++/toml.h>

using namespace std;
namespace fs = std::filesystem;

toml::table Config_Manager::parse_config_file() {
#ifndef _DEBUG
    // creates the app directory if it doesn't exist
    CreateDirectory(MY_APP_DATA_DIR.c_str(), nullptr);
#endif

    // copy the default config to the app data directoy if it doesn't exist
    if (!fs::exists(CONFIG_FILE_PATH)) {
        fs::copy_file(fs::relative("assets\\Defaults.toml"), fs::absolute(CONFIG_FILE_PATH));
    }

    return toml::parse_file(CONFIG_FILE_PATH);
}