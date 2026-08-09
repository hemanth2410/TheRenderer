#include "Camera.h"
#include "imgui/imgui.h"
#include "GameCoordinates.h"
#include "GeometryMath.h"
namespace dx = DirectX;

Camera::Camera() noexcept
{
	Reset();
	rotationSpeed_Rad = DirectX::XMConvertToRadians(rotationSpeed_deg);
}

DirectX::XMMATRIX Camera::GetMatrix() const noexcept
{
	/*return dx::XMMatrixTranslation(-transform.position.x, -transform.position.y, -transform.position.z) *
		dx::XMMatrixRotationQuaternion(transform.rotation.ToXMVector());*/
	/*XMVECTOR forwardVector = Vector3::Forward().ToXmVector();
	const auto lookVector = XMVector3Transform(forwardVector,
		XMMatrixRotationQuaternion(transform.rotation.ToXMVector()));
	Vector3 camPosition = transform.position;
	Vector3 lookAt = camPosition + Vector3::FromXmVector(lookVector);
	return XMMatrixLookAtLH(camPosition.ToXmVector(), lookAt.ToXmVector(), XMVectorSet(0, 1, 0, 0));*/
	return dx::XMMatrixTranslation(-transform.position.x, -transform.position.y, -transform.position.z) *
		dx::XMMatrixRotationQuaternion(DirectX::XMQuaternionConjugate(transform.rotation.ToXMVector()));
}

void Camera::SpawnControlWindow() noexcept
{
	Vector3 eulerRadians = transform.rotation.ToEuler();
	Vector3 eulerDegrees = Vector3(
		DirectX::XMConvertToDegrees(eulerRadians.x),
		DirectX::XMConvertToDegrees(eulerRadians.y),
		DirectX::XMConvertToDegrees(eulerRadians.z)
		);
	bool rotationChanged = false;
	if (ImGui::Begin("Camera"))
	{
		
		ImGui::Text("Position");
		
		ImGui::PushItemWidth(150.0f);
		ImGui::SameLine();
		ImGui::InputFloat("X", &transform.position.x);
		ImGui::SameLine();
		ImGui::InputFloat("Y", &transform.position.y);
		ImGui::SameLine();
		ImGui::InputFloat("Z", &transform.position.z);
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
			Reset();
		}
	}
	ImGui::End();
	// write the values back to original transfrom
	if (rotationChanged)
	{
		transform.SetRotationEuler(
			DirectX::XMConvertToRadians(eulerDegrees.x),
			DirectX::XMConvertToRadians(eulerDegrees.y),
			DirectX::XMConvertToRadians(eulerDegrees.z)
		);
	}
}

void Camera::Reset() noexcept
{
	transform.position = Vector3(0,1,-10);
	transform.rotation = Quaternion(); // somehow every time a rotation is set we need to compute local vectors, its default now so local vectors are same as global vectors
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
}

void Camera::Translate(Vector3 direction, float deltaTime) noexcept
{
	transform.position += transform.forward * (deltaTime * travelSpeed * direction.z);
	transform.position += transform.right * (deltaTime * travelSpeed * direction.x);
	transform.position += transform.up * (deltaTime * travelSpeed * direction.y);
}