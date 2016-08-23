#pragma once

#include<Windows.h>
#include"Direct3D.h"
#include"Star.h"
#include"BaseModel.h"
#include"Lighting.h"
#include"Terrain.h"
#include"Skybox.h"
#include"XTime.h"

// Globals
const float ZFar = 1000.0f;
const float ZNear = 0.1f;

// Safe Release for pointers
#define SAFE_RELEASE(p) {if(p){p->Release(); p = 0;}}

class GraphicsClass
{
public:

	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	bool Frame();
	void ShutDown();

	Direct3D* GetDirect3DPtr(void);
	Star* myStar;
	BaseModel* skyrimLogo;
	BaseModel* ground;
	BaseModel* tree;
	Skybox* mySkybox;
	Lighting* Lights;

private:

	bool Render();

private:

	Direct3D* Direct3DPtr;
};