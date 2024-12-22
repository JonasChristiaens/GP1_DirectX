#include "pch.h"
#include "EffectFlat.h"

namespace dae
{
	EffectFlat::EffectFlat(ID3D11Device* pDevice, const std::wstring& assetFile)
		: EffectBase(pDevice, assetFile)
	{
		// The EffectBase class already handles everything required for Diffuse
	}
}