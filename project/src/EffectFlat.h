#pragma once
#include "EffectBase.h"

namespace dae
{
	class EffectFlat : public EffectBase
	{
	public:
		EffectFlat(ID3D11Device* pDevice, const std::wstring& assetFile);
		~EffectFlat() override = default;
	};
}