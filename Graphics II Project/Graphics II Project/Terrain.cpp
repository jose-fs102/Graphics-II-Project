#include "Terrain.h"


Terrain::Terrain()
{
	vertexBuffer = 0;
	indexBuffer = 0;
	constantMatrixBuffer = 0;
	inputLayout = 0;
	vertexShader = 0;
	pixelShader = 0;
	terrainWidth = 100;
	terrainHeight = 100;

	tVertex = 0;
	indices = 0;
}

Terrain::Terrain(const Terrain &other)
{

}

Terrain::~Terrain()
{
	// Shutdown()
}

bool Terrain::Initialize(Direct3D* D3DPtr)
{
	if (D3DPtr == NULL)
		return false;

	int index, i, j;
	float positionX, positionZ;

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc, constantMatrixBufferDesc;
	D3D11_SUBRESOURCE_DATA subResourceData;

	numVertices = (terrainWidth - 1) * (terrainHeight - 1) * 8;

	numIndices = numVertices;

	tVertex = new TerrainVertex[numVertices];
	indices = new unsigned long[numIndices];

	index = 0;

	for (j = 0; j < (terrainHeight - 1); j++)
	{
		for (i = 0; i < (terrainWidth - 1); i++)
		{
			// LINE 1
			// Upper left.
			positionX = (float)i;
			positionZ = (float)(j + 1);

			tVertex[index].position = { positionX, 0.0f, positionZ };
			tVertex[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			indices[index] = index;
			index++;

			// Upper right.
			positionX = (float)(i + 1);
			positionZ = (float)(j + 1);

			tVertex[index].position = { positionX, 0.0f, positionZ };
			tVertex[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			indices[index] = index;
			index++;

			// LINE 2
			// Upper right.
			positionX = (float)(i + 1);
			positionZ = (float)(j + 1);

			tVertex[index].position = { positionX, 0.0f, positionZ };
			tVertex[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			indices[index] = index;
			index++;

			// Bottom right.
			positionX = (float)(i + 1);
			positionZ = (float)j;

			tVertex[index].position = { positionX, 0.0f, positionZ };
			tVertex[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			indices[index] = index;
			index++;

			// LINE 3
			// Bottom right.
			positionX = (float)(i + 1);
			positionZ = (float)j;

			tVertex[index].position = { positionX, 0.0f, positionZ };
			tVertex[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			indices[index] = index;
			index++;

			// Bottom left.
			positionX = (float)i;
			positionZ = (float)j;

			tVertex[index].position = { positionX, 0.0f, positionZ };
			tVertex[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			indices[index] = index;
			index++;

			// LINE 4
			// Bottom left.
			positionX = (float)i;
			positionZ = (float)j;

			tVertex[index].position = { positionX, 0.0f, positionZ };
			tVertex[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			indices[index] = index;
			index++;

			// Upper left.
			positionX = (float)i;
			positionZ = (float)(j + 1);

			tVertex[index].position = { positionX, 0.0f, positionZ };
			tVertex[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			indices[index] = index;
			index++;
		}
	}

#pragma region VERTEX_BUFFER_SETUP

	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = NULL;
	vertexBufferDesc.ByteWidth = sizeof(TerrainVertex) * numVertices;

	subResourceData.pSysMem = tVertex;
	subResourceData.SysMemPitch = 0;
	subResourceData.SysMemSlicePitch = 0;

	HR(D3DPtr->GetDevice()->CreateBuffer(&vertexBufferDesc, &subResourceData, &vertexBuffer));

#pragma endregion

#pragma region INDEX_BUFFER_DESC

	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = NULL;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * numIndices;

	subResourceData.pSysMem = indices;

	HR(D3DPtr->GetDevice()->CreateBuffer(&indexBufferDesc, &subResourceData, &indexBuffer));

#pragma endregion

#pragma region CONSTANT_MATRIX_BUFFER_SETUP

	ZeroMemory(&constantMatrixBufferDesc, sizeof(constantMatrixBufferDesc));

	constantMatrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantMatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantMatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantMatrixBufferDesc.ByteWidth = sizeof(MATRIX);

	subResourceData.pSysMem = &constMatrixData;

	HR(D3DPtr->GetDevice()->CreateBuffer(&constantMatrixBufferDesc, &subResourceData, &constantMatrixBuffer));

#pragma endregion

#pragma region INPUT_LAYOUT_SETUP

	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	HR(D3DPtr->GetDevice()->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), (const void*)VS_TerrainShader, sizeof(VS_TerrainShader), &inputLayout));

#pragma endregion

	// Create Shaders
	HR(D3DPtr->GetDevice()->CreateVertexShader((void*)VS_TerrainShader, sizeof(VS_TerrainShader), NULL, &vertexShader));
	HR(D3DPtr->GetDevice()->CreatePixelShader((void*)PS_TerrainShader, sizeof(PS_TerrainShader), NULL, &pixelShader));

	// Set World matrix to Identity
	terrainWorldMatrix = XMMatrixIdentity();

	XMMATRIX scale = XMMatrixScaling(10.0f, 10.0f, 10.0f);
	XMMATRIX translation = XMMatrixTranslation(-100.0f, -100.0f, -100.0f);

	terrainWorldMatrix = scale * translation;

	return true;
}

void Terrain::Shutdown()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(constantMatrixBuffer);
	SAFE_RELEASE(inputLayout);
	SAFE_RELEASE(vertexShader);
	SAFE_RELEASE(pixelShader);

	if (tVertex)
	{
		delete[] tVertex;
		tVertex = 0;
	}

	if (indices)
	{
		delete[] indices;
		indices = 0;
	}
}

bool Terrain::Render(Direct3D *D3DPtr, CameraClass* cameraPtr)
{
	if (D3DPtr == NULL)
		return false;

#pragma region MAP_UNMAP

	D3D11_MAPPED_SUBRESOURCE mapSubResource;

	constMatrixData.worldMatix = terrainWorldMatrix;
	constMatrixData.viewMatix = cameraPtr->GetInverseViewMatrix();
	constMatrixData.projectionMatrix = cameraPtr->GetProjectionMatrix();

	HR(D3DPtr->GetDeviceContext()->Map(constantMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapSubResource));
	memcpy_s(mapSubResource.pData, sizeof(constMatrixData), &constMatrixData, sizeof(constMatrixData));
	D3DPtr->GetDeviceContext()->Unmap(constantMatrixBuffer, 0);

#pragma endregion

#pragma region INPUT_ASSEMBLER

	unsigned int strides = sizeof(TerrainVertex);
	unsigned int offsets = 0;

	D3DPtr->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
	D3DPtr->GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, offsets);
	D3DPtr->GetDeviceContext()->IASetInputLayout(inputLayout);
	D3DPtr->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

#pragma endregion

#pragma region VERTEX_SHADER

	D3DPtr->GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3DPtr->GetDeviceContext()->VSSetConstantBuffers(0, 1, &constantMatrixBuffer);

#pragma endregion

#pragma region PIXEL_SHADER

	D3DPtr->GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);

#pragma endregion

	D3DPtr->GetDeviceContext()->DrawIndexed(numIndices, 0, 0);

	return true;
}