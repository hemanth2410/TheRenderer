#pragma once
#include "Window.h"
#include "PersistantTimer.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include "Mesh.h"
#include <set>

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
private:
	ImGuiManager imgui;
	Window wnd;
	PersistantTimer timer;
	float speed_factor = 1.0f;
	Camera cam;
	PointLight pointLight;
	Model Ethan{ wnd.Gfx(),"Models\\Ethan_WithAccessories\\char_EthanFullRig.fbx" };
	struct
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float scale = 1.0f;
	} pos;
};