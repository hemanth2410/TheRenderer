#pragma once
#include "DrawableBase.h"
#include <memory>
#include "ConstantBuffers.h"

class SolidSphere : public DrawableBase<SolidSphere>
{
public:
	SolidSphere(Graphics& gfx, float radius, DirectX::XMFLOAT4 color);
	//void SetSphereColor(Graphics gfx, DirectX::XMFLOAT3 inColor) noexcept;
	void Update(float dt) noexcept override;
	void SetPos(DirectX::XMFLOAT3 pos) noexcept;
	void SetColor(DirectX::XMFLOAT4 color) noexcept;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	DirectX::XMFLOAT3 pos = { 1.0f,1.0f,1.0f };
	struct meshCbufData 
	{
		DirectX::XMFLOAT4 color;
	}mData;
	/*struct PSColorConstant
	{
		DirectX::XMFLOAT3 _color = { 1.0f,1.0f,1.0f };
		float padding;
	};

	struct LightMeshCbuf
	{
		DirectX::XMFLOAT4 lightClor = {1.0f,1.0f,1.0f,1.0f};
	};
	LightMeshCbuf meshCbuf;
	std::unique_ptr<PixelConstantBuffer<LightMeshCbuf>> cbuf;
	PSColorConstant colorConst;*/
	//std::unique_ptr<PixelConstantBuffer<PSColorConstant>> pColorCbuf;
	//mutable PixelConstantBuffer<meshCbufData> mcBuf;
	Graphics& gfx;
};