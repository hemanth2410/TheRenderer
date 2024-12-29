#pragma once
#include "Window.h"
#include "PersistantTimer.h"
#include "ImGuiManager.h"
#include "Camera.h"
class App
{
public:
	App();
	// master frame / message loop
	int Go();
	~App();
private:
	void DoFrame();
	void ResetSimulationSpeed(float value);
private:
	ImGuiManager imgui;
	Window wnd;
	PersistantTimer timer;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	bool showDemoWindow = true;
	float simulationSpeed = 1.0f;
	Camera cam;
	static constexpr size_t nDrawables = 180;
};