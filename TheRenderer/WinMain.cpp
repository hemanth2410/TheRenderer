#include <Windows.h>
#include "WindowsMessageMap.h"
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wPram, LPARAM lParam)
{
	static WindowsMessageMap mm;
	// Convert the std::string to std::wstring
	std::string narrowStr = mm(msg, lParam, wPram);
	std::wstring wideStr(narrowStr.begin(), narrowStr.end());

	// Use OutputDebugString with the wide string
	OutputDebugString(wideStr.c_str());
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(69);
		break;
	case WM_KEYDOWN:
		if (wPram == 'F')
		{
			// Pay some respect
			SetWindowTextA(hWnd, "F for respect");
		}
		break;
	case WM_KEYUP:
		if (wPram == 'F')
		{
			SetWindowTextA(hWnd, "You released F");
		}
		break;
	case WM_CHAR:
		break;
	}
	return DefWindowProc(hWnd, msg, wPram, lParam);
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	const auto pClassName = L"HW3d";
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC; // Drawinf to window using graphics driver.
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	wc.hIconSm = nullptr;
	// register window class
	RegisterClassEx(&wc);
	// create window instance
	HWND hWnd = CreateWindowEx(
		0, pClassName,
		L"Happy hard window",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		200, 200, 640, 480,
		nullptr, nullptr, hInstance, nullptr
	);
	ShowWindow(hWnd, SW_SHOW);

	//Message pump
	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	if (gResult == -1)
	{
		return -1;
	}
	else {
		return msg.wParam;
	}
}