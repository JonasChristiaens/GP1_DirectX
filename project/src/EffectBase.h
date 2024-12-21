#pragma once

namespace dae
{
	class Texture;
	class EffectBase
	{
	public:
		EffectBase(ID3D11Device* pDevice, const std::wstring& assetFile);
		~EffectBase();

		EffectBase(const EffectBase& other)						= delete;
		EffectBase(EffectBase&& other) noexcept					= delete;
		EffectBase& operator =(const EffectBase& other)			= delete;
		EffectBase& operator =(EffectBase&& other) noexcept		= delete;

		ID3DX11Effect* GetEffectPtr() const;
		ID3DX11EffectTechnique* GetTechniquePtr() const;

		void SetMatrix(const Matrix& projectionMatrix);
		void SetDiffuseMap(Texture* pDiffuseTexture);

		static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);
	private:

		ID3DX11Effect*							m_EffectPtr{ nullptr };
		ID3DX11EffectTechnique*					m_TechniquePtr{ nullptr };
		ID3DX11EffectMatrixVariable*			m_MatWorldViewProjVariablePtr{ nullptr };
		ID3DX11EffectShaderResourceVariable*	m_DiffuseMapVariablePtr{ nullptr };
	};
}