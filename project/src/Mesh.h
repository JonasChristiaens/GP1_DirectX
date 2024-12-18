#pragma once

namespace dae
{
	struct PosCol3D
	{
		Vector3 position;
		ColorRGB color;
	};

	struct ID3D11DeviceContext;
	class EffectBase;

	class Mesh
	{
	public:
		Mesh(ID3D11Device* pDevice, const std::vector<PosCol3D>& vertices_in, const std::vector<uint32_t>& vertices_idx);
		~Mesh();

		void Render(ID3D11DeviceContext* deviceContextPtr, const Matrix& viewProjectionMatrix) const;

	private:
		EffectBase* m_pEffect{};

	};
}