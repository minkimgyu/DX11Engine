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
	// 모델 데이터를 로드합니다.
	if (!LoadModel(_modelPath, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded))
	{
		return false;
	}

	// 정점 및 인덱스 버퍼를 초기화합니다.
	if (!InitializeBuffers(device))
	{
		return false;
	}

	// 이 모델의 텍스처를 로드합니다.
	return LoadTextures(device);
}


void MultiTextureModelRenderer::Shutdown()
{
	// 모델 텍스쳐를 반환합니다.
	ReleaseTextures();

	// 버텍스 및 인덱스 버퍼를 종료합니다.
	ShutdownBuffers();
}


void MultiTextureModelRenderer::Render(ID3D11DeviceContext* deviceContext)
{
	// 그리기를 준비하기 위해 그래픽 파이프 라인에 꼭지점과 인덱스 버퍼를 놓습니다.
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

	// 정적 정점 버퍼의 구조체를 설정합니다.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource 구조에 정점 데이터에 대한 포인터를 제공합니다.
	vertexData.pSysMem = _vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 이제 정점 버퍼를 만듭니다.
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		return false;
	}

	// 정적 인덱스 버퍼의 구조체를 설정합니다.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 인덱스 데이터를 가리키는 보조 리소스 구조체를 작성합니다.
	indexData.pSysMem = _indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 인덱스 버퍼를 생성합니다.
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		return false;
	}

	// 생성되고 값이 할당된 정점 버퍼와 인덱스 버퍼를 해제합니다.
	delete[] _vertices;
	_vertices = 0;

	delete[] _indices;
	_indices = 0;

	return true;
}


void MultiTextureModelRenderer::ShutdownBuffers()
{
	// 인덱스 버퍼를 해제합니다.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// 정점 버퍼를 해제합니다.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}


void MultiTextureModelRenderer::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	// 정점 버퍼의 단위와 오프셋을 설정합니다.
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	// 렌더링 할 수 있도록 입력 어셈블러에서 정점 버퍼를 활성으로 설정합니다.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// 렌더링 할 수 있도록 입력 어셈블러에서 인덱스 버퍼를 활성으로 설정합니다.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 정점 버퍼로 그릴 기본형을 설정합니다. 여기서는 삼각형으로 설정합니다.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


bool MultiTextureModelRenderer::LoadTextures(ID3D11Device* device)
{
	// 텍스처 배열 오브젝트를 생성한다.
	m_TextureArray = new TextureArrayContainer;
	if (!m_TextureArray)
	{
		return false;
	}

	// 텍스처 배열 오브젝트를 초기화한다.
	return m_TextureArray->Initialize(device, _texturePath);
}


void MultiTextureModelRenderer::ReleaseTextures()
{
	// 텍스처 배열 오브젝트를 릴리즈한다.
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

		// 위치 읽어오기
		_indices[i * 3] = face.mIndices[0];
		_indices[i * 3 + 1] = face.mIndices[1];
		_indices[i * 3 + 2] = face.mIndices[2];
	}

	return true;
}