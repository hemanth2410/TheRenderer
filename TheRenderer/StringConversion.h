#pragma once
#include <string>

// Converts a narrow string (std::string) to a wide string (std::wstring)
std::wstring ConvertToWideString(const std::string& str);

// Converts a C-style narrow string to a wide string
std::wstring ConvertToWideString(const char* str);

// Converts a wide string (std::wstring) to a narrow string (std::string)
std::string ConvertToNarrowString(const std::wstring& wstr);
