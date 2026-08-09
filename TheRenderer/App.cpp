#include "App.h"
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
	Ethan2.Draw(wnd.Gfx());
	Ethan2.ShowWindow("Model Window # 2");
	//ShowRawInputWindow();
	pointLight.Draw(wnd.Gfx());

	// imgui windows
	//cam.SpawnControlWindow();
	pointLight.SpawnControlWindow();
	//ShowModelWindow();
	cam.Translate(movementVecor, dt);
	cam.Rotate(rotationDelta.x, rotationDelta.y, dt);
	// present
	wnd.Gfx().EndFrame();
	//wnd.DisableCursor();
	while (const auto e = wnd.keyboard.ReadKey())
	{
		if (!e.IsPress())
		{
			continue;
		}
		switch(e.GetCode())
		{
		case VK_ESCAPE:
			if (wnd.CursorEnabled())
			{
				wnd.DisableCursor();
				wnd.mouse.EnableRaw();
			}
			else
			{
				wnd.EnableCursor();
				wnd.mouse.DisableRaw();
			}
			break;

			/*if (wnd.CursorEnabled())
			{
				wnd.DisableCursor();
				wnd.mouse.EnableRaw();
			}
			else
			{
				wnd.EnableCursor();
				wnd.mouse.DisableRaw();
			}*/
		}
	}
	if (!wnd.CursorEnabled())
	{

		movementVecor.z = wnd.keyboard.KeyIsPressed('W') ? 1.0f : wnd.keyboard.KeyIsPressed('S') ? -1.0 : 0;
		movementVecor.x = wnd.keyboard.KeyIsPressed('D') ? 1.0f : wnd.keyboard.KeyIsPressed('A') ? -1.0 : 0;
		movementVecor.y = wnd.keyboard.KeyIsPressed('E') ? 1.0f : wnd.keyboard.KeyIsPressed('Q') ? -1.0 : 0;
	}
	rotationDelta = Vector3();
	while (const auto delta = wnd.mouse.ReadRawDelta())
	{
		if (!wnd.CursorEnabled())
		{
			rotationDelta = Vector3(delta->x, delta->y, 0);
		}
	}
	//rotationDelta = Vector3();
}

App::~App()
{}

void App::ShowRawInputWindow()
{
	while (const auto d = wnd.mouse.ReadRawDelta())
	{
		x += d->x;
		y += d->y;
	}
	if (ImGui::Begin("Raw Input"))
	{
		ImGui::Text("Delta : (%d,%d)", x, y);
	}
	ImGui::End();
}

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