#pragma once

#include"Direct3D.h"
#include<vector>	
#include"XTime.h"

#include"VS_StarVertexShader.csh"
#include"PS_StarPixelShader.csh"
 
using namespace std;

class Star
{
public:

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* constantObjectBuffer;
	ID3D11Buffer* constantSceneBuffer;
	ID3D11InputLayout* inputLayout;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	XMMATRIX starWorldMatrix;
	XTime xTimer;

public:

	Star();
	Star(const Star&);
	~Star();

	bool Initialize(Direct3D*);
	void ShutDown();
	bool Render(Direct3D*, CameraClass*);

	struct StarVertex
	{
		union
		{
			float position[4];

			struct
			{
				float x, y, z, w;
			};
		};

		union
		{
			float color[4];

			struct
			{
				float r, g, b, a;
			};
		};
	};

	StarVertex* myStar;
	vector<StarVertex> starVerts;
	vector<unsigned int> indexBufferIndices;
	unsigned int vertCount;

	struct OBJECT_MATRIX
	{
		XMMATRIX worldMatrix;
	};

	OBJECT_MATRIX worldMatrixObject;

	struct SCENE_MATRIX
	{
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;
	};

	SCENE_MATRIX sceneMatrixObject;

	void RotateModel_X(float);
	void RotateModel_Y(float);
	void RotateModel_Z(float);

	void TranslateModel(float, float, float);
};