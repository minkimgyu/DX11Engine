////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTUREMODELRENDERER_H_
#define _TEXTUREMODELRENDERER_H_

/////////////
// LINKING //
/////////////
#pragma comment(lib, "lib/assimp-vc142-mtd.lib")


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
#include <fstream>

#include <string>

using namespace DirectX;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Renderer.h"
#include "TextureContainer.h"

// ASSIMP library
#include "include/assimp/Importer.hpp"
#include "include/assimp/scene.h"
#include "include/assimp/postprocess.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class TextureModelRenderer : public Renderer
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

public:
	TextureModelRenderer(string, string);
	TextureModelRenderer(const TextureModelRenderer&);
	~TextureModelRenderer();

	virtual bool Initialize(ID3D11Device*);
	virtual void Shutdown();
	virtual void Render(ID3D11DeviceContext*);

	virtual int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	bool LoadModel(string, UINT);

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, string);
	void ReleaseTexture();

private:
	string _modelPath, _texturePath;

	ID3D11Buffer* _vertexBuffer, * _indexBuffer;
	int _vertexCount, _indexCount, _faceCount;
	TextureContainer* _Texture;

	VertexType* _vertices;
	unsigned long* _indices;
};

#endif