#pragma once
#include "EffectBase.h"

namespace dae
{
	class EffectCombined : public EffectBase
	{
	public:
		EffectCombined(ID3D11Device* pDevice, const std::wstring& assetFile);
		~EffectCombined() override;

		void SetCameraPosition(const Vector3& origin);
		void SetNormalMap(const Texture* pNormalMapTexture);
		void SetSpecularMap(const Texture* pSpecularMapTexture);
		void SetGlossinessMap(const Texture* pGlossinessTexture);

	private:
		ID3DX11EffectVectorVariable*			m_CameraPositionVariablePtr{ nullptr };
		ID3DX11EffectShaderResourceVariable*	m_NormalMapVariablePtr{ nullptr };
		ID3DX11EffectShaderResourceVariable*	m_SpecularMapVariablePtr{ nullptr };
		ID3DX11EffectShaderResourceVariable*	m_GlossinessMapVariablePtr{ nullptr };
	};
}