#include "TextureArrayContainer.h"
#include "DDSTextureLoader.h"

using namespace DirectX;

TextureArrayContainer::TextureArrayContainer()
{
}

TextureArrayContainer::TextureArrayContainer(const TextureArrayContainer&)
{
}

TextureArrayContainer::~TextureArrayContainer()
{
}

bool TextureArrayContainer::Initialize(ID3D11Device* device, vector<string> fileNames)
{
	for (int i = 0; i < _sizeOfTextures; i++)
	{
		wstring widestr = wstring(fileNames[i].begin(), fileNames[i].end());
		const wchar_t* wideCStr = widestr.c_str();

		// ù��° �ؽ�ó�� ���Ϸκ��� �о�´�
		if (FAILED(CreateDDSTextureFromFile(device, wideCStr, nullptr, &_textures[i])))
		{
			return false;
		}
	}

	return true;
}

void TextureArrayContainer::Shutdown()
{
	// �ؽ�ó ���ҽ��� �����Ѵ�.
	if (_textures[0])
	{
		_textures[0]->Release();
		_textures[0] = 0;
	}

	if (_textures[1])
	{
		_textures[1]->Release();
		_textures[1] = 0;
	}

	return;
}

ID3D11ShaderResourceView** TextureArrayContainer::GetTextureArray()
{
	return _textures;
}
