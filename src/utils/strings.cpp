#include "strings.h"
#include <cctype>
#include <string>

using namespace std;
using namespace utils;

string strings::ltrim(const string &str) {
    size_t start = str.find_first_not_of(strings::WHITESPACE);
    return (start == string::npos) ? "" : str.substr(start);
}

string strings::rtrim(const string &str) {
    size_t end = str.find_last_not_of(strings::WHITESPACE);
    return (end == string::npos) ? "" : str.substr(0, end + 1);
}

string strings::trim(const string &str) { return strings::rtrim(strings::ltrim(str)); }

string strings::toLower(const string &str) {
    string lowered = "";
    for (auto c : str) {
        lowered += tolower(c);
    }
    return lowered;
};
