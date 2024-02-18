////////////////////////////////////////////////////////////////////////////////
// Filename: TextureModelRenderer.cpp
////////////////////////////////////////////////////////////////////////////////
#include "TextureModelRenderer.h"

TextureModelRenderer::TextureModelRenderer(string modelPath, string texturePath)
{
	_vertexBuffer = 0;
	_indexBuffer = 0;
	_Texture = 0;

	_faceCount = 0;
	
	_shaderNumber = 1;

	_modelPath = modelPath;
	_texturePath = texturePath;
}

TextureModelRenderer::TextureModelRenderer(const TextureModelRenderer& other)
{
}


TextureModelRenderer::~TextureModelRenderer()
{
}


bool TextureModelRenderer::Initialize(ID3D11Device* device)
{
	bool result;

	// Load in the model data,
	result = LoadModel(_modelPath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, _texturePath);
	if (!result)
	{
		return false;
	}

	return true;
}


void TextureModelRenderer::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	return;
}


void TextureModelRenderer::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int TextureModelRenderer::GetIndexCount()
{
	return _indexCount;
}


ID3D11ShaderResourceView* TextureModelRenderer::GetTexture()
{
	return _Texture->GetTexture();
}


bool TextureModelRenderer::InitializeBuffers(ID3D11Device* device)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	if (!_vertices || !_indices)
		return false;

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = _vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = _indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


void TextureModelRenderer::ShutdownBuffers()
{
	// Release the index buffer.
	if (_indexBuffer)
	{
		_indexBuffer->Release();
		_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (_vertexBuffer)
	{
		_vertexBuffer->Release();
		_vertexBuffer = 0;
	}

	return;
}


void TextureModelRenderer::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


bool TextureModelRenderer::LoadTexture(ID3D11Device* device, string filename)
{
	bool result;


	// Create the texture object.
	_Texture = new TextureContainer;
	if (!_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = _Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}


void TextureModelRenderer::ReleaseTexture()
{
	// Release the texture object.
	if (_Texture)
	{
		_Texture->Shutdown();
		delete _Texture;
		_Texture = 0;
	}

	return;
}

bool TextureModelRenderer::LoadModel(string filename, UINT flag)
{
	// Load model using ASSIMP
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(filename, flag);

	if (!pScene) return false;

	const aiMesh* pMesh = pScene->mMeshes[0];	// Assume it's a single mesh

	if (!pMesh)
		return false;

	_vertexCount = pMesh->mNumVertices;
	_faceCount = pMesh->mNumFaces;
	_indexCount = _faceCount * 3;

	if (_vertexCount == 0 || _indexCount == 0)
		return false;

	_vertices = new VertexType[_vertexCount];

	// Fill each vertex datas
	for (unsigned int i = 0; i < _vertexCount; i++)
	{
		// Get position
		_vertices[i].position.x = pMesh->mVertices[i].x;
		_vertices[i].position.y = pMesh->mVertices[i].y;
		_vertices[i].position.z = pMesh->mVertices[i].z;

		// Get UV
		if (pMesh->HasTextureCoords(0)) {
			_vertices[i].texture.x = pMesh->mTextureCoords[0][i].x;
			_vertices[i].texture.y = pMesh->mTextureCoords[0][i].y;
		}
		else {
			_vertices[i].texture.x = 0;
			_vertices[i].texture.y = 0;
		}

		// Get normal
		_vertices[i].normal.x = pMesh->mNormals[i].x;
		_vertices[i].normal.y = pMesh->mNormals[i].y;
		_vertices[i].normal.z = pMesh->mNormals[i].z;
	}

	_indices = new unsigned long[_indexCount];

	for (unsigned int i = 0; i < _faceCount; i++)
	{
		aiFace face = pMesh->mFaces[i];

		// 위치 읽어오기
		_indices[i * 3] = face.mIndices[0];
		_indices[i * 3 + 1] = face.mIndices[1];
		_indices[i * 3 + 2] = face.mIndices[2];
	}

	return true;
}