////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "ModelRenderer.h"

ModelRenderer::ModelRenderer(string path)
{
	_vertexBuffer = 0;
	_indexBuffer = 0;
	_vertices = 0;
	_indices = 0;

	_vertexCount = 0;
	_indexCount = 0;
	_faceCount = 0;

	_modelPath = path;

}

ModelRenderer::ModelRenderer(const ModelRenderer& other)
{
}


ModelRenderer::~ModelRenderer()
{
}

// 이거 분할시켜서 하나는 생성자에서

bool ModelRenderer::Initialize(ID3D11Device* device)
{
	bool result;

	// Load the model data
	result = LoadModel(_modelPath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
	if (!result)
		return false;

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
		return false;
	
	return true;
}


void ModelRenderer::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}


void ModelRenderer::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int ModelRenderer::GetIndexCount()
{
	return _indexCount;
}

bool ModelRenderer::InitializeBuffers(ID3D11Device* device)
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
	if(FAILED(result))
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
	if(FAILED(result))
	{
		return false;
	}

	return true;
}


void ModelRenderer::ShutdownBuffers()
{
	// Release the index buffer.
	if(_indexBuffer)
	{
		_indexBuffer->Release();
		_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(_vertexBuffer)
	{
		_vertexBuffer->Release();
		_vertexBuffer = 0;
	}

	return;
}


void ModelRenderer::RenderBuffers(ID3D11DeviceContext* deviceContext)
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

bool ModelRenderer::LoadModel(string filename, UINT flag)
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
	
	// Fill each vertex data
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
		} else {
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

void ModelRenderer::ReleaseModel()
{
	if (_vertices)
	{
		delete[] _vertices;
		_vertices = 0;
	}

	return;
}

string ModelRenderer::GetPath()
{
	return _modelPath;
}

void ModelRenderer::SetPash(string path)
{
	_modelPath = path;
}