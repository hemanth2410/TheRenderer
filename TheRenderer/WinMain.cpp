#include "App.h"
#include <string>
#include <sstream>

std::wstring ConvertToWideString(const char* str) {
	return std::wstring(str, str + strlen(str));
}

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPervInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	try {
		return App{}.Go();
	}
	catch (const ChiliException& e)
	{
		
		MessageBox(nullptr, ConvertToWideString(e.what()).c_str(), ConvertToWideString(e.GetType()).c_str(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBox(nullptr, ConvertToWideString(e.what()).c_str(), L"Standard exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, L"No details available", L"Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}