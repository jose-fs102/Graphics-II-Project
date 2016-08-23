#include "CameraClass.h"


CameraClass::CameraClass()
{
	timer.Restart();

	viewMatrix = XMMatrixIdentity();
}

CameraClass::CameraClass(CameraClass & other)
{
}

CameraClass::~CameraClass()
{
}

void CameraClass::Initialize(XMVECTOR position, XMVECTOR direction, float fieldOfView, float aspectRatio, const float zNear, const float zFar, HWND hwnd)
{
	hWnd = hwnd;
	viewMatrix = XMMatrixLookAtLH(position, position + direction, { 0.0f, 1.0f, 0.0f });
	projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, zNear, zFar);
}

void CameraClass::CheckInput()
{
	timer.Signal();

	POINT localMouseLook;
	GetCursorPos(&localMouseLook);
	ScreenToClient(hWnd, &localMouseLook);

	float deltaX = 0.0f, deltaY = 0.0f;

	if (GetAsyncKeyState(VK_RBUTTON))
	{
		deltaX = (float)localMouseLook.x - (float)mouseLook.x;
		deltaY = (float)localMouseLook.y - (float)mouseLook.y;
	}

	mouseLook = localMouseLook;

	XMMATRIX tempMatrix = XMMatrixIdentity();

	if (GetAsyncKeyState('W'))
	{
		tempMatrix = XMMatrixTranslation(0.0f, 0.0f, timer.Delta() * cameraSpeed);
		viewMatrix = XMMatrixMultiply(tempMatrix, viewMatrix);
	}

	if (GetAsyncKeyState('A'))
	{
		tempMatrix = XMMatrixTranslation(-timer.Delta() *cameraSpeed, 0.0f, 0.0f);
		viewMatrix = XMMatrixMultiply(tempMatrix, viewMatrix);
	}

	if (GetAsyncKeyState('S'))
	{
		tempMatrix = XMMatrixTranslation(0.0f, 0.0f, -timer.Delta() * cameraSpeed);
		viewMatrix = XMMatrixMultiply(tempMatrix, viewMatrix);
	}

	if (GetAsyncKeyState('D'))
	{
		tempMatrix = XMMatrixTranslation(timer.Delta() * cameraSpeed, 0.0f, 0.0f);
		viewMatrix = XMMatrixMultiply(tempMatrix, viewMatrix);
	}

	if (GetAsyncKeyState('Q'))
	{
		tempMatrix = XMMatrixTranslation(0.0f, timer.Delta() * cameraSpeed, 0.0f);
		viewMatrix = XMMatrixMultiply(tempMatrix, viewMatrix);
	}

	if (GetAsyncKeyState('Z'))
	{
		tempMatrix = XMMatrixTranslation(0.0f, -timer.Delta() * cameraSpeed, 0.0f);
		viewMatrix = XMMatrixMultiply(tempMatrix, viewMatrix);
	}

	XMMATRIX rotationMatrix = XMMatrixIdentity();
	XMVECTOR tempPosition = viewMatrix.r[3];
	viewMatrix.r[3] = { 0.0f, 0.0f, 0.0f, 1.0f };

	if (fabs(deltaX) > fabs(deltaY))
	{
		rotationMatrix = XMMatrixRotationY(10.0f * deltaX * timer.Delta());
		viewMatrix = XMMatrixMultiply(viewMatrix, rotationMatrix);
	}
	else
	{
		rotationMatrix = XMMatrixRotationX(10.0f * deltaY * timer.Delta());
		viewMatrix = XMMatrixMultiply(rotationMatrix,viewMatrix);
	}

	viewMatrix.r[3] = tempPosition;
}

XMMATRIX CameraClass::GetInverseViewMatrix()
{
	return XMMatrixInverse(0, viewMatrix);
}

XMMATRIX CameraClass::GetViewMatrix()
{
	return viewMatrix;
}

XMMATRIX CameraClass::GetProjectionMatrix()
{
	return projectionMatrix;
}

void CameraClass::SetProjectionMatrix(XMMATRIX& newProjectionMatrix)
{
	projectionMatrix = newProjectionMatrix;
}

void CameraClass::SetCameraSpeed(float speed)
{
	cameraSpeed = speed;
}