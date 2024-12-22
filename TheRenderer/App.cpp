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
	float t = timer.Peek();
	std::ostringstream oss;
	oss << "Time elapsed : " << std::setprecision(1) << std::fixed << t << "sec";
	wnd.SetTitle(oss.str());
}