#include "pch.h"
#include "EffectCombined.h"
#include "Texture.h"

namespace dae
{
	EffectCombined::EffectCombined(ID3D11Device* pDevice, const std::wstring& assetFile)
		: EffectBase(pDevice, assetFile)
	{
		if (m_EffectPtr)
		{
			// Vectors
			m_CameraPositionVariablePtr = m_EffectPtr->GetVariableByName("gCameraPosition")->AsVector();
			if (!m_CameraPositionVariablePtr->IsValid())
				std::wcout << L"m_CameraPositionVariablePtr not valid!\n";

			// Textures
			m_NormalMapVariablePtr = m_EffectPtr->GetVariableByName("gNormalMap")->AsShaderResource();
			if (!m_NormalMapVariablePtr->IsValid())
				std::wcout << L"m_NormalMapVariablePtr not valid!\n";

			m_SpecularMapVariablePtr = m_EffectPtr->GetVariableByName("gSpecularMap")->AsShaderResource();
			if (!m_SpecularMapVariablePtr->IsValid())
				std::wcout << L"m_SpecularMapVariablePtr not valid!\n";

			m_GlossinessMapVariablePtr = m_EffectPtr->GetVariableByName("gGlossinessMap")->AsShaderResource();
			if (!m_GlossinessMapVariablePtr->IsValid())
				std::wcout << L"m_GlossinessMapVariablePtr not valid!\n";
		}
	}

	EffectCombined::~EffectCombined()
	{
		if (m_CameraPositionVariablePtr)	m_CameraPositionVariablePtr->Release();
		if (m_NormalMapVariablePtr)			m_NormalMapVariablePtr->Release();
		if (m_SpecularMapVariablePtr)		m_SpecularMapVariablePtr->Release();
		if (m_GlossinessMapVariablePtr)		m_GlossinessMapVariablePtr->Release();
	}

	void EffectCombined::SetCameraPosition(const Vector3& origin)
	{
		if (m_CameraPositionVariablePtr)
			m_CameraPositionVariablePtr->SetFloatVector(reinterpret_cast<const float*>(&origin));
	}

	void EffectCombined::SetNormalMap(const Texture* pNormalMapTexture)
	{
		if (m_NormalMapVariablePtr)
			m_NormalMapVariablePtr->SetResource(pNormalMapTexture->GetSRV());
	}

	void EffectCombined::SetSpecularMap(const Texture* pSpecularMapTexture)
	{
		if (m_SpecularMapVariablePtr)
			m_SpecularMapVariablePtr->SetResource(pSpecularMapTexture->GetSRV());
	}

	void EffectCombined::SetGlossinessMap(const Texture* pGlossinessTexture)
	{
		if (m_GlossinessMapVariablePtr)
			m_GlossinessMapVariablePtr->SetResource(pGlossinessTexture->GetSRV());
	}

}