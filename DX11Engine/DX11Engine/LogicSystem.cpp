#include "LogicSystem.h"

LogicSystem::LogicSystem(list<GameObject*>* routines) 
{
	_gameObjectsFromCore = routines;
}

LogicSystem::LogicSystem(const LogicSystem&) {}

LogicSystem::~LogicSystem() {}

bool LogicSystem::Initialize(HWND hwnd)
{
	return true;
}

void LogicSystem::Shutdown()
{
}

bool LogicSystem::Frame()
{
	for (GameObject* tmpRoutine : *_gameObjectsFromCore)
	{
		tmpRoutine->Update();
	}

	return true;
}

void LogicSystem::RunStart(GameObject* gameObject)
{
	gameObject->Start();
}

void LogicSystem::RunDestroy(GameObject* gameObject)
{
	gameObject->OnDestroy();
}