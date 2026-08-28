#pragma once
#include <DirectXMath.h>
#include <string>
#include "Projection.h"
#include "Transform.h"
#include "CameraIndicator.h"

class Graphics;
namespace Rgph
{
	class RenderGraph;
}
class Camera
{
public:
	Camera(Graphics& gfx, std::string name, Vector3 homePos = { 0,0,0 }, Vector3 EulerRotation = { 0,0,0 }) noexcept;
	void BindToGraphics(Graphics& gfx) const;
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void SpawnControlWidgets(Graphics& gfx) noexcept;
	void Reset(Graphics& gfx) noexcept;
	void Rotate(float dx, float dy, float deltaTime) noexcept;
	void Translate(Vector3 translation, float deltaTime) noexcept;
	void SetPosition(Vector3 position) noexcept;
	DirectX::XMFLOAT3 GetPos() noexcept;
	const std::string& GetName() const noexcept;
	void LinkTechniques(Rgph::RenderGraph& rg);
	void Submit() const;
private:
	std::string name;
	Transform transform;
	Vector3 homePos;
	Vector3 homeRotation;
	static constexpr float travelSpeed = 5.0f;
	static constexpr float rotationSpeed_deg = 150.0f;
	float yaw;
	float pitch;
	float roll;
	float rotationSpeed_Rad;
	Projection proj;
	CameraIndicator indicator;
	bool enableCameraGizmo = true;
	bool enableProjectionGizmo = true;
};