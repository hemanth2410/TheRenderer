#include "App.h"
#include <sstream>

App::App() :wnd(800, 600, "The Donkey fart box") {}

int App::Go()
{
	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}
		DoFrame();
	}
}

void App::DoFrame()
{
	/*float t = timer.Peek();
	std::ostringstream oss;
	oss << "Time elapsed : " << std::setprecision(1) << std::fixed << t << "sec";
	wnd.SetTitle(oss.str());*/
	float startTime = timer.Peek();
	//const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	wnd.Gfx().ClearBuffer(0.0f, 0.0f, 0.0f);
	wnd.Gfx().DrawTestTriangle(timer.Peek(), 1.5,0, 4.0f);
	wnd.Gfx().DrawTestTriangle(timer.Peek(), 0,0,10.0f);
	wnd.Gfx().DrawTestTriangle(timer.Peek(), -10,0,20.0f);
	wnd.Gfx().EndFrame();
	float endTime = timer.Peek();
	float delta = endTime - startTime;
	float fps = 1.0f / delta;
	std::ostringstream ss;
	ss << fps;
	std::string s(ss.str());
	wnd.SetTitle(ss.str());
}