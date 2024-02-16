////////////////////////////////////////////////////////////////////////////////
// Filename: entity.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

//////////////
// INCLUDES //
//////////////
#include <vector>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Component.h"
#include "Transform.h"
#include "Renderer.h"
#include "Routine.h"

#include "Delegate.h"

#include "InputSystem.h"

using namespace std;

#pragma once
class GameObject
{
public:
	GameObject(Delegate<InputSystem>*);
	GameObject(const GameObject&);
	~GameObject();

	vector<Component*>* GetComponents();

	// 템플릿을 사용한 함수 선언
	template <typename T>
	T GetComponent()
	{
		for (Component* component : _components)
		{
			T tmp = dynamic_cast<T>(component);
			if (tmp != nullptr) return tmp;
		}
	}

	virtual void Start();
	virtual void Update();
	virtual void OnDestroy();

	virtual void OnEnable();
	virtual void OnDisable();

	void AddComponent(Component*);
	void AddComponent(Renderer*);

	void RemoveComponent(Component*);

	Renderer* GetRenerer();
	Transform* GetTransform();

	void SetActive(bool);
	bool GetActive();


private:
	void AddComponent(Transform*);

private:
	bool _isActive;
	Renderer* _renderer;
	Transform* _transform;
	vector<Component*> _components; // 삽입 삭제가 많지 않으므로 vector 사용

	Delegate<InputSystem>* IsKeyPressed;
};

#endif