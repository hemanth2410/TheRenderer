#pragma once
#include "Bindable.h"

namespace Bind
{
	class Sampler : public Bindable
	{
	public:
		Sampler(Graphics& gfx, bool anisoEnabled, bool reflect);
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<Sampler> Resolve(Graphics& gfx, bool anisoEnabled = true, bool reflect = true);
		static std::string GenerateUID(bool anisoEnabled, bool reflect);
		std::string GetUID() const noexcept override;
	protected:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
		bool anisoEnabled;
		bool reflect;
	};
}