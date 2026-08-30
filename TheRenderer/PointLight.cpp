#include "PointLight.h"
#include "imgui/imgui.h"
#include "GameCoordinates.h"
#include "Camera.h"

PointLight::PointLight(Graphics& gfx, float radius)
	:
	mesh(gfx, radius),
	cbuf(gfx)
{
	Reset();
	Vector3 position = transform.position;
	Vector3 rotation = { 0,0,0 };
	pCamera = std::make_shared<Camera>(gfx, "Light", position, rotation, true);
}

void PointLight::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Light"))
	{
		bool dirtyPos = false;
		const auto d = [&dirtyPos](bool dirty) {dirtyPos = dirtyPos || dirty; };
		ImGui::Text("Position");
		d(ImGui::SliderFloat("X", &transform.position.x, -20.0f, 20.0f, "%.1f"));
		d(ImGui::SliderFloat("Y", &transform.position.y, -20.0f, 20.0f, "%.1f"));
		d(ImGui::SliderFloat("Z", &transform.position.z, -20.0f, 20.0f, "%.1f"));
		if (dirtyPos)
		{
			pCamera->SetPosition(transform.position);
		}
		ImGui::Text("Intensity/Color");
		ImGui::SliderFloat("Intensity", &cbData.diffuseIntensity, 0.01f, 5.0f, "%.2f", 2);
		ImGui::ColorEdit3("Diffuse Color", &cbData.diffuseColor.x);
		ImGui::ColorEdit3("Ambient", &cbData.ambient.x);

		ImGui::Text("Falloff");
		ImGui::SliderFloat("Constant", &cbData.attConst, 0.05f, 10.0f, "%.2f", 4);
		ImGui::SliderFloat("Linear", &cbData.attLin, 0.0001f, 4.0f, "%.4f", 8);
		ImGui::SliderFloat("Quadratic", &cbData.attQuad, 0.0000001f, 10.0f, "%.7f", 10);

		if (ImGui::Button("Reset"))
		{
			Reset();
		}
		// building cBufData
		cbData.pos.x = transform.position.x;
		cbData.pos.y = transform.position.y;
		cbData.pos.z = transform.position.z;
	}
	ImGui::End();
}

void PointLight::Reset() noexcept
{
	cbData = {
		{ 0.0f,0.0f,0.0f },
		{ 0.05f,0.05f,0.05f },
		{ 1.0f,1.0f,1.0f },
		1.0f,
		1.0f,
		0.045f,
		0.0075f,
	};
}

void PointLight::Submit(size_t channelFilter) const noxnd
{
	mesh.SetPos(cbData.pos);
	mesh.Submit(channelFilter);
}

void PointLight::Bind(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept
{
	auto dataCopy = cbData;
	const auto pos = DirectX::XMLoadFloat3(&cbData.pos);
	DirectX::XMStoreFloat3(&dataCopy.pos, DirectX::XMVector3Transform(pos, view));
	cbuf.Update(gfx, dataCopy);
	cbuf.Bind(gfx);
}
void PointLight::LinkTechniques(Rgph::RenderGraph& rg)
{
	mesh.LinkTechniques(rg);
}


std::shared_ptr<Camera> PointLight::ShareCamera() const noexcept
{
	return pCamera;
}