#include "GameObject.h"
#include "InputSystem.h"

GameObject::GameObject(Delegate<bool, InputSystem, int>* delegate)
{
	_isActive = true;
	_renderer = nullptr;

	IsKeyPressed = delegate;
	_rotationValue = 0;

	AddComponent(new Transform());
}

GameObject::GameObject(const GameObject&)
{
}

GameObject::~GameObject()
{
}

vector<Component*>* GameObject::GetComponents()
{
	return &_components;
}

void GameObject::Start()
{
	if (_isActive == false) return;
}

void GameObject::Update()
{
	if (_isActive == false) return;

	if (IsKeyPressed->Invoke(DIK_1))
	{
		_rotationValue += 0.3f;
		_transform->SetRotation(0, _rotationValue, 0);
	}

	if (IsKeyPressed->Invoke(DIK_2))
	{
		_rotationValue += 0.3f;
		_transform->SetRotation(0, _rotationValue, _rotationValue);
	}
}

void GameObject::OnDestroy()
{
}

void GameObject::OnEnable()
{
}

void GameObject::OnDisable()
{
}

void GameObject::AddComponent(Component* component)
{
	_components.push_back(component);
}

void GameObject::AddComponent(Renderer* renderer)
{
	_components.push_back(renderer);
	_renderer = renderer; // ���� �������� ���´ٸ� �̸� �����صα�
}

void GameObject::AddComponent(Transform* transform)
{
	_components.push_back(transform);
	_transform = transform; // ���� �������� ���´ٸ� �̸� �����صα�
}

Renderer* GameObject::GetRenerer()
{
	return _renderer;
}

Transform* GameObject::GetTransform()
{
	return _transform;
}

void GameObject::SetActive(bool active)
{
	_isActive = active;
}

bool GameObject::GetActive()
{
	return _isActive;
}

void GameObject::RemoveComponent(Component* component)
{
	_components.erase(remove(_components.begin(), _components.end(), component), _components.end());
}