#pragma once
#include "Graphics.h"
#include "Transform.h"
class Camera
{
public:
	Camera() noexcept;
	DirectX::XMMATRIX GetMatrix() const noexcept;
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Rotate(float dx, float dy, float deltaTime) noexcept;
	void Translate(Vector3 translation, float deltaTime) noexcept;
	void SetPosition(Vector3 position) noexcept;
private:
	Transform transform;
	static constexpr float travelSpeed = 5.0f;
	static constexpr float rotationSpeed_deg = 150.0f;
	float yaw;
	float pitch;
	float roll;
	float rotationSpeed_Rad;
};