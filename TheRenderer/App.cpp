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
#include "Channels.h"
namespace dx = DirectX;

App::App()
	:
	wnd(1366, 768, "Render Window"),
	light(wnd.Gfx(), 0.15f)
{
	cameras.AddCamera(std::make_unique<Camera>(wnd.Gfx(), "A", Vector3{-10, 6, 3.5f}, Vector3{0,0,0}));
	cameras.AddCamera(std::make_unique<Camera>(wnd.Gfx(), "B", Vector3{ 10, 4, 3.5f }, Vector3{ 0,0,0 }));
	cameras.AddCamera(light.ShareCamera());
	cube.SetPos({ 0, 1, 2 });
	cube2.SetPos({ 0, 1, 4 });
	cube.LinkTechniques(rg);
	cube2.LinkTechniques(rg);
	light.LinkTechniques(rg);
	sponza.LinkTechniques(rg);
	fridge.LinkTechniques(rg);
	AK47.LinkTechniques(rg);
	cameras.LinkTechniques(rg);
	rg.BindShadowCamera(*light.ShareCamera());
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
		case VK_RETURN:
			savingDepth = true;
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
	//cameras->BindToGraphics(wnd.Gfx());
	light.Bind(wnd.Gfx(), cameras->GetMatrix());
	rg.BindMainCamera(cameras.GetActiveCamera());
	

	light.Submit(Chan::main);
	cube.Submit(Chan::main);
	cube2.Submit(Chan::main);
	sponza.Submit(Chan::main);
	fridge.Submit(Chan::main);
	AK47.Submit(Chan::main);
	cameras.Submit(Chan::main);

	cube.Submit(Chan::shadow);
	cube2.Submit(Chan::shadow);
	sponza.Submit(Chan::shadow);
	fridge.Submit(Chan::shadow);
	AK47.Submit(Chan::shadow);

	rg.Execute(wnd.Gfx());
	static MP modelProbe{"Sponza"};
	modelProbe.SpawnWindow(sponza);
	static MP fridgeWindow{"Fridge"};
	fridgeWindow.SpawnWindow(fridge);
	static MP Ak47Window{"AK47"};
	Ak47Window.SpawnWindow(AK47);
	rg.RenderWindows(wnd.Gfx());
	light.SpawnControlWindow();
	cube.SpawnControlWindow(wnd.Gfx(), "Cube 1");
	cube2.SpawnControlWindow(wnd.Gfx(), "Cube 2");
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
	if (savingDepth)
	{
		rg.DumpShadowMap(wnd.Gfx(), "shadow_Debug.png");
		savingDepth = false;
	}
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