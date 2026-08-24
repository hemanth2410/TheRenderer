#pragma once
#include "Window.h"
#include "PersistantTimer.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include "Mesh.h"
#include <set>
#include "TestPlane.h"
#include "Stencil.h"
#include "TestCube.h"
#include "FrameCommander.h"
#include "Model.h"
#include "ScriptCommander.h"
#include "Material.h"
class App
{
public:
	App();
	// master frame / message loop
	int Go();
	~App();
private:
	void DoFrame();
	void ShowModelWindow();
	void ShowRawInputWindow();
private:
	int x = 0, y = 0;
	bool showDemoWindow = false;
	ImGuiManager imgui;
	Window wnd;
	//ScriptCommander scriptCommander;
	PersistantTimer timer;
	float speed_factor = 1.0f;
	Camera cam;
	FrameCommander fc{ wnd.Gfx() };
	PointLight light;
	TestCube cube{ wnd.Gfx(),1.0f };
	//TestCube cube2{ wnd.Gfx(),4.0f };
	Model sponza{ wnd.Gfx(),"Models\\sponza_palace\\scene.gltf",1.0f };
	Vector3 movementVecor;
	Vector3 rotationDelta;
};