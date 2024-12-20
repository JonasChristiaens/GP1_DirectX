#pragma once
#include "Renderer.h"

namespace dae
{
	struct Vertex
	{
		Vector3 position;
		ColorRGB color;
	};

	// class forwarding
	class EffectBase;

	class Mesh
	{
	public:
		Mesh(ID3D11Device* pDevice, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
		~Mesh();

		Mesh(const Mesh& other)						= delete;
		Mesh(Mesh&& other) noexcept					= delete;
		Mesh& operator =(const Mesh& other)			= delete;
		Mesh& operator =(Mesh&& other) noexcept		= delete;

		void Render(ID3D11DeviceContext* pDeviceContext) const;

	private:
		ID3D11Device*			m_DevicePtr{ nullptr };
		EffectBase*				m_EffectBasePtr{ nullptr };

		// shader variables
		ID3D11InputLayout*		m_InputLayoutPtr{ nullptr };
		ID3D11Buffer*			m_VertexBufferPtr{ nullptr };
		ID3D11Buffer*			m_IndexBufferPtr{ nullptr };

		// technique
		ID3DX11EffectTechnique*	m_TechniquePtr{ nullptr };
		ID3D11DeviceContext*	m_DeviceContextPtr{ nullptr };

		std::vector<Vertex>		m_Vertices{};
		std::vector<uint32_t>	m_Indices{};
		uint32_t				m_NumIndices = 0;
	};
}