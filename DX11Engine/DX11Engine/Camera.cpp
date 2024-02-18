////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Camera.h"


Camera::Camera(Delegate<bool, InputSystem, int>* delegate1, Delegate<DIMOUSESTATE, InputSystem>* delegate2)
{
	IsKeyPressed = delegate1;
	GetMouseState = delegate2;

	m_camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//Set the View matrix
	//m_camView = XMMatrixLookAtLH(m_camPosition, m_camTarget, m_camUp);

	//Set the Projection matrix
	m_camProjection = XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)800 / (float)600, 1.0f, 1000.0f);
}


Camera::Camera(const Camera& other)
{
}


Camera::~Camera()
{
}


void Camera::SetPosition(float x, float y, float z)
{
	m_camPosition = XMVectorSet(x, y, z, 0.0f);
}


XMFLOAT3 Camera::GetPosition()
{
	XMFLOAT3 tmp;
	XMStoreFloat3(&tmp, m_camPosition);

	return tmp;
}

// This uses the position and rotation of the camera to build and to update the view matrix.
void Camera::Render()
{
	m_camRotationMatrix = XMMatrixRotationRollPitchYaw(m_camPitch, m_camYaw, 0);

	m_camTarget = XMVector3TransformCoord(m_DefaultForward, m_camRotationMatrix); // 카메라 Y축 회전으로 움직일 방향을 지정해줌
	m_camTarget = XMVector3Normalize(m_camTarget); // 이후 nomalize 진행

	// First-Person Camera
	//XMMATRIX RotateYTempMatrix;
	//RotateYTempMatrix = XMMatrixRotationY(m_camYaw); // 위의 변수를 이용해서 돌림

	//m_camRight = XMVector3TransformCoord(m_DefaultRight, RotateYTempMatrix); // 카메라 오른쪽 백터
	//m_camUp = XMVector3TransformCoord(m_camUp, RotateYTempMatrix); //카메라 기준 위 백터
	//m_camForward = XMVector3TransformCoord(m_DefaultForward, RotateYTempMatrix); // 카메라 앞 백터

	// Free-Look Camera
	m_camRight = XMVector3TransformCoord(m_DefaultRight, m_camRotationMatrix);
	m_camForward = XMVector3TransformCoord(m_DefaultForward, m_camRotationMatrix);
	m_camUp = XMVector3Cross(m_camForward, m_camRight);


	m_camPosition += m_moveLeftRight * m_camRight; // 위 내용을 바탕으로 카메라 위치 재지정
	m_camPosition += m_moveBackForward * m_camForward;

	m_moveLeftRight = 0.0f;
	m_moveBackForward = 0.0f;

	m_camTarget = m_camPosition + m_camTarget; // 최종적으로 camTarget을 지정해줌
	m_camView = XMMatrixLookAtLH(m_camPosition, m_camTarget, m_camUp);

	return;
}

void Camera::Frame()
{
	if (IsKeyPressed->Invoke(DIK_W))
	{
		MoveVertical(true);
	}
	if (IsKeyPressed->Invoke(DIK_A))
	{
		MoveHorizontal(true);
	}
	if (IsKeyPressed->Invoke(DIK_S))
	{
		MoveVertical(false);
	}
	if (IsKeyPressed->Invoke(DIK_D))
	{
		MoveHorizontal(false);
	}
	if (IsKeyPressed->Invoke(DIK_Q))
	{
		XMFLOAT3 matrix = GetPosition();
		SetPosition(matrix.x, matrix.y + m_speed, matrix.z);	// for chair
	}
	if (IsKeyPressed->Invoke(DIK_E))
	{
		XMFLOAT3 matrix = GetPosition();
		SetPosition(matrix.x, matrix.y - m_speed, matrix.z);	// for chair
	}

	DIMOUSESTATE mouseState = GetMouseState->Invoke();

	if ((mouseState.lX != _mouseLastState.lX) || (mouseState.lY != _mouseLastState.lY))
	{
		MoveMouse(mouseState, _mouseLastState);
		_mouseLastState = mouseState;
	}
}

void Camera::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_camView;
}

void Camera::MoveHorizontal(bool isLeft)
{
	if (isLeft == true)
		m_moveLeftRight -= m_speed;
	else
		m_moveLeftRight += m_speed;
}

void Camera::MoveVertical(bool isUp)
{
	if (isUp == true)
		m_moveBackForward += m_speed;
	else
		m_moveBackForward -= m_speed;
}

void Camera::MoveMouse(DIMOUSESTATE mouseCurrState, DIMOUSESTATE mouseLastState)
{
	m_camYaw += mouseLastState.lX * 0.001f;
	m_camPitch += mouseCurrState.lY * 0.001f;
}