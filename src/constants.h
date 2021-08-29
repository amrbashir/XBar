#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Windows.h>
#include <string>

static const UINT_PTR MENU_ITEM_EDIT_SETTINGS = 1000;
static const UINT_PTR MENU_ITEM_RELOAD_CONFIG = 1001;
static const UINT_PTR MENU_ITEM_HELP          = 1002;
static const UINT_PTR MENU_ITEM_EXIT          = 1003;

static const UINT WM_USER_TRAYICON = 546;

#ifdef _DEBUG
extern const std::string LOG_FILE_PATH;
extern const std::string CONFIG_FILE_PATH;
#else
extern const std::string MY_APP_DATA_DIR;
extern const std::string CONFIG_FILE_PATH;
extern const std::string LOG_FILE_PATH;
#endif

#endif