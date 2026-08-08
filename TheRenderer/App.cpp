#include "App.h"
#include "AssTest.h"
#include <memory>
#include <algorithm>
#include "GeometryMath.h"
#include "Surface.h"
#include "GDIPlusManager.h"
#include "imgui/imgui.h"
#include "GameCoordinates.h"
namespace dx = DirectX;

GDIPlusManager gdipm;

App::App()
	:
	wnd(1366, 768, "Render Window"),
	pointLight(wnd.Gfx(), 0.15f)
{
	wnd.Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 768.0f / 1366.0f, 0.5f, GameCoordinates::MetersToCentimeters(200)));
}

void App::DoFrame()
{
	const auto dt = timer.Mark() * speed_factor;
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	wnd.Gfx().SetCamera(cam.GetMatrix());
	pointLight.Bind(wnd.Gfx(), cam.GetMatrix());

	/*const auto transform = dx::XMMatrixRotationRollPitchYaw(pos.roll, pos.pitch, pos.yaw) *
		dx::XMMatrixTranslation(pos.x, pos.y, pos.z) * dx::XMMatrixScaling(pos.scale, pos.scale, pos.scale);*/
	Ethan.Draw(wnd.Gfx());
	Ethan.ShowWindow("Model Window");
	pointLight.Draw(wnd.Gfx());

	// imgui windows
	cam.SpawnControlWindow();
	pointLight.SpawnControlWindow();
	//ShowModelWindow();

	// present
	wnd.Gfx().EndFrame();
}

/*void App::ShowModelWindow()
{
	if (ImGui::Begin("Model"))
	{
		using namespace std::string_literals;

		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &pos.roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &pos.pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &pos.yaw, -180.0f, 180.0f);

		ImGui::Text("Position");
		ImGui::SliderFloat("X", &pos.x, -500.0f, 500.0f);
		ImGui::SliderFloat("Y", &pos.y, -500.0f, 500.0f);
		ImGui::SliderFloat("Z", &pos.z, -500.0f, 500.0f);

		ImGui::Text("Scale");
		ImGui::SliderFloat("Scale", &pos.scale, 0.0f, 1.0f);
	}
	ImGui::End();
}*/

App::~App()
{}


int App::Go()
{
	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = Window::ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		DoFrame();
	}
}