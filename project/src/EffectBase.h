#pragma once

namespace dae
{
	class EffectBase
	{
	public:
		EffectBase(ID3D11Device* pDevice, const std::wstring& assetFile);
		~EffectBase();

		EffectBase(const EffectBase& other)						= delete;
		EffectBase(EffectBase&& other) noexcept					= delete;
		EffectBase& operator =(const EffectBase& other)			= delete;
		EffectBase& operator =(EffectBase&& other) noexcept		= delete;

		ID3DX11Effect* GetEffectPtr();
		ID3DX11EffectTechnique* GetTechniquePtr();

		static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);
	private:

		ID3DX11Effect*				m_EffectPtr{ nullptr };
		ID3DX11EffectTechnique*		m_TechniquePtr{ nullptr };
	};
}