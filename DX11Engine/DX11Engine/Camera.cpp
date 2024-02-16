////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Camera.h"


Camera::Camera()
{
	_position.x = 0.0f;
	_position.y = 0.0f;
	_position.z = 0.0f;

	_rotation.x = 0.0f;
	_rotation.y = 0.0f;
	_rotation.z = 0.0f;
}


Camera::Camera(const Camera& other)
{
}


Camera::~Camera()
{
}


void Camera::SetPosition(float x, float y, float z)
{
	_position.x = x;
	_position.y = y;
	_position.z = z;
}


void Camera::SetRotation(float x, float y, float z)
{
	_rotation.x = x;
	_rotation.y = y;
	_rotation.z = z;
}


XMFLOAT3 Camera::GetPosition()
{
	return _position;
}


XMFLOAT3 Camera::GetRotation()
{
	return _rotation;
}

// This uses the position and rotation of the camera to build and to update the view matrix.
void Camera::Render()
{
	XMVECTOR up, position, lookAt;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	// Setup the vector that points upwards.
	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// Setup the position of the camera in the world.
	position = XMLoadFloat3(&_position);

	// Setup where the camera is looking by default.
	lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = _rotation.x * 0.0174532925f;
	yaw   = _rotation.y * 0.0174532925f;
	roll  = _rotation.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	_viewMatrix = XMMatrixLookAtLH(position, lookAt, up);

	return;
}


void Camera::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = _viewMatrix;
}