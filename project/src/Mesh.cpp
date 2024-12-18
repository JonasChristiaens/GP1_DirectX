#include "pch.h"
#include "Mesh.h"
#include "EffectBase.h"

namespace dae
{
	Mesh::Mesh(ID3D11Device* pDevice, const std::vector<PosCol3D>& vertices_in, const std::vector<uint32_t>& vertices_idx)
	{
		m_pEffect = new EffectBase(pDevice, L"Test");
	}

	Mesh::~Mesh()
	{
		delete m_pEffect;
	}

	void Mesh::Render(ID3D11DeviceContext* deviceContextPtr, const Matrix& viewProjectionMatrix) const
	{
		return void();
	}
}