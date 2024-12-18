#pragma once

struct ID3D11Device;
struct ID3DX11Effect;
struct ID3DX11EffectTechnique;

namespace dae
{
	class EffectBase
	{
	public:
		EffectBase(ID3D11Device* pDevice, const std::wstring& assetFile);
		~EffectBase();

		const ID3DX11Effect* GetEffectPtr();
		const ID3DX11EffectTechnique* GetTechniquePtr();

	private:
		static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);

		ID3DX11Effect* m_pEffect{};
		ID3DX11EffectTechnique* m_pTechnique{};
	};
}