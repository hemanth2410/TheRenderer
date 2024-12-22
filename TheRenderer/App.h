#pragma once
#include "Window.h"
#include "PersistantTimer.h"

class App
{
public:
	App();
	int Go();

private:
	void DoFrame();
private:
	Window wnd;
	PersistantTimer timer;
};
