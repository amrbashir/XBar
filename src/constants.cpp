#include "utils/strings.h"
#include <ShlObj.h>
#include <string>

using namespace std;
using namespace utils;

#ifdef _DEBUG
extern const string LOG_FILE_PATH    = "..\\..\\xbar.log";
extern const string CONFIG_FILE_PATH = "..\\..\\xbar.json";
#else
string app_data_folder_path() {
    wchar_t *path;
    auto     result = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path);
    if (!SUCCEEDED(result)) {
        return "";
    }

    wstring save_path(path);
    CoTaskMemFree(path);

    return strings::wstring_to_string(save_path);
}

extern const string MY_APP_DATA_DIR  = app_data_folder_path() + "\\XBar";
extern const string CONFIG_FILE_PATH = app_data_folder_path() + "\\XBar\\xbar.json";
extern const string LOG_FILE_PATH    = app_data_folder_path() + "\\XBar\\xbar.log";
#endif
