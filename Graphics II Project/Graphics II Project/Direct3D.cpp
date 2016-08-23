#include "Direct3D.h"


Direct3D::Direct3D()
{
	theSwapChain = 0;
	theResource = 0;
	theDevice = 0;
	theDeviceContext = 0;
	theRenderTarget = 0;
	depthStencilBuffer = 0;
	depthStencilState = 0;
	depthStencilView = 0;

	cameraPtr = 0;
}

Direct3D::Direct3D(const Direct3D& other)
{
	// Empty 
}

Direct3D::~Direct3D()
{
	// cleanup happens in shutdown()
}

bool Direct3D::Initialize(int screenWidth, int screenHeight, HWND hwnd, float screenDepth, float screenNear)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	DXGI_SAMPLE_DESC sampleDesc;
	D3D_FEATURE_LEVEL featureLevel;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
	D3D11_VIEWPORT viewport;

#pragma region SWAP_CHAIN_SETUP

	sampleDesc.Count = 1;
	sampleDesc.Quality = 0;

#ifndef _DEBUG

	UINT flag = D3D11_CREATE_DEVICE_SINGLETHREADED;

#else

	UINT flag = D3D11_CREATE_DEVICE_DEBUG;

#endif

	// Swap Chain Description
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.SampleDesc = sampleDesc;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Create the device and swap chain
	HR(D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, flag, 0, 0, D3D11_SDK_VERSION, &swapChainDesc, &theSwapChain, &theDevice, &featureLevel, &theDeviceContext));

	// Extract a buffer from the swap chain
	theSwapChain->GetBuffer(0, __uuidof(theResource),
		reinterpret_cast<void**>(&theResource));

	// Create a Render Target View from that buffer
	theDevice->CreateRenderTargetView(theResource, NULL, &theRenderTarget);

	// Viewport structure
	viewport.Width = screenWidth;
	viewport.Height = screenHeight;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

#pragma endregion

#pragma region DEPTH_BUFFER_SETUP

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = swapChainDesc.BufferDesc.Width;
	depthBufferDesc.Height = swapChainDesc.BufferDesc.Height;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.SampleDesc = sampleDesc;

	HR(theDevice->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer));

#pragma endregion

#pragma region DEPTH_STENCIL_STATE_AND_VIEW_SETUP

	ZeroMemory(&depthStencilStateDesc, sizeof(depthStencilStateDesc));

	depthStencilStateDesc.DepthEnable = true;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilStateDesc.StencilEnable = true;
	depthStencilStateDesc.StencilReadMask = 0xFF;
	depthStencilStateDesc.StencilWriteMask = 0xFF;
	depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	HR(theDevice->CreateDepthStencilState(&depthStencilStateDesc, &depthStencilState));
	HR(theDevice->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView));

#pragma endregion

#pragma region CAMERA_AND_MATRIX_SETUP

	float fieldOfView, aspectRatio;
	cameraPtr = new CameraClass;

	fieldOfView = PI / 4.0f;
	aspectRatio = (float)screenWidth / (float)screenHeight;

	cameraPtr->Initialize({ 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 0.0f }, fieldOfView, aspectRatio, screenNear, screenDepth, hwnd);
	cameraPtr->SetCameraSpeed(10.0f);

#pragma endregion

	theDeviceContext->RSSetViewports(1, &viewport);
	theDeviceContext->OMSetRenderTargets(1, &theRenderTarget, depthStencilView);

	return true;
}

void Direct3D::ShutDown()
{
	// Release pointers safely
	SAFE_RELEASE(theSwapChain);
	SAFE_RELEASE(theDevice);
	SAFE_RELEASE(theDeviceContext);
	SAFE_RELEASE(theRenderTarget);
	SAFE_RELEASE(theResource);
	SAFE_RELEASE(depthStencilBuffer);
	SAFE_RELEASE(depthStencilState);
	SAFE_RELEASE(depthStencilView);

	if (cameraPtr)
	{
		delete cameraPtr;
		cameraPtr = 0;
	}
}

void Direct3D::BeginScene(float r, float g, float b, float alpha)
{
	float backgroundColor[4];

	backgroundColor[0] = r;
	backgroundColor[1] = g;
	backgroundColor[2] = b;
	backgroundColor[3] = alpha;

	theDeviceContext->ClearRenderTargetView(theRenderTarget, backgroundColor);
	theDeviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	cameraPtr->CheckInput();

	return;
}

void Direct3D::EndScene()
{
	theSwapChain->Present(0, 0);
}

ID3D11Device * Direct3D::GetDevice()
{
	return theDevice;
}

ID3D11DeviceContext * Direct3D::GetDeviceContext()
{
	return theDeviceContext;
}

ID3D11DepthStencilView * Direct3D::GetDepthStencilView()
{
	return depthStencilView;
}

CameraClass * Direct3D::GetCameraPtr()
{
	return cameraPtr;
}

void Direct3D::Resize(unsigned int width, unsigned int height)
{
	if (theDevice != NULL && theDeviceContext != NULL && theSwapChain != NULL)
	{
		// Clear and release 
		theDeviceContext->ClearState();

		SAFE_RELEASE(depthStencilBuffer);
		SAFE_RELEASE(theRenderTarget);
		SAFE_RELEASE(depthStencilView);
		SAFE_RELEASE(theResource);

		// Remake render target and resize swapchain
		theDeviceContext->OMSetRenderTargets(0, 0, 0);
		theSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

		HR(theSwapChain->GetBuffer(0, __uuidof(theResource), (void**)&theResource));
		HR(theDevice->CreateRenderTargetView(theResource, NULL, &theRenderTarget));

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		theSwapChain->GetDesc(&swapChainDesc);

		// Rebuild Projection matrix
		XMMATRIX projectionMatrix;

		float fieldOfView = PI / 4.0f;
		float aspectRatio = (float)width / (float)height;
		float zNear = 0.1f;
		float zFar = 1000.0f;

		projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, zNear, zFar);

		cameraPtr->SetProjectionMatrix(projectionMatrix);

		D3D11_TEXTURE2D_DESC depthBuffer;
		ZeroMemory(&depthBuffer, sizeof(depthBuffer));

		DXGI_SAMPLE_DESC sampleDescription;
		sampleDescription.Count = 1;
		sampleDescription.Quality = 0;

		depthBuffer.Width = swapChainDesc.BufferDesc.Width;
		depthBuffer.Height = swapChainDesc.BufferDesc.Height;
		depthBuffer.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBuffer.Format = DXGI_FORMAT_D32_FLOAT;
		depthBuffer.ArraySize = 1;
		depthBuffer.Usage = D3D11_USAGE_DEFAULT;
		depthBuffer.SampleDesc = sampleDescription;

		HR(theDevice->CreateTexture2D(&depthBuffer, NULL, &depthStencilBuffer));
		HR(theDevice->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView));

		theDeviceContext->OMSetRenderTargets(1, &theRenderTarget, depthStencilView);

		D3D11_VIEWPORT viewport;
		viewport.Width = swapChainDesc.BufferDesc.Width;
		viewport.Height = swapChainDesc.BufferDesc.Height;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		theDeviceContext->RSSetViewports(1, &viewport);
	}
}