#include "Camera.h"
#include "imgui/imgui.h"
#include "GameCoordinates.h"
#include "GeometryMath.h"
#include "Graphics.h"
namespace dx = DirectX;

Camera::Camera(Graphics& gfx, std::string name, Vector3 homePos, Vector3 homeRotation) noexcept
	:
	name(name),
	homePos(homePos),
	homeRotation(homeRotation),
	proj(gfx, 1.0f, 768.0f / 1366.0f, 0.5f, 200),
	indicator(gfx)
{
	Reset(gfx);
	rotationSpeed_Rad = DirectX::XMConvertToRadians(rotationSpeed_deg);
	indicator.SetPos(transform.position.ToXmFloat3());
	indicator.SetRotation(transform.rotation.ToEuler().ToXmFloat3());
}

void Camera::BindToGraphics(Graphics& gfx) const
{
	gfx.SetCamera(GetMatrix());
	gfx.SetProjection(proj.GetMatrix());
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	return dx::XMMatrixTranslation(-transform.position.x, -transform.position.y, -transform.position.z) *
		dx::XMMatrixRotationQuaternion(DirectX::XMQuaternionConjugate(transform.rotation.ToXMVector()));
}

void Camera::SpawnControlWidgets(Graphics& gfx) noexcept
{
	Vector3 eulerRadians = transform.rotation.ToEuler();
	Vector3 eulerDegrees = Vector3(
		DirectX::XMConvertToDegrees(eulerRadians.x),
		DirectX::XMConvertToDegrees(eulerRadians.y),
		DirectX::XMConvertToDegrees(eulerRadians.z)
		);
	bool rotationChanged = false;
	bool posDirty = false;
	const auto dcheck = [](bool d, bool& carry) {carry = carry || d; };
	ImGui::Text("Position");

	ImGui::PushItemWidth(150.0f);
	ImGui::SameLine();
	dcheck(ImGui::InputFloat("X", &transform.position.x), posDirty);
	ImGui::SameLine();
	dcheck(ImGui::InputFloat("Y", &transform.position.y), posDirty);
	ImGui::SameLine();
	dcheck(ImGui::InputFloat("Z", &transform.position.z), posDirty);
	ImGui::PopItemWidth();

	ImGui::Text("Rotation");
	ImGui::PushItemWidth(150.0f);
	ImGui::SameLine();
	rotationChanged |= ImGui::InputFloat("X", &eulerDegrees.x);
	ImGui::SameLine();
	rotationChanged |= ImGui::InputFloat("Y", &eulerDegrees.y);
	ImGui::SameLine();
	rotationChanged |= ImGui::InputFloat("Z", &eulerDegrees.z);
	ImGui::PopItemWidth();
	if (ImGui::Button("Reset"))
	{
		Reset(gfx);
	}
	ImGui::Checkbox("Camera Gizmo", &enableCameraGizmo);
	ImGui::Checkbox("Projection Gizmo", &enableProjectionGizmo);
	proj.RenderWidgets(gfx);
	//ImGui::End();
	// write the values back to original transfrom
	if (rotationChanged)
	{
		transform.SetRotationEuler(
			DirectX::XMConvertToRadians(eulerDegrees.x),
			DirectX::XMConvertToRadians(eulerDegrees.y),
			DirectX::XMConvertToRadians(eulerDegrees.z)
		);
		indicator.SetRotation(transform.rotation.ToEuler().ToXmFloat3());
		proj.SetRotation(transform.rotation.ToEuler().ToXmFloat3());
	}
	if (posDirty)
	{
		indicator.SetPos(transform.position.ToXmFloat3());
		proj.SetPos(transform.position.ToXmFloat3());
	}
}

void Camera::Reset(Graphics& gfx) noexcept
{
	transform.position = homePos;
	transform.rotation = Quaternion().FromEuler(to_rad(homeRotation.x), to_rad(homeRotation.y), to_rad(homeRotation.z)); // somehow every time a rotation is set we need to compute local vectors, its default now so local vectors are same as global vectors
	indicator.SetRotation(transform.rotation.ToEuler().ToXmFloat3());
	indicator.SetPos(transform.position.ToXmFloat3());
	proj.SetRotation(transform.rotation.ToEuler().ToXmFloat3());
	proj.SetPos(transform.position.ToXmFloat3());
	proj.Reset(gfx);
}

void Camera::Rotate(float dx, float dy, float deltaTime) noexcept
{
	// converting rotation to degrees
	yaw = DirectX::XMConvertToDegrees(transform.rotation.ToEuler().y);
	pitch = DirectX::XMConvertToDegrees(transform.rotation.ToEuler().x);
	roll = DirectX::XMConvertToDegrees(transform.rotation.ToEuler().z);
	yaw = wrap_angle_degrees(yaw + dx * rotationSpeed_Rad * deltaTime); // this has to be euler
	pitch = std::clamp(pitch + dy * rotationSpeed_Rad * deltaTime, -75.0f, 75.0f); // so is this 

	//write the rotation back to the transfrom
	transform.SetRotationEuler(
		DirectX::XMConvertToRadians(pitch),
		DirectX::XMConvertToRadians(yaw),
		DirectX::XMConvertToRadians(roll)
	);
	indicator.SetRotation(transform.rotation.ToEuler().ToXmFloat3());
	proj.SetRotation(transform.rotation.ToEuler().ToXmFloat3());
}

void Camera::Translate(Vector3 direction, float deltaTime) noexcept
{
	transform.position += transform.forward * (deltaTime * travelSpeed * direction.z);
	transform.position += transform.right * (deltaTime * travelSpeed * direction.x);
	transform.position += transform.up * (deltaTime * travelSpeed * direction.y);
	indicator.SetPos(transform.position.ToXmFloat3());
	proj.SetPos(transform.position.ToXmFloat3());
}
DirectX::XMFLOAT3 Camera::GetPos() noexcept
{
	return transform.position.ToXmFloat3();
}

const std::string& Camera::GetName() const noexcept
{
	return name;
}
void Camera::LinkTechniques(Rgph::RenderGraph& rg)
{
	indicator.LinkTechniques(rg);
	proj.LinkTechniques(rg);
}
void Camera::Submit() const
{
	if (enableCameraGizmo)
	{
		indicator.Submit();
	}
	if (enableProjectionGizmo)
	{
		proj.Submit();
	}
}