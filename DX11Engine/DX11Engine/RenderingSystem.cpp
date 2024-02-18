////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "RenderingSystem.h"


RenderingSystem::RenderingSystem(Camera* camera, list<GameObject*>* objects)
{
	_D3dModule = 0;
	_ColorShader = 0;

	_CameraFromSystem = camera;
	_gameObjectsFromCore = objects;
}

RenderingSystem::RenderingSystem(const RenderingSystem& other) {}

RenderingSystem::~RenderingSystem() {}

// 게임 오브젝트가 추가될 때 작동한다.
void RenderingSystem::InitializeRenderer(Renderer* renderer)
{
	renderer->Initialize(_D3dModule->GetDevice()); // 렌더러가 추가될 때 초기화 진행
}

bool RenderingSystem::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Create the Direct3D object.
	_D3dModule = new D3dModule;
	if(!_D3dModule)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = _D3dModule->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the color shader object.
	_ColorShader = new ColorShader;
	if(!_ColorShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = _ColorShader->Initialize(_D3dModule->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the texture shader object.
	_TextureShader = new TextureShader;
	if (!_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = _TextureShader->Initialize(_D3dModule->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void RenderingSystem::Shutdown()
{
	if (_CameraFromSystem)
	{
		_CameraFromSystem = 0;
	}

	// Release the D3D object.
	if(_D3dModule)
	{
		_D3dModule->Shutdown();
		delete _D3dModule;
		_D3dModule = 0;
	}

	// Release the texture shader object.
	if (_ColorShader)
	{
		_ColorShader->Shutdown();
		delete _ColorShader;
		_ColorShader = 0;
	}

	// Release the texture shader object.
	if (_TextureShader)
	{
		_TextureShader->Shutdown();
		delete _TextureShader;
		_TextureShader = 0;
	}

	return;
}


bool RenderingSystem::Frame()
{
	bool result;

	// Render the graphics scene.
	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}

bool RenderingSystem::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	//XMMATRIX tempMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	_D3dModule->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	_CameraFromSystem->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	_CameraFromSystem->GetViewMatrix(viewMatrix);
	_D3dModule->GetWorldMatrix(worldMatrix);
	_D3dModule->GetProjectionMatrix(projectionMatrix);

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	/*tempMatrix = XMMatrixRotationX(rotation);
	worldMatrix *= tempMatrix;
	tempMatrix = XMMatrixRotationY(rotation);
	worldMatrix *= tempMatrix;
	tempMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f);
	worldMatrix *= tempMatrix;*/

	// --> Transform 값 받아서 적용시키기


	// 이거는 따로 Logic 클레스에서 진행

	for (GameObject* tmpObject : *_gameObjectsFromCore)
	{
		Renderer* renderer = tmpObject->GetRenerer();
		if (renderer == nullptr) continue;

		Transform* transform = tmpObject->GetTransform();
		XMMATRIX localMatrix = transform->ConvertToLcoalMatrix(worldMatrix);

		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		renderer->Render(_D3dModule->GetDeviceContext());

		int num = renderer->GetShaderNumber();

		if (num == 0)
		{
			// Render the model using the texture shader.
			result = _ColorShader->Render(_D3dModule->GetDeviceContext(), renderer->GetIndexCount(),
				localMatrix, viewMatrix, projectionMatrix);
			if (!result)
			{
				return false;
			}
		}
		else if (num == 1)
		{
			// Render the model using the texture shader.
			result = _TextureShader->Render(_D3dModule->GetDeviceContext(), renderer->GetIndexCount(),
				localMatrix, viewMatrix, projectionMatrix, renderer->GetTexture());
			if (!result)
			{
				return false;
			}
		}
	}

	// Present the rendered scene to the screen.
	_D3dModule->EndScene();

	return true;
}