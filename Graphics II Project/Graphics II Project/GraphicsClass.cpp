#include "GraphicsClass.h"


GraphicsClass::GraphicsClass()
{
	Direct3DPtr = 0;
	myStar = 0;
	skyrimLogo = 0;
	mySkybox = 0;
	ground = 0;
	tree = 0;
	Lights = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{

}

GraphicsClass::~GraphicsClass()
{
	// Cleanup happens in shutdown();
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	Direct3DPtr = new Direct3D;

	if (!Direct3DPtr)
		return false;

	result = Direct3DPtr->Initialize(screenWidth, screenHeight, hwnd, ZFar, ZNear);

	if (!result)
	{
		MessageBox(hwnd, L"ERROR, FAILED TO INITIALIZE DIRECT3D", L"ERROR", MB_OK);
		return false;
	}

	mySkybox = new Skybox;
	
	if (!mySkybox)
		return false;
	
	result = mySkybox->Initialize(Direct3DPtr, "Models/skyboxCube.obj", L"Textures/skybox_2_texture.dds");
	
	if (!result)
	{
		MessageBox(hwnd, L"ERROR, FAILED TO INITIALIZE SKYBOX", L"ERROR", MB_OK);
		return false;
	}

	myStar = new Star;
	if (!myStar)
		return false;
	
	result = myStar->Initialize(Direct3DPtr);
	
	if (!result)
	{
		MessageBox(hwnd, L"ERROR, FAILED TO INITIALIZE STAR", L"ERROR", MB_OK);
		return false;
	}
	
	myStar->TranslateModel(0.0f, 0.0f, 5.0f);
	myStar->RotateModel_X(360.0f);

	skyrimLogo = new BaseModel;
	if (!skyrimLogo)
		return false;
	
	result = skyrimLogo->Initialize(Direct3DPtr, "Models/skyrimlogo.obj", L"Textures/skyrimlogo_texture.dds");
	
	if (!result)
	{
		MessageBox(hwnd, L"ERROR, FAILED TO INITIALIZE MY BASE MODEL", L"ERROR", MB_OK);
		return false;
	}

	skyrimLogo->TranslateModel(0.0f, 0.0f, 5.0f);
	
	ground = new BaseModel;
	if (!ground)
		return false;
	
	result = ground->Initialize(Direct3DPtr, "Models/ground.obj", L"Textures/ground_texture.dds");
	
	if (!result)
	{
		MessageBox(hwnd, L"ERROR, FAILED TO INITIALIZE GROUND", L"ERROR", MB_OK);
		return false;
	}
	
	ground->TranslateModel(0.0f, -2.0f, 0.0f);

	tree = new BaseModel;
	if (!tree)
		return false;

	result = tree->Initialize(Direct3DPtr, "Models/alientree.obj", L"Textures/T_Tree_D.dds");

	if (!result)
	{
		MessageBox(hwnd, L"ERROR, FAILED TO INITIALIZE TREE", L"ERROR", MB_OK);
		return false;
	}

	tree->TranslateModel(5.0f, -1.0f, 0.0f);

	Lights = new Lighting;
	if (!Lights)
		return false;

	result = Lights->Initialize(Direct3DPtr);

	if (!result)
	{
		MessageBox(hwnd, L"ERROR, FAILED TO INITIALIZE LIGHTS", L"ERROR", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::ShutDown()
{
	if (Direct3DPtr)
	{
		Direct3DPtr->ShutDown();
		delete Direct3DPtr;
		Direct3DPtr = 0;
	}

	if (myStar)
	{
		myStar->ShutDown();
		delete myStar;
		myStar = 0;
	}

	if (skyrimLogo)
	{
		skyrimLogo->ShutDown();
		delete skyrimLogo;
		skyrimLogo = 0;
	}

	if (mySkybox)
	{
		mySkybox->ShutDown();
		delete mySkybox;
		mySkybox = 0;
	}

	if (ground)
	{
		ground->ShutDown();
		delete ground;
		ground = 0;
	}
	
	if (Lights)
	{
		Lights->Shutdown();
		delete Lights;
		Lights = 0;
	}

	if (tree)
	{
		tree->ShutDown();
		delete tree;
		tree = 0;
	}
	
	return;
}

Direct3D * GraphicsClass::GetDirect3DPtr(void)
{
	return Direct3DPtr;
}

bool GraphicsClass::Frame()
{
	bool result;

	result = Render();

	if (!result)
		return false;

	return true;
}

bool GraphicsClass::Render()
{
	// Clear screen to begin drawing
	Direct3DPtr->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Clear and render skybox
	Direct3DPtr->GetDeviceContext()->ClearDepthStencilView(Direct3DPtr->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	mySkybox->Render(Direct3DPtr, Direct3DPtr->GetCameraPtr());
	Direct3DPtr->GetDeviceContext()->ClearDepthStencilView(Direct3DPtr->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	
	// Rotate star and render 
	myStar->RotateModel_Z(2.0f);
	myStar->Render(Direct3DPtr, Direct3DPtr->GetCameraPtr());

	// Wireframe on
	if (GetAsyncKeyState('1') & 0x1)
		skyrimLogo->TurnOnWireframe(Direct3DPtr);

	// Wireframe off
	if (GetAsyncKeyState('2') & 0x1)
		skyrimLogo->TurnOffWireframe(Direct3DPtr);

	// Rotate model and render
	//skyrimLogo->RotateModel_Y(2.0f);
	skyrimLogo->Render(Direct3DPtr, Direct3DPtr->GetCameraPtr());
	
	// Render ground
	ground->Render(Direct3DPtr, Direct3DPtr->GetCameraPtr());
	
	// Render tree
	tree->Render(Direct3DPtr, Direct3DPtr->GetCameraPtr());

	// Render Lights
	Lights->Render(Direct3DPtr);

	// Present scene
	Direct3DPtr->EndScene();

	return true;
}