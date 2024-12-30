#pragma once
#include "Graphics.h"
#include "SolidSphere.h"
#include "ConstantBuffers.h"

class PointLight
{
public:
	PointLight(Graphics& gfx, float radius = 0.5f);
	void SpawnControlWindow() noexcept;
	void Reset() noexcept;
	void Draw(Graphics& gfx) const noexcept(!IS_DEBUG);
	void Bind(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept;
private:
	struct PointLightCBuf
	{
		DirectX::XMFLOAT3 pos;
		float padding;
		DirectX::XMFLOAT4 lightClor;
		DirectX::XMFLOAT4 ambient;
		DirectX::XMFLOAT4 diffuseLight;
		float diffuseIntensity;
		float attConstant;
		float attLin;
		float attQuad;
	};
private:
	PointLightCBuf cbData;
	mutable SolidSphere mesh;
	mutable PixelConstantBuffer<PointLightCBuf> cbuf;
};