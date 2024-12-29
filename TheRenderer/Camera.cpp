#include "Camera.h"
#include "imgui/imgui.h"

namespace dx = DirectX;

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	//const auto pos = dx::XMVectorSet(x, y, z, 0.0f)
	return dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) * dx::XMMatrixTranslation(x, y, z);
}

void Camera::SpawnControlWindow() noexcept
{
	if (ImGui::Begin("Camera"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &x, -80.0f, 80.0f, "%.1f");
		ImGui::SliderFloat("Y", &y, -80.0f, 80.0f);
		ImGui::SliderFloat("Z", &z, -80.0f, 80.0f);
		ImGui::Text("Orientation");
		ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
		ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
		ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);
		if (ImGui::Button("Reset"))
		{
			Reset();
		}
	}
	ImGui::End();
}

void Camera::Reset() noexcept
{
	x = 0.0f;
	y = 1.0f;
	z = -10.0f;
	pitch = 0.0f;
	yaw = 0.0f;
	roll = 0.0f;
}