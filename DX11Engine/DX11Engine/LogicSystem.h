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
	// �̷��� 2�� ���� ����ϱ�
	// ������Ʈ�� ��Ȱ��ȭ�Ǹ� �������� ����
	// OnEnable, OnDisable �Լ��� ������
	list<GameObject*>* _gameObjectsFromCore;
};

#endif