////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTUREARRAYCONTAINER_H_
#define _TEXTUREARRAYCONTAINER_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <vector>
#include <string>

using namespace std;

class TextureArrayContainer
{
public:
	TextureArrayContainer();
	TextureArrayContainer(const TextureArrayContainer&);
	~TextureArrayContainer();

	bool Initialize(ID3D11Device*, vector<string>);
	void Shutdown();

	ID3D11ShaderResourceView** GetTextureArray();

private:
	int _sizeOfTextures = 2;
	ID3D11ShaderResourceView* _textures[2] = { nullptr, nullptr };
};

#endif