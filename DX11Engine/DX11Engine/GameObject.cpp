#include "GameObject.h"
#include "InputSystem.h"

GameObject::GameObject(Delegate<InputSystem>* delegate)
{
	_isActive = true;
	_renderer = nullptr;

	IsKeyPressed = delegate;

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
	_renderer = renderer; // 만약 렌더러가 들어온다면 이를 저장해두기
}

void GameObject::AddComponent(Transform* transform)
{
	_components.push_back(transform);
	_transform = transform; // 만약 렌더러가 들어온다면 이를 저장해두기
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