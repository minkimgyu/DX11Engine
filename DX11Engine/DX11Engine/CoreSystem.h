////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CORESYSTEM_H_
#define _CORESYSTEM_H_

//////////////
// INCLUDES //
//////////////
#include <list>
#include <functional>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "GameObject.h"
#include "Routine.h"
#include "Renderer.h"
#include "InputSystem.h"
#include "LogicSystem.h"
#include "RenderingSystem.h"

#include "Delegate.h"

class CoreSystem
{

public:
	CoreSystem();
	CoreSystem(const CoreSystem&);
	~CoreSystem();

	void AddGameObject(GameObject*);
	void RemoveGameObject(GameObject*);
	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

    Delegate<InputSystem> IsKeyPressedDelegate;

private:
	list<GameObject*> _gameObjects;

	RenderingSystem* _RenderingSystem;
	LogicSystem* _LogicSystem;
	Camera* _Camera;
};

#endif