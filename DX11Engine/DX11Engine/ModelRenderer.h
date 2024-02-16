////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

/////////////
// LINKING //
/////////////
#pragma comment(lib, "lib/assimp-vc142-mtd.lib")

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
#include <string>

using namespace DirectX;

#include <fstream>
#include <atlstr.h>

using namespace std;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Component.h"
#include "Renderer.h"

// ASSIMP library
#include "include/assimp/Importer.hpp"
#include "include/assimp/scene.h"
#include "include/assimp/postprocess.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelRenderer : public Renderer
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

public:
	ModelRenderer(string);
	ModelRenderer(const ModelRenderer&);
	~ModelRenderer();

	virtual bool Initialize(ID3D11Device*);
	virtual void Render(ID3D11DeviceContext*);
	virtual int GetIndexCount();

	virtual void Shutdown();

	bool LoadModel(string, UINT);
	void ReleaseModel();

	// path get/set
	string GetPath();
	void SetPash(string);

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	string _modelPath;
	ID3D11Buffer *_vertexBuffer, *_indexBuffer;
	unsigned int _vertexCount, _indexCount, _faceCount;

	VertexType *_vertices;
	unsigned long *_indices;
};

#endif