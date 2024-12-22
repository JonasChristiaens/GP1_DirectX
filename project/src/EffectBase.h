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

		void SetProjectionMatrix(const Matrix& projectionMatrix);
		void SetWorldMatrix(const Matrix& worldMatrix);
		void SetCameraPosition(const Vector3& origin);

		void SetDiffuseMap(const Texture* pDiffuseTexture);
		void SetNormalMap(const Texture* pNormalMapTexture);
		void SetSpecularMap(const Texture* pSpecularMapTexture);
		void SetGlossinessMap(const Texture* pGlossinessTexture);

		static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);
	private:

		ID3DX11Effect*							m_EffectPtr{ nullptr };
		ID3DX11EffectTechnique*					m_TechniquePtr{ nullptr };
		ID3DX11EffectMatrixVariable*			m_MatWorldViewProjVariablePtr{ nullptr };
		ID3DX11EffectMatrixVariable*			m_MatWorldVariablePtr{ nullptr };
		ID3DX11EffectVectorVariable*			m_CameraPositionVariablePtr{ nullptr };

		ID3DX11EffectShaderResourceVariable*	m_DiffuseMapVariablePtr{ nullptr };
		ID3DX11EffectShaderResourceVariable*	m_NormalMapVariablePtr{ nullptr };
		ID3DX11EffectShaderResourceVariable*	m_SpecularMapVariablePtr{ nullptr };
		ID3DX11EffectShaderResourceVariable*	m_GlossinessMapVariablePtr{ nullptr };
	};
}