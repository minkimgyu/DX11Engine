////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "TextureContainer.h"
#include "DDSTextureLoader.h"

using namespace DirectX;

TextureContainer::TextureContainer()
{
	m_texture = 0;
}


TextureContainer::TextureContainer(const TextureContainer& other)
{
}


TextureContainer::~TextureContainer()
{
}


bool TextureContainer::Initialize(ID3D11Device* device, string filename)
{
	HRESULT result;

	wstring widestr = wstring(filename.begin(), filename.end());
	const wchar_t* wideCStr = widestr.c_str();

	// Load texture data from a file by using DDS texture loader.
	result = CreateDDSTextureFromFile(device, wideCStr, nullptr, &m_texture);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


void TextureContainer::Shutdown()
{
	// Release the texture resource.
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}


ID3D11ShaderResourceView* TextureContainer::GetTexture()
{
	return m_texture;
}