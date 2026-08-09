#include "SolidSphere.h"
#include "BindableCommon.h"
#include "GraphicsThrowMacros.h"
#include "Sphere.h"
#include "Vertex.h"

SolidSphere::SolidSphere(Graphics& gfx, float radius)
{
	using namespace Bind;
	namespace dx = DirectX;
	auto model = Sphere::Make();
	model.Transform(dx::XMMatrixScaling(radius, radius, radius));
	const auto geometryTag = "$sphere." + std::to_string(radius);
	addBind(VertexBuffer::Resolve(gfx, geometryTag,model.vertices));
	addBind(IndexBuffer::Resolve(gfx,geometryTag,model.indices));

	auto pvs = VertexShader::Resolve(gfx, "SolidVS.cso");
	auto pvsbc = pvs->GetBytecode();
	addBind(std::move(pvs));

	addBind(PixelShader::Resolve(gfx, "SolidPS.cso"));

	struct PSColorConstant
	{
		dx::XMFLOAT3 color = { 1.0f,1.0f,1.0f };
		float padding;
	} colorConst;
	addBind(PixelConstantBuffer<PSColorConstant>::Resolve(gfx, colorConst,1u));

	addBind(InputLayout::Resolve(gfx, model.vertices.GetLayout(), pvsbc));
	addBind(Topology::Resolve(gfx,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	addBind(std::make_shared<TransformCbuf>(gfx, *this));
}

void SolidSphere::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	this->pos = pos;
}

DirectX::XMMATRIX SolidSphere::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}