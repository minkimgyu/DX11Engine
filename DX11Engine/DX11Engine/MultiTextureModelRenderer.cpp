#include "MultiTextureModelRenderer.h"

#include <fstream>
using namespace std;


MultiTextureModelRenderer::MultiTextureModelRenderer(string modelPath, vector<string> texturePath)
{
	_modelPath = modelPath;
	_texturePath = texturePath;

	SetShaderNumber(2);
}


MultiTextureModelRenderer::MultiTextureModelRenderer(const MultiTextureModelRenderer& other)
{
}


MultiTextureModelRenderer::~MultiTextureModelRenderer()
{
}


bool MultiTextureModelRenderer::Initialize(ID3D11Device* device)
{
	// �� �����͸� �ε��մϴ�.
	if (!LoadModel(_modelPath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded))
	{
		return false;
	}

	// ���� �� �ε��� ���۸� �ʱ�ȭ�մϴ�.
	if (!InitializeBuffers(device))
	{
		return false;
	}

	// �� ���� �ؽ�ó�� �ε��մϴ�.
	return LoadTextures(device);
}


void MultiTextureModelRenderer::Shutdown()
{
	// �� �ؽ��ĸ� ��ȯ�մϴ�.
	ReleaseTextures();

	// ���ؽ� �� �ε��� ���۸� �����մϴ�.
	ShutdownBuffers();
}


void MultiTextureModelRenderer::Render(ID3D11DeviceContext* deviceContext)
{
	// �׸��⸦ �غ��ϱ� ���� �׷��� ������ ���ο� �������� �ε��� ���۸� �����ϴ�.
	RenderBuffers(deviceContext);
}


int MultiTextureModelRenderer::GetIndexCount()
{
	return _indexCount;
}


ID3D11ShaderResourceView** MultiTextureModelRenderer::GetTextureArray()
{
	return m_TextureArray->GetTextureArray();
}


bool MultiTextureModelRenderer::InitializeBuffers(ID3D11Device* device)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	if (!_vertices || !_indices)
		return false;

	// ���� ���� ������ ����ü�� �����մϴ�.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource ������ ���� �����Ϳ� ���� �����͸� �����մϴ�.
	vertexData.pSysMem = _vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// ���� ���� ���۸� ����ϴ�.
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		return false;
	}

	// ���� �ε��� ������ ����ü�� �����մϴ�.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// �ε��� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ��մϴ�.
	indexData.pSysMem = _indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// �ε��� ���۸� �����մϴ�.
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		return false;
	}

	// �����ǰ� ���� �Ҵ�� ���� ���ۿ� �ε��� ���۸� �����մϴ�.
	delete[] _vertices;
	_vertices = 0;

	delete[] _indices;
	_indices = 0;

	return true;
}


void MultiTextureModelRenderer::ShutdownBuffers()
{
	// �ε��� ���۸� �����մϴ�.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// ���� ���۸� �����մϴ�.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}


void MultiTextureModelRenderer::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	// ���� ������ ������ �������� �����մϴ�.
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	// ������ �� �� �ֵ��� �Է� ��������� ���� ���۸� Ȱ������ �����մϴ�.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// ������ �� �� �ֵ��� �Է� ��������� �ε��� ���۸� Ȱ������ �����մϴ�.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// ���� ���۷� �׸� �⺻���� �����մϴ�. ���⼭�� �ﰢ������ �����մϴ�.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


bool MultiTextureModelRenderer::LoadTextures(ID3D11Device* device)
{
	// �ؽ�ó �迭 ������Ʈ�� �����Ѵ�.
	m_TextureArray = new TextureArrayContainer;
	if (!m_TextureArray)
	{
		return false;
	}

	// �ؽ�ó �迭 ������Ʈ�� �ʱ�ȭ�Ѵ�.
	return m_TextureArray->Initialize(device, _texturePath);
}


void MultiTextureModelRenderer::ReleaseTextures()
{
	// �ؽ�ó �迭 ������Ʈ�� �������Ѵ�.
	if (m_TextureArray)
	{
		m_TextureArray->Shutdown();
		delete m_TextureArray;
		m_TextureArray = 0;
	}
}

bool MultiTextureModelRenderer::LoadModel(string filename, UINT flag)
{
	// Load model using ASSIMP
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(filename, flag);

	if (!pScene) return false;

	int count = pScene->mNumMeshes;

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

		// ��ġ �о����
		_indices[i * 3] = face.mIndices[0];
		_indices[i * 3 + 1] = face.mIndices[1];
		_indices[i * 3 + 2] = face.mIndices[2];
	}

	return true;
}