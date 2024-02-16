#include "Transform.h"

Transform::Transform()
{
	_position = XMFLOAT3(0, 0, 0);
	_rotation = XMFLOAT3(0, 0, 0);
	_scale = XMFLOAT3(1, 1, 1);
}

Transform::~Transform()
{

}

void Transform::SetPosition(float x, float y, float z)
{
	_position.x = x;
	_position.y = y;
	_position.z = z;
}

void Transform::SetRotation(float x, float y, float z)
{
	_rotation.x = x;
	_rotation.y = y;
	_rotation.z = z;
}

void Transform::SetScale(float x, float y, float z)
{
	_scale.x = x;
	_scale.y = y;
	_scale.z = z;
}

XMMATRIX Transform::ConvertToLcoalMatrix(XMMATRIX worldMatrix)
{
	// scale, rotation, position 순으로 지정

	XMMATRIX matrix;

	matrix = XMMatrixScaling(_scale.x, _scale.y, _scale.z);
	worldMatrix *= matrix;

	matrix = XMMatrixRotationX(_rotation.x);
	worldMatrix *= matrix;
	matrix = XMMatrixRotationY(_rotation.y);
	worldMatrix *= matrix;
	matrix = XMMatrixRotationZ(_rotation.z);
	worldMatrix *= matrix;

	matrix = XMMatrixTranslation(_position.x, _position.y, _position.z);
	worldMatrix *= matrix;

	return worldMatrix;
}
