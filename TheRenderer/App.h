#pragma once
#include "Window.h"
#include "PersistantTimer.h"
#include "ImGuiManager.h"
class App
{
public:
	App();
	// master frame / message loop
	int Go();
	~App();
private:
	void DoFrame();
private:
	ImGuiManager imgui;
	Window wnd;
	PersistantTimer timer;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	static constexpr size_t nDrawables = 180;
};