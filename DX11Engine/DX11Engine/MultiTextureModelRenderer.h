////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MULTITEXTUREMODELRENDERER_H_
#define _MULTITEXTUREMODELRENDERER_H_

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

#include <vector>
#include <string>

using namespace DirectX;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Renderer.h"
#include "TextureArrayContainer.h"

// ASSIMP library
#include "include/assimp/Importer.hpp"
#include "include/assimp/scene.h"
#include "include/assimp/postprocess.h"

using namespace std;
class MultiTextureModelRenderer : public Renderer
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

public:
	MultiTextureModelRenderer(string, vector<string>);
	MultiTextureModelRenderer(const MultiTextureModelRenderer&);
	~MultiTextureModelRenderer();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView** GetTextureArray();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTextures(ID3D11Device*);
	void ReleaseTextures();

	bool LoadModel(string, UINT);

private:
	string _modelPath;
	vector<string> _texturePath;
	ID3D11Buffer* m_vertexBuffer, *m_indexBuffer;
	int _vertexCount, _indexCount, _faceCount;
	TextureArrayContainer* m_TextureArray;

	VertexType* _vertices;
	unsigned long* _indices;
};

#endif