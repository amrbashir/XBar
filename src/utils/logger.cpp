#include "logger.h"
#include "../constants.h"
#include <ctime>
#include <fmt/core.h>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;
using namespace utils;

void log(string log_level, string message) {
#ifndef _DEBUG
    // creates the app directory if it doesn't exist
    CreateDirectory(MY_APP_DATA_DIR.c_str(), nullptr);
#endif

    // get the timestamp
    time_t    rawTime;
    struct tm timeInfo;
    char      dateAndTime[80];
    time(&rawTime);
    localtime_s(&timeInfo, &rawTime);
    strftime(dateAndTime, 80, "%d/%b/%y %I:%M %p", &timeInfo);

    ofstream logFile;
    logFile.open(LOG_FILE_PATH, ios_base::app);
    logFile << fmt::format("[ {0} - {1} ] {2}\n", log_level, dateAndTime, message);
    logFile.close();
}

void logger::info(string message) { log("Info", message); };

void logger::deug(string message) { log("Debug", message); };

void logger::error(string message) { log("Error", message); };
