#include "pch.h"
#include "Texture.h"
#include "Vector2.h"
#include <SDL_image.h>

#include <iostream>
#include "MathHelpers.h"

namespace dae
{
    Texture::Texture(SDL_Surface* pSurface) :
        m_SurfacePtr{ pSurface },
        m_SurfacePixelsPtr{ (uint32_t*)pSurface->pixels }
    {

    }

    Texture::Texture(SDL_Surface* pSurface, ID3D11Device* devicePtr) :
        m_SurfacePtr{ pSurface },
        m_SurfacePixelsPtr{ (uint32_t*)pSurface->pixels }
    {
        DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

        D3D11_TEXTURE2D_DESC desc{};
        desc.Width = m_SurfacePtr->w;
        desc.Height = m_SurfacePtr->h;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = format;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA initData;
        initData.pSysMem = m_SurfacePtr->pixels;
        initData.SysMemPitch = static_cast<UINT>(m_SurfacePtr->pitch);
        initData.SysMemSlicePitch = static_cast<UINT>(m_SurfacePtr->pitch * m_SurfacePtr->h);

        HRESULT hr = devicePtr->CreateTexture2D(&desc, &initData, &m_ResourcePtr);
        if (FAILED(hr))
        {
            std::cout << "Texture::Texture() failed: " << hr << '\n';
            return;
        }
        D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc{};
        SRVDesc.Format = format;
        SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        SRVDesc.Texture2D.MipLevels = desc.MipLevels;
        hr = devicePtr->CreateShaderResourceView(m_ResourcePtr, &SRVDesc, &m_ShaderResourceViewPtr);
        if (FAILED(hr))
        {
            std::cout << "Texture::Texture() failed: " << hr << '\n';
            return;
        }

        // Clean up
        FreeSurface();
    }

    void Texture::FreeSurface()
    {
        if (m_SurfacePtr)
        {
            SDL_FreeSurface(m_SurfacePtr);
            m_SurfacePtr = nullptr;
        }  
    }

    Texture::~Texture()
    {
        FreeSurface();

        if (m_ResourcePtr) m_ResourcePtr->Release();
        if (m_ShaderResourceViewPtr) m_ShaderResourceViewPtr->Release();
    }

    Texture* Texture::LoadFromFile(const std::string& path)
    {
        //Load SDL_Surface using IMG_LOAD
        SDL_Surface* pSurface = IMG_Load(path.c_str());
        if (!pSurface) return nullptr;

        //Create & Return a new Texture Object (using SDL_Surface)
        return new Texture(pSurface);
    }

    Texture* Texture::LoadFromFile(const std::string& path, ID3D11Device* devicePtr)
    {
        //Load SDL_Surface using IMG_LOAD
        SDL_Surface* pSurface = IMG_Load(path.c_str());
        if (!pSurface) return nullptr;

        //Create & Return a new Texture Object (using SDL_Surface)
        return new Texture(pSurface, devicePtr);
    }

    ColorRGB Texture::Sample(const Vector2& uv) const
    {
        // clamp uv to [0,1]
        const float u{ dae::Clamp(uv.x, 0.f, 1.f) * float(m_SurfacePtr->w) };
        const float v{ dae::Clamp(uv.y, 0.f, 1.f) * float(m_SurfacePtr->h) };

        const int index{ static_cast<int>(u) + (static_cast<int>(v) * m_SurfacePtr->w) };

        Uint8 r{};
        Uint8 g{};
        Uint8 b{};

        SDL_GetRGB(m_SurfacePixelsPtr[index], m_SurfacePtr->format, &r, &g, &b);

        return ColorRGB{ static_cast<float>(r) / 255.f, static_cast<float>(g) / 255.f, static_cast<float>(b) / 255.f };
    }
    ID3D11ShaderResourceView* Texture::GetSRV() const
    {
        return m_ShaderResourceViewPtr;
    }
}