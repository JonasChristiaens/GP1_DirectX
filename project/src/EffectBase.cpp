#include "pch.h"
#include "EffectBase.h"
#include "Texture.h"

namespace dae
{
	EffectBase::EffectBase(ID3D11Device* pDevice, const std::wstring& assetFile)
	{
		//load effect
		m_EffectPtr = LoadEffect(pDevice, assetFile);

		if (m_EffectPtr)
		{
			m_TechniquePtr = m_EffectPtr->GetTechniqueByName("DefaultTechnique");
			if (!m_TechniquePtr->IsValid())
				std::wcout << L"m_TechniquePtr not valid!\n";

			// Matrices
			m_MatWorldViewProjVariablePtr = m_EffectPtr->GetVariableByName("gWorldViewProj")->AsMatrix();
			if (!m_MatWorldViewProjVariablePtr->IsValid())
				std::wcout << L"m_MatWorldViewProjVariablePtr not valid!\n";

			m_MatWorldVariablePtr = m_EffectPtr->GetVariableByName("gWorldMatrix")->AsMatrix();
			if (!m_MatWorldVariablePtr->IsValid())
				std::wcout << L"m_MatWorldVariablePtr not valid!\n";


			// Vectors
			m_CameraPositionVariablePtr = m_EffectPtr->GetVariableByName("gCameraPosition")->AsVector();
			if (!m_CameraPositionVariablePtr->IsValid())
				std::wcout << L"m_CameraPositionVariablePtr not valid!\n";


			// Textures
			m_DiffuseMapVariablePtr = m_EffectPtr->GetVariableByName("gDiffuseMap")->AsShaderResource();
			if (!m_DiffuseMapVariablePtr->IsValid())
				std::wcout << L"m_DiffuseMapVariablePtr not valid!\n";

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
	EffectBase::~EffectBase()
	{
		if (m_TechniquePtr)					m_TechniquePtr->Release();

		if (m_MatWorldViewProjVariablePtr)	m_MatWorldViewProjVariablePtr->Release();
		if (m_MatWorldVariablePtr)			m_MatWorldVariablePtr->Release();

		if (m_CameraPositionVariablePtr)	m_CameraPositionVariablePtr->Release();

		if (m_DiffuseMapVariablePtr)		m_DiffuseMapVariablePtr->Release();
		if (m_NormalMapVariablePtr)			m_NormalMapVariablePtr->Release();
		if (m_SpecularMapVariablePtr)		m_SpecularMapVariablePtr->Release();
		if (m_GlossinessMapVariablePtr)		m_GlossinessMapVariablePtr->Release();

		if (m_EffectPtr)					m_EffectPtr->Release();
	}

	ID3DX11Effect* EffectBase::GetEffectPtr() const
	{
		return m_EffectPtr;
	}
	ID3DX11EffectTechnique* EffectBase::GetTechniquePtr() const
	{
		return m_TechniquePtr;
	}

	void EffectBase::SetProjectionMatrix(const Matrix& projectionMatrix)
	{
		m_MatWorldViewProjVariablePtr->SetMatrix(reinterpret_cast<const float*>(&projectionMatrix));
	}
	void EffectBase::SetWorldMatrix(const Matrix& worldMatrix)
	{
		m_MatWorldVariablePtr->SetMatrix(reinterpret_cast<const float*>(&worldMatrix));
	}
	void EffectBase::SetCameraPosition(const Vector3& origin)
	{
		m_CameraPositionVariablePtr->SetFloatVector(reinterpret_cast<const float*>(&origin));
	}
	void EffectBase::SetDiffuseMap(const Texture* pDiffuseTexture)
	{
		if (m_DiffuseMapVariablePtr)
			m_DiffuseMapVariablePtr->SetResource(pDiffuseTexture->GetSRV());
	}
	void EffectBase::SetNormalMap(const Texture* pNormalMapTexture)
	{
		if (m_NormalMapVariablePtr)
			m_NormalMapVariablePtr->SetResource(pNormalMapTexture->GetSRV());
	}
	void EffectBase::SetSpecularMap(const Texture* pSpecularMapTexture)
	{
		if (m_SpecularMapVariablePtr)
			m_SpecularMapVariablePtr->SetResource(pSpecularMapTexture->GetSRV());
	}
	void EffectBase::SetGlossinessMap(const Texture* pGlossinessTexture)
	{
		if (m_GlossinessMapVariablePtr)
			m_GlossinessMapVariablePtr->SetResource(pGlossinessTexture->GetSRV());
	}

	ID3DX11Effect* EffectBase::LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile)
	{
		HRESULT result;
		ID3D10Blob* pErrorBlob{ nullptr };
		ID3DX11Effect* pEffect{ nullptr };

		DWORD shaderFlags{ 0 };
#if defined(DEBUG) || defined(_DEBUG)
		shaderFlags |= D3DCOMPILE_DEBUG;
		shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;

#endif

		result = D3DX11CompileEffectFromFile(assetFile.c_str(),
			nullptr,
			nullptr,
			shaderFlags,
			0,
			pDevice,
			&pEffect,
			&pErrorBlob);


		if (FAILED(result))
		{
			if (pErrorBlob != nullptr)
			{
				const char* pErrors = static_cast<char*>(pErrorBlob->GetBufferPointer());

				std::wstringstream ss;
				for (unsigned int i = 0; i < pErrorBlob->GetBufferSize(); i++)
					ss << pErrors[i];

				pErrorBlob->Release();
				pErrorBlob = nullptr;

				std::wcout << ss.str() << std::endl;
			}
			else
			{
				std::wstringstream ss;
				ss << "EffectLoader failed to create affect from file!\nPath: " << assetFile;
				std::wcout << ss.str() << std::endl;
				return nullptr;
			}
		}

		return pEffect;
	}
}