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

			m_MatWorldViewProjVariablePtr = m_EffectPtr->GetVariableByName("gWorldViewProj")->AsMatrix();
			if (!m_MatWorldViewProjVariablePtr->IsValid())
				std::wcout << L"m_MatWorldViewProjVariablePtr not valid!\n";

			m_DiffuseMapVariablePtr = m_EffectPtr->GetVariableByName("gDiffuseMap")->AsShaderResource();
			if (!m_DiffuseMapVariablePtr->IsValid())
				std::wcout << L"m_DiffuseMapVariablePtr not valid!\n";
		}	
	}
	EffectBase::~EffectBase()
	{
		if (m_TechniquePtr)		m_TechniquePtr->Release();
		if (m_EffectPtr)		m_EffectPtr->Release();
	}

	ID3DX11Effect* EffectBase::GetEffectPtr() const
	{
		return m_EffectPtr;
	}
	ID3DX11EffectTechnique* EffectBase::GetTechniquePtr() const
	{
		return m_TechniquePtr;
	}
	void EffectBase::SetMatrix(const Matrix& projectionMatrix)
	{
		m_MatWorldViewProjVariablePtr->SetMatrix(reinterpret_cast<const float*>(&projectionMatrix));
	}

	void EffectBase::SetDiffuseMap(Texture* pDiffuseTexture)
	{
		if (m_DiffuseMapVariablePtr)
			m_DiffuseMapVariablePtr->SetResource(pDiffuseTexture->GetSRV());
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