#include "Transform.h"
#include "imgui/imgui.h"
void TransformWindow::SpawnTransformWindow(Transform& transform, const char* transformName) noexcept
{
	if (ImGui::Begin("Transform"))
	{
		ImGui::Text(transformName);

		ImGui::Text("Position");
		ImGui::SameLine();
		ImGui::PushItemWidth(100);
		ImGui::InputFloat("X##Pos", &transform.position.x);
		ImGui::SameLine();
		ImGui::InputFloat("Y##Pos", &transform.position.y);
		ImGui::SameLine();
		ImGui::InputFloat("Z##Pos", &transform.position.z);
		ImGui::PopItemWidth();

		ImGui::Text("Rotation (deg)");
		// ToEuler() returns radians -- convert to degrees for display/editing,
		// since typing "9" into a field should mean 9 degrees, not 9 radians
		// (9 radians is over 500 degrees, which is why rotations looked huge).
		const Vector3 eulerRadians = transform.rotation.ToEuler();
		Vector3 eulerDegrees(
			DirectX::XMConvertToDegrees(eulerRadians.x),
			DirectX::XMConvertToDegrees(eulerRadians.y),
			DirectX::XMConvertToDegrees(eulerRadians.z)
		);
		ImGui::SameLine();
		ImGui::PushItemWidth(100);
		bool rotationChanged = false;
		rotationChanged |= ImGui::InputFloat("X##Rot", &eulerDegrees.x);
		ImGui::SameLine();
		rotationChanged |= ImGui::InputFloat("Y##Rot", &eulerDegrees.y);
		ImGui::SameLine();
		rotationChanged |= ImGui::InputFloat("Z##Rot", &eulerDegrees.z);
		ImGui::PopItemWidth();
		if (rotationChanged)
		{
			transform.SetRotationEuler(
				DirectX::XMConvertToRadians(eulerDegrees.x),
				DirectX::XMConvertToRadians(eulerDegrees.y),
				DirectX::XMConvertToRadians(eulerDegrees.z)
			);
		}

		ImGui::Text("Scale");
		ImGui::SameLine();
		ImGui::PushItemWidth(100);
		ImGui::InputFloat("X##Scale", &transform.scale.x);
		ImGui::SameLine();
		ImGui::InputFloat("Y##Scale", &transform.scale.y);
		ImGui::SameLine();
		ImGui::InputFloat("Z##Scale", &transform.scale.z);
		ImGui::PopItemWidth();
	}
	ImGui::End();
}