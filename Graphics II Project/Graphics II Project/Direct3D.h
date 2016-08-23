#pragma once

#include<d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include<DirectXMath.h>
using namespace DirectX;

#include"CameraClass.h"

// Safe Release for pointers
#define SAFE_RELEASE(p) {if(p){p->Release(); p = 0;}}

// Check to see if an HResult has failed
#define HR(hr) {if(hr != S_OK) {assert(false);}}

// PI
#define PI 3.141592654f

class Direct3D
{
private:

	IDXGISwapChain* theSwapChain;
	ID3D11Resource* theResource;
	ID3D11Device* theDevice;
	ID3D11DeviceContext* theDeviceContext;
	ID3D11RenderTargetView* theRenderTarget;
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView* depthStencilView;

	CameraClass* cameraPtr;

public:

	Direct3D();
	Direct3D(const Direct3D&);
	~Direct3D();

	bool Initialize(int, int, HWND, float, float);
	void ShutDown();

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	ID3D11DepthStencilView* GetDepthStencilView();
	CameraClass* GetCameraPtr();

	void Resize(unsigned int width, unsigned int height);
};