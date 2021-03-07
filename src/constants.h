#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Windows.h>
#include <string>

extern const UINT_PTR IDM_CONTEXT_EDIT_SETTINGS;
extern const UINT_PTR IDM_CONTEXT_HELP;
extern const UINT_PTR IDM_CONTEXT_EXIT;

static const UINT WM_USER_SHELLICON = 0x546;

#ifdef _DEBUG
extern const std::string LOG_FILE_PATH;
extern const std::string CONFIG_FILE_PATH;
#else
extern const std::string MY_APP_DATA_DIR;
extern const std::string CONFIG_FILE_PATH;
extern const std::string LOG_FILE_PATH;
#endif

#endif