#include "App.h"
#include <algorithm>
#include "GeometryMath.h"
#include "imgui/imgui.h"
#include "GameCoordinates.h"
#include "ChiliUtil.h"
#include "Testing.h"
#include "PerfLog.h"
#include "TestModelProbe.h"
#include "Camera.h"
namespace dx = DirectX;

App::App()
	:
	wnd(1366, 768, "Render Window"),
	light(wnd.Gfx(), 0.15f)
{
	cameras.AddCamera(std::make_unique<Camera>(wnd.Gfx(), "A", Vector3{-10, 6, 3.5f}, Vector3{0,0,0}));
	cameras.AddCamera(std::make_unique<Camera>(wnd.Gfx(), "B", Vector3{ 10, 4, 3.5f }, Vector3{ 0,0,0 }));
	cube.SetPos({ 4.0f,0.0f,0.0f });
	
	cube.LinkTechniques(rg);
	light.LinkTechniques(rg);
	sponza.LinkTechniques(rg);
	fridge.LinkTechniques(rg);
	AK47.LinkTechniques(rg);
	cameras.LinkTechniques(rg);
	//wnd.Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 768.0f / 1366.0f, 0.5f, GameCoordinates::MetersToCentimeters(200)));
}
void App::HandleInput(float deltaTime)
{
	while (const auto e = wnd.keyboard.ReadKey())
	{
		if (!e.IsPress())
		{
			continue;
		}
		switch (e.GetCode())
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
}
void App::DoFrame(float deltaTime)
{
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	//wnd.Gfx().SetCamera(cameras.GetCamera().GetMatrix());
	cameras->BindToGraphics(wnd.Gfx());
	light.Bind(wnd.Gfx(), cameras->GetMatrix());
	light.Submit();
	cube.Submit();
	//cube2.Submit(fc);
	sponza.Submit();
	fridge.Submit();
	AK47.Submit();
	cameras.Submit();
	rg.Execute(wnd.Gfx());
	static MP modelProbe{"Sponza"};
	modelProbe.SpawnWindow(sponza);
	static MP fridgeWindow{"Fridge"};
	fridgeWindow.SpawnWindow(fridge);
	static MP Ak47Window{"AK47"};
	Ak47Window.SpawnWindow(AK47);
	rg.RenderWidgets(wnd.Gfx());
	light.SpawnControlWindow();
	cube.SpawnControlWindow(wnd.Gfx(), "Cube 1");
	cameras->Translate(movementVecor, deltaTime);
	cameras->Rotate(rotationDelta.x, rotationDelta.y, deltaTime);
	cameras.SpawnWindow(wnd.Gfx());
	// present

	//wnd.DisableCursor();
	
	if (ImGui::Begin("Performance window"))
	{
		ImGui::Text("FPS %d", (int)(1.0f/deltaTime));
		ImGui::End();
	}
	wnd.Gfx().EndFrame();
	rg.Reset();
}




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
		const auto dt = timer.Mark() * speed_factor;
		HandleInput(dt);
		DoFrame(dt);
	}
}