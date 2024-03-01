#include "CoreSystem.h"

CoreSystem::CoreSystem()
{
	_LogicSystem = 0;
	_RenderingSystem = 0;
}

CoreSystem::CoreSystem(const CoreSystem&)
{
}

CoreSystem::~CoreSystem()
{
}

void CoreSystem::AddGameObject(GameObject* gameObject)
{
	_gameObjects.push_back(gameObject);
	_LogicSystem->RunStart(gameObject); // Delegate를 만들어 함수 포인터를 바인딩

	Renderer* tmp = gameObject->GetRenerer();
	if (tmp == nullptr) return;

	_RenderingSystem->InitializeRenderer(tmp); // 랜더러를 초기화 해준다.
}

void CoreSystem::RemoveGameObject(GameObject* gameObject)
{
	_LogicSystem->RunDestroy(gameObject); // Delegate를 만들어 함수 포인터를 바인딩

	// 배회 돌려서 삭제해주기
	for (GameObject* tmpEntity : _gameObjects)
	{
		if (tmpEntity == gameObject)
		{
			delete tmpEntity;
			_gameObjects.remove(tmpEntity);
		}
	}
}

bool CoreSystem::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Create the Direct3D object.
	_Camera = new Camera(&IsKeyPressedDelegate, &GetMouseStateDelegate);
	if (!_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	_Camera->SetPosition(0.0f, 0.0f, -7.0f);	// for cube model

	// Create the Direct3D object.
	_RenderingSystem = new RenderingSystem(_Camera, &_gameObjects);
	if (!_RenderingSystem)
	{
		return false;
	}

	// Do the frame processing for the graphics object.
	result = _RenderingSystem->Initialize(screenWidth, screenHeight, hwnd);
	if (!result)
	{
		return false;
	}

	// Create the Direct3D object.
	_LogicSystem = new LogicSystem(&_gameObjects);
	if (!_LogicSystem)
	{
		return false;
	}

	// Do the frame processing for the graphics object.
	result = _LogicSystem->Initialize(hwnd);
	if (!result)
	{
		return false;
	}

	// 오브젝트가 추가되는 로직은 여기서 진행하자
	GameObject* go = new GameObject(&IsKeyPressedDelegate);
	go->AddComponent(new MultiTextureModelRenderer("./data/tank.fbx", { "./data/tank_Col.dds" , "./data/tank_Tracks.dds" }));
		
	//go->AddComponent(new TextureModelRenderer("./data/chair.obj", { "./data/chair.dds" }));
	AddGameObject(go);

	return true;
}

void CoreSystem::Shutdown()
{
	// Release the graphics object.
	if (_RenderingSystem)
	{
		_RenderingSystem->Shutdown();
		delete _RenderingSystem;
		_RenderingSystem = 0;
	}

	// Release the graphics object.
	if (_LogicSystem)
	{
		_LogicSystem->Shutdown();
		delete _LogicSystem;
		_LogicSystem = 0;
	}
}

bool CoreSystem::Frame()
{
	bool result;

	// Do the frame processing for the graphics object.
	result = _LogicSystem->Frame();
	if (!result)
	{
		return false;
	}

	// Do the frame processing for the graphics object.
	result = _RenderingSystem->Frame();
	if (!result)
	{
		return false;
	}

	_Camera->Frame();

	return true;
}
