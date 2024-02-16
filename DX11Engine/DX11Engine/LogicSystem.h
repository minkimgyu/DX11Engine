////////////////////////////////////////////////////////////////////////////////
// Filename: logicclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LOGICCLASS_H_
#define _LOGICCLASS_H_

//////////////
// INCLUDES //
//////////////
#include <list>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "D3dModule.h"
#include "GameObject.h"
#include "ModelRenderer.h"

using namespace std;

class LogicSystem
{
public:
	LogicSystem(list<GameObject*>* routines);
	LogicSystem(const LogicSystem&);
	~LogicSystem();

	bool Initialize(HWND);
	void Shutdown();
	bool Frame();

	void RunStart(GameObject*);
	void RunDestroy(GameObject*);

private:
	// 이렇게 2개 만들어서 사용하기
	// 오브젝트가 비활성화되면 진행하지 않음
	// OnEnable, OnDisable 함수도 만들어보자
	list<GameObject*>* _gameObjectsFromCore;
};

#endif