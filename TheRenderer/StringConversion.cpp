#include "StringConversion.h"
#include <cstdlib> // for mbstowcs_s and wcstombs_s

std::wstring ConvertToWideString(const std::string& str) {
    size_t len = str.length() + 1; // length including null-terminator
    std::wstring wstr(len, L'\0'); // Initialize a wide string with enough space
    size_t convertedChars = 0; // To store the number of characters converted

    // Use mbstowcs_s for safe conversion
    mbstowcs_s(&convertedChars, &wstr[0], len, str.c_str(), len);

    // Resize the string to fit the actual converted characters (minus null-terminator)
    wstr.resize(convertedChars - 1);
    return wstr;
}

std::wstring ConvertToWideString(const char* str) {
    return std::wstring(str, str + strlen(str));
}

std::string ConvertToNarrowString(const std::wstring& wstr) {
    size_t len = wstr.length() + 1; // Length including null-terminator
    std::string str(len, '\0');     // Initialize a string with enough space
    size_t convertedChars = 0;      // To store the number of characters converted

    // Use wcstombs_s for safe conversion
    wcstombs_s(&convertedChars, &str[0], len, wstr.c_str(), len);

    // Resize the string to fit the actual converted characters (minus null-terminator)
    str.resize(convertedChars - 1);
    return str;
}
