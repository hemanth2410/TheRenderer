#include "Sampler.h"
#include "GraphicsThrowMacros.h"
#include "BindableCodex.h"

namespace Bind
{
	Sampler::Sampler(Graphics& gfx, bool anisoEnabled, bool reflect)
		:
		anisoEnabled(anisoEnabled),
		reflect(reflect)
	{
		INFOMAN(gfx);

		D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC{ CD3D11_DEFAULT{} };
		samplerDesc.Filter = anisoEnabled ? D3D11_FILTER_ANISOTROPIC : D3D11_FILTER_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = reflect ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = reflect ? D3D11_TEXTURE_ADDRESS_MIRROR : D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
		GFX_THROW_INFO(GetDevice(gfx)->CreateSamplerState(&samplerDesc, &pSampler));
	}

	void Sampler::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->PSSetSamplers(0, 1, pSampler.GetAddressOf());
	}
	std::shared_ptr<Sampler> Sampler::Resolve(Graphics& gfx, bool anisoEnabled, bool reflect)
	{
		return Codex::Resolve<Sampler>(gfx,anisoEnabled, reflect);
	}
	std::string Sampler::GenerateUID(bool anisoEnabled, bool reflect)
	{
		using namespace std::string_literals;
		return typeid(Sampler).name() + "#"s + (anisoEnabled ? "A"s : "a"s) + (reflect ? "R"s : "W"s);
	}
	std::string Sampler::GetUID() const noexcept
	{
		return GenerateUID(anisoEnabled, reflect);
	}
}