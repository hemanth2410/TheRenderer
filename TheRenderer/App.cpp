#include "App.h"
//#include "Melon.h"
//#include "Pyramid.h"
#include <sstream>
#include "Box.h"
#include <memory>
#include <algorithm>
#include "GeometryMath.h"
//#include "Sheet.h"
#include "Surface.h"
#include "GDIPlusManager.h"
#include "Imgui/imgui.h"


GDIPlusManager gdipm;
namespace dx = DirectX;
App::App()
	:
	wnd(1366, 768, "The Donkey Fart Box"),
	pointLight(wnd.Gfx())
{
	class Factory
	{
	public:
		Factory(Graphics& gfx)
			:
			gfx(gfx)
		{}
		std::unique_ptr<Drawable> operator()()
		{
			/*switch (typedist(rng))
			{
			case 0:
				return std::make_unique<Pyramid>(
					gfx, rng, adist, ddist,
					odist, rdist
				);
			case 1:
				return std::make_unique<Box>(
					gfx, rng, adist, ddist,
					odist, rdist, bdist
				);
			case 2:
				return std::make_unique<Melon>(
					gfx, rng, adist, ddist,
					odist, rdist, longdist, latdist
				);
			case 3:
				return std::make_unique<Sheet>(
					gfx, rng, adist, ddist,
					odist, rdist
				);
			default:
				assert(false && "bad drawable type in factory");
				return {};
			}*/
			const DirectX::XMFLOAT4 mat = {cdist(rng), cdist(rng), cdist(rng),1.0f};
			float specularIntensity = sidist(rng);
			float specularPower = Spdist(rng);
			return std::make_unique<Box>(
				gfx, rng, adist, ddist,
				odist, rdist, bdist, mat, specularIntensity, specularPower
			);
		}
	private:
		Graphics& gfx;
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float> adist{ 0.0f,PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
		std::uniform_int_distribution<int> latdist{ 5,20 };
		std::uniform_int_distribution<int> longdist{ 10,40 };
		std::uniform_int_distribution<int> typedist{ 0,3 };
		std::uniform_real_distribution<float> cdist{ 0.0f,1.0f };
		std::uniform_real_distribution<float> sidist{ 0.0f,1.0f };
		std::uniform_real_distribution<float> Spdist{ 0.0f,100.0f };
	};

	Factory f(wnd.Gfx());
	drawables.reserve(nDrawables);
	std::generate_n(std::back_inserter(drawables), nDrawables, f);
	//const auto s = Surface::FromFile("Images\\Trollface.png");
	wnd.Gfx().SetProjection(dx::XMMatrixPerspectiveLH(1.0f, 768.0f/1366.0f, 0.5f, 40.0f));
	//wnd.Gfx().SetCamera(dx::XMMatrixTranslation(0.0f, 0.0f, 20.0f));
}

void App::DoFrame()
{
	const auto dt = timer.Mark() * simulationSpeed;
	
	wnd.Gfx().BeginFrame(0.0f, 0.0f, 0.0f);
	wnd.Gfx().SetCamera(cam.GetMatrix());
	pointLight.Bind(wnd.Gfx(), cam.GetMatrix());
	for (auto& d : drawables)
	{
		d->Update(wnd.keyboard.KeyIsPressed(VK_CONTROL) ? 0.0f : dt);
		d->Draw(wnd.Gfx());
	}
	pointLight.Draw(wnd.Gfx());
	static char buffer[1024];
	if (ImGui::Begin("Simulation Speed"))
	{
		ImGui::SliderFloat("Simulation Speed", &simulationSpeed, 0.0f, 6.0f,"%.4f",3.2f);
		ImGui::Text("Render engine average %.3f ms/frame (%.0fps)", 1000.0f/ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::InputText("Example text box", buffer, sizeof(buffer));
		if (ImGui::Button("Reset simulation speed"))
		{
			ResetSimulationSpeed(1.0f);
		}
		cam.SpawnControlWindow();
		pointLight.SpawnControlWindow();
	}
	ImGui::End();
	wnd.Gfx().EndFrame();
}

void App::ResetSimulationSpeed(float value)
{
	simulationSpeed = 1.0f;
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
		DoFrame();
	}
}