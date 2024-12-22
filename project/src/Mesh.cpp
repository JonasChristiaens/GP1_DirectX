#include "pch.h"
#include "Mesh.h"
#include "EffectCombined.h"
#include "EffectFlat.h"
#include "Texture.h"

#include <cassert>

namespace dae
{
	Mesh::Mesh(ID3D11Device* pDevice, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, bool isFlatEffect)
		: m_DevicePtr{ pDevice }
		, m_Vertices{ vertices }
		, m_Indices{ indices }
		, m_IsFlatEffect{ isFlatEffect }

	{
		if (m_IsFlatEffect)
		{
			m_EffectFlatPtr = new EffectFlat(m_DevicePtr, L"./resources/Fire.fx");
			m_TechniquePtr = m_EffectFlatPtr->GetTechniquePtr();
		}
		else
		{
			m_EffectCombinedPtr = new EffectCombined(m_DevicePtr, L"./resources/PosCol3D.fx");
			m_TechniquePtr = m_EffectCombinedPtr->GetTechniquePtr();
		}

		// ===================
		// Create Vertex layout
		// ====================

		static constexpr uint32_t numElements{ 5 };
		D3D11_INPUT_ELEMENT_DESC vertexDesc[numElements]{};
		
		vertexDesc[0].SemanticName = "POSITION";
		vertexDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		vertexDesc[0].AlignedByteOffset = 0;
		vertexDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		vertexDesc[1].SemanticName = "COLOR";
		vertexDesc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		vertexDesc[1].AlignedByteOffset = 12;
		vertexDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		vertexDesc[2].SemanticName = "TEXCOORD";
		vertexDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;
		vertexDesc[2].AlignedByteOffset = 24;
		vertexDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		vertexDesc[3].SemanticName = "NORMAL";
		vertexDesc[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		vertexDesc[3].AlignedByteOffset = 32;
		vertexDesc[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		vertexDesc[4].SemanticName = "TANGENT";
		vertexDesc[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		vertexDesc[4].AlignedByteOffset = 44;
		vertexDesc[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;


		// ===================
		// Create Input layout
		// ===================

		D3DX11_PASS_DESC passDesc{};
		m_TechniquePtr->GetPassByIndex(0)->GetDesc(&passDesc);

		HRESULT result = m_DevicePtr->CreateInputLayout(
			vertexDesc,
			numElements,
			passDesc.pIAInputSignature,
			passDesc.IAInputSignatureSize,
			&m_InputLayoutPtr
		);

		if (FAILED(result))
			assert(false && "Failed to create input layout");


		// ====================
		// Create Vertex Buffer
		// ====================

		D3D11_BUFFER_DESC bd{};
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = sizeof(Vertex) * static_cast<uint32_t>(m_Vertices.size());
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData{};
		initData.pSysMem = m_Vertices.data();

		result = m_DevicePtr->CreateBuffer(&bd, &initData, &m_VertexBufferPtr);

		if (FAILED(result))
			assert(false && "Failed to create Vertex Buffer");


		// ====================
		// Create Index Buffer
		// ====================

		m_NumIndices = static_cast<uint32_t>(m_Indices.size());
		bd.Usage = D3D11_USAGE_IMMUTABLE;
		bd.ByteWidth = sizeof(uint32_t) * m_NumIndices;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		initData.pSysMem = indices.data();
		
		result = m_DevicePtr->CreateBuffer(&bd, &initData, &m_IndexBufferPtr);
		if (FAILED(result)) 
		{
			assert(false && "Failed to create Index Buffer");
			return;
		}
	}

	Mesh::~Mesh()
	{
		if (m_VertexBufferPtr) m_VertexBufferPtr->Release();
		if (m_InputLayoutPtr) m_InputLayoutPtr->Release();
		if (m_IndexBufferPtr) m_IndexBufferPtr->Release();

		delete m_EffectFlatPtr;
		delete m_EffectCombinedPtr;
	}

	void Mesh::Render(ID3D11DeviceContext* pDeviceContext, const Matrix& worldViewProjectionMatrix, const Matrix& worldMatrix, 
		Texture* pDiffuseTexture, Texture* pNormalMapTexture, Texture* pSpecularMapTexture, Texture* pGlossinessTexture, Vector3 cameraPos) const
	{

		if (m_IsFlatEffect)
		{
			m_EffectFlatPtr->SetProjectionMatrix(worldViewProjectionMatrix);
			m_EffectFlatPtr->SetWorldMatrix(worldMatrix);

			m_EffectFlatPtr->SetDiffuseMap(pDiffuseTexture);
		}
		else
		{
			m_EffectCombinedPtr->SetProjectionMatrix(worldViewProjectionMatrix);
			m_EffectCombinedPtr->SetWorldMatrix(worldMatrix);
			m_EffectCombinedPtr->SetCameraPosition(cameraPos);

			m_EffectCombinedPtr->SetDiffuseMap(pDiffuseTexture);
			m_EffectCombinedPtr->SetNormalMap(pNormalMapTexture);
			m_EffectCombinedPtr->SetSpecularMap(pSpecularMapTexture);
			m_EffectCombinedPtr->SetGlossinessMap(pGlossinessTexture);
		}

		

		// 1. Set Primitive Topology
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 2. Set Input Layout
		pDeviceContext->IASetInputLayout(m_InputLayoutPtr);

		// 3. Set VertexBuffer
		constexpr UINT stride = sizeof(Vertex);
		constexpr UINT offset = 0;
		pDeviceContext->IASetVertexBuffers(0, 1, &m_VertexBufferPtr, &stride, &offset);

		// 4. Set IndexBuffer
		pDeviceContext->IASetIndexBuffer(m_IndexBufferPtr, DXGI_FORMAT_R32_UINT, 0);

		// 5. Draw
		D3DX11_TECHNIQUE_DESC techDesc{};
		m_TechniquePtr->GetDesc(&techDesc);

		if (m_PassIndex < techDesc.Passes)
		{
			m_TechniquePtr->GetPassByIndex(m_PassIndex)->Apply(0, pDeviceContext);
			pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
		}
	}
	void Mesh::ChangeFilteringMode(UINT passIndex)
	{
		m_PassIndex = passIndex;
	}
}