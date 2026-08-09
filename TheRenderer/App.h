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
	void ShowRawInputWindow();
private:
	int x = 0, y = 0;
	ImGuiManager imgui;
	Window wnd;
	PersistantTimer timer;
	float speed_factor = 1.0f;
	Camera cam;
	PointLight pointLight;
	Model Ethan{ wnd.Gfx(),"Models\\NanoSuit_Textured\\nanosuit.obj", "Models\\NanoSuit_Textured\\"};
	Model Ethan2{ wnd.Gfx(),"Models\\Bricks\\Bricks.obj", "Models\\Bricks\\"};
	Vector3 movementVecor;
	Vector3 rotationDelta;
	/*struct
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float scale = 1.0f;
	} pos;*/
};