#pragma once

#include<DirectXMath.h>
#include<Windows.h>
#include"XTime.h"
using namespace DirectX;

class CameraClass
{
public:

	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;

	XTime timer;

	POINT mouseLook;

	HWND hWnd;

	float cameraSpeed;

public:

	CameraClass();
	CameraClass(CameraClass&);
	~CameraClass();

	void Initialize(XMVECTOR, XMVECTOR, float, float, const float, const float, HWND);

	void CheckInput();

	XMMATRIX GetInverseViewMatrix();
	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();

	void SetProjectionMatrix(XMMATRIX&);
	void SetCameraSpeed(float);
};