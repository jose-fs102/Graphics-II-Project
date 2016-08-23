#pragma once


#include"Direct3D.h"
#include"ModelLoader.h"
#include"XTime.h"
#include"DDSTextureLoader.h"

#include"VS_SkyboxVertexShader.csh"
#include"PS_SkyboxPixelShader.csh"

class Skybox
{
public:

	unsigned int numVertices;
	unsigned int numIndices;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* constantBuffer;

	ID3D11ShaderResourceView* shaderResource;
	ID3D11SamplerState* samplerState;
	ID3D11InputLayout* inputLayout;
	ID3D11RasterizerState* rasterState;

	ID3D11VertexShader* skyboxVertexShader;
	ID3D11PixelShader* skyboxPixelShader;

	struct MATRIX
	{
		XMMATRIX worldMatrix;
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;
	};

	MATRIX constBufferData;
	XMMATRIX skyboxWorldMatrix;
	ModelLoader* loader;
	XTime timer;

public:

	Skybox();
	Skybox(const Skybox&);
	~Skybox();

	bool Initialize(Direct3D*, const char *, const wchar_t*);
	void ShutDown();
	bool Render(Direct3D*, CameraClass*);
};