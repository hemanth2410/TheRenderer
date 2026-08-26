#pragma once
#include "Window.h"
#include "PersistantTimer.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include "Stencil.h"
#include "TestCube.h"
#include "Model.h"
#include "ScriptCommander.h"
#include "BlurOutlineRenderGraph.h"
class App
{
public:
	App();
	// master frame / message loop
	int Go();
	~App();
private:
	void DoFrame(float deltaTime);
	void HandleInput(float deltaTime);
private:
	int x = 0, y = 0;
	bool showDemoWindow = false;
	ImGuiManager imgui;
	Window wnd;
	//ScriptCommander scriptCommander;
	Rgph::BlurOutlineRenderGraph rg{ wnd.Gfx() };
	PersistantTimer timer;
	float speed_factor = 1.0f;
	Camera cam;
	PointLight light;
	TestCube cube{ wnd.Gfx(),1.0f };
	//TestCube cube2{ wnd.Gfx(),4.0f };
	Model sponza{ wnd.Gfx(),"Models\\sponza_palace\\scene.gltf",1.0f };
	Vector3 movementVecor;
	Vector3 rotationDelta;
};