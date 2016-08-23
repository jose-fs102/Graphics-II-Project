#pragma once

#include"Direct3D.h"
#include<fstream>	
#include<vector>

#include"VS_TerrainShader.csh"
#include"PS_TerrainShader.csh"


class Terrain
{
public:

	struct TerrainVertex
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};
	
	TerrainVertex* tVertex;
	unsigned long* indices;

	struct MATRIX
	{
		XMMATRIX worldMatix;
		XMMATRIX viewMatix;
		XMMATRIX projectionMatrix;
	};

	MATRIX constMatrixData;

	int terrainWidth;
	int terrainHeight;
	int numVertices;
	int numIndices;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* constantMatrixBuffer;

	ID3D11InputLayout* inputLayout;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	XMMATRIX terrainWorldMatrix;

public:

	Terrain();
	Terrain(const Terrain&);
	~Terrain();

	bool Initialize(Direct3D*);
	void Shutdown();
	bool Render(Direct3D*, CameraClass*);
};