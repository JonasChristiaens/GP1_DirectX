#pragma once

struct SDL_Window;
struct SDL_Surface;

namespace dae
{
	class Camera;
	class Mesh;
	class Texture;

	class Renderer final
	{
	public:
		enum class FilteringMode
		{
			Point,
			Linear,
			Anisotropic,
			COUNT
		};

		Renderer(SDL_Window* pWindow);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void Update(const Timer* pTimer);
		void Render() const;

		void ChangeFilteringMode();
		void ToggleRotation();
	private:
		SDL_Window*				m_pWindow{};

		int						m_Width{};
		int						m_Height{};

		bool					m_IsInitialized{ false };
		bool					m_Rotate{ true };

		Camera*					m_Camera{ nullptr };
		Mesh*					m_MeshPtr{ nullptr };
		Texture*				m_DiffuseTexturePtr{ nullptr };
		Texture*				m_NormalMapTexturePtr{ nullptr };
		Texture*				m_SpecularMapTexturePtr{ nullptr };
		Texture*				m_GlossinessTexturePtr{ nullptr };

		FilteringMode			m_CurrentFilteringMode{ };

		//DIRECTX
		HRESULT InitializeDirectX();

		ID3D11Device*			m_pDevice{};
		ID3D11DeviceContext*	m_pDeviceContext{};
		IDXGISwapChain*			m_pSwapChain{};
		ID3D11Texture2D*		m_pDepthStencilBuffer{};
		ID3D11DepthStencilView* m_pDepthStencilView{};
		ID3D11Resource*			m_pRenderTargetBuffer{};
		ID3D11RenderTargetView* m_pRenderTargetView{};
	};
}
