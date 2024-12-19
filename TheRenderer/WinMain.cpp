#include "Window.h"
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
		Window wnd(600, 480, "Renderer");
		MSG msg;
		BOOL gResult;
		while ((gResult = GetMessageW(&msg, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg); // Automatically picks the correct version
			if (wnd.keyboard.KeyIsPressed(VK_SPACE))
			{
				MessageBox(nullptr, L"Something happon!", L"Space key was pressed", MB_OK | MB_ICONEXCLAMATION);
			}
			while (!wnd.mouse.IsEmpty())
			{
				const auto e = wnd.mouse.Read();
				if(e.value().GetType() == Mouse::Event::Type::Move)
				{
					std::ostringstream oss;
					oss << "Mouse Position : (" << e.value().GetPosX() << ";" << e.value().GetPosY() << ")";
					wnd.SetTitle(oss.str());

				}
			}
		}
		if (gResult == -1)
		{
			return -1;
		}
		return msg.wParam;
		return 0;
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