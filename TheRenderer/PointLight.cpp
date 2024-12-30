#include "PointLight.h"
#include "imgui/imgui.h"

PointLight::PointLight(Graphics& gfx, float radius)
	:
	mesh(gfx, radius, {1.0f,1.0f,1.0f,1.0f}),
	cbuf(gfx)
{
	Reset();
}

void PointLight::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &cbData.pos.x, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Y", &cbData.pos.y, -60.0f, 60.0f, "%.1f");
		ImGui::SliderFloat("Z", &cbData.pos.z, -60.0f, 60.0f, "%.1f");
		if (ImGui::Button("Reset Position"))
		{
			Reset();
		}
		ImGui::Text("Light Color");
		float colorArray[4] = { cbData.lightClor.x, cbData.lightClor.y, cbData.lightClor.z, cbData.lightClor.w };
		if (ImGui::ColorPicker4("Color", colorArray, 0))
		{
			// Update color from float array
			cbData.lightClor = { colorArray[0],colorArray[1],colorArray[2],colorArray[3]};
			mesh.SetColor(cbData.lightClor);
		}
	}
	ImGui::End();
}

void PointLight::Reset() noexcept
{
	cbData = {
		{0,0,0},
		{0},
		{0.7f,0.7f,0.9f,1.0f},
		{0.05f,0.05f,0.05f,1.0f},
		{1.0f,1.0f,1.0f,1.0f},
		1.0f,
		1.0f,
		0.045f,
		0.0075f,

	};
}

void PointLight::Draw(Graphics& gfx) const noexcept(!IS_DEBUG)
{
	mesh.SetPos(cbData.pos);
	mesh.Draw(gfx);
}

void PointLight::Bind(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept
{
	//cbuf.Update(gfx, cbData);
	auto dataCopy = cbData;
	const auto pos = DirectX::XMLoadFloat3(&cbData.pos);
	DirectX::XMStoreFloat3(&dataCopy.pos, DirectX::XMVector3Transform(pos, view));
	cbuf.Update(gfx, dataCopy);
	cbuf.Bind(gfx);
}