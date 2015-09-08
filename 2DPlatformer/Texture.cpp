////////////////////////////////////////////////////////////////////////////////
// Filename: Texture.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Texture.h"

Texture::Texture()
{
	this->m_texture = nullptr;
}

Texture::Texture(const Texture& other)
{
}

Texture::~Texture()
{
}

bool Texture::Initialize(ID3D11Device* device, WCHAR* fileName)
{
	HRESULT result;

	result = D3DX11CreateShaderResourceViewFromFile(device, fileName, nullptr, nullptr, &this->m_texture, nullptr);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void Texture::Shutdown()
{
	SAFE_RELEASE(this->m_texture);
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return this->m_texture;
}

void Texture::GetTexture(ID3D11ShaderResourceView*& texture)
{
	texture = this->m_texture;
}
