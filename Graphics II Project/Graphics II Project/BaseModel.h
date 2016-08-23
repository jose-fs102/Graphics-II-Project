#pragma once


#include"Direct3D.h"
#include"XTime.h"
#include"ModelLoader.h"
#include"DDSTextureLoader.h"

#include "VS_BaseModelVertexShader.csh"
#include "PS_BaseModelPixelShader.csh"

#include "DS_TessDomainShader.csh"
#include "HS_TessHullShader.csh"


class BaseModel
{
public:

	unsigned int numVertices;
	unsigned int numIndices;
	unsigned int instanceCount;
	bool drawingInstanced;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* constantBuffer;

	ID3D11ShaderResourceView* shaderResource;
	ID3D11SamplerState* samplerState;
	ID3D11InputLayout* inputLayout;
	ID3D11RasterizerState* rasterState;

	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* Pixelshader;
	ID3D11HullShader* HullShader;
	ID3D11DomainShader* DomainShader;

	XMMATRIX modelWorldMatrix;

	struct MATRIX
	{
		XMMATRIX worldMatrix;
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;
		XMMATRIX viewMatrixNotInversed;
	};

	MATRIX constBufferData;

	ModelLoader* loader;
	XTime timer;

public:

	BaseModel();
	~BaseModel();

	bool Initialize(Direct3D*, const char *, const wchar_t*);
	void ShutDown();
	bool Render(Direct3D*, CameraClass*, bool = true);

	// Transformations
	void RotateModel_X(float);
	void RotateModel_Y(float);
	void RotateModel_Z(float);
	void TranslateModel(float, float, float);
	void TurnOnWireframe(Direct3D*);
	void TurnOffWireframe(Direct3D*);
};