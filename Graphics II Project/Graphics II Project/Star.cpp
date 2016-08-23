#include "Star.h"


Star::Star()
{
	vertexBuffer = 0;
	indexBuffer = 0;
	constantObjectBuffer = 0;
	constantSceneBuffer = 0;
	inputLayout = 0;
	vertexShader = 0;
	pixelShader = 0;
	myStar = 0;
	xTimer.Restart();
}

Star::Star(const Star& other)
{

}

Star::~Star()
{
	// Cleanup happens in shutdown()
}

bool Star::Initialize(Direct3D* D3DPtr)
{
	if (myStar == NULL)
		myStar = new StarVertex[12];

	if (D3DPtr == NULL)
		return false;

#pragma region STAR_SETUP

	float degree = 0.0f;
	vertCount = 0;

	myStar[0].x = 0.0f;
	myStar[0].y = 0.0f;
	myStar[0].z = -0.2f;
	myStar[0].w = 1.0f;

	myStar[0].r = 1.0f;
	myStar[0].g = 1.0f;
	myStar[0].b = 1.0f;
	myStar[0].a = 1.0f;

	vertCount++;

	myStar[1].x = 0.0f;
	myStar[1].y = 0.0f;
	myStar[1].z = 0.2f;
	myStar[1].w = 1.0f;

	myStar[1].r = 1.0f;
	myStar[1].g = 1.0f;
	myStar[1].b = 1.0f;
	myStar[1].a = 1.0f;

	vertCount++;

	for (int i = 2; i < 12; i++)
	{
		if (i % 2 != 0)
		{
			myStar[i].x = cosf((degree * PI) / 180.0f);
			myStar[i].y = sinf((degree * PI) / 180.0f);
			myStar[i].z = 0.0f;
			myStar[i].w = 1.0f;

			myStar[i].r = 1.0f;
			myStar[i].g = 0.0f;
			myStar[i].b = 1.0f;
			myStar[i].a = 1.0f;
		}
		else
		{
			myStar[i].x = cosf((degree * PI) / 180.0f) * 0.2f;
			myStar[i].y = sinf((degree * PI) / 180.0f) * 0.2f;
			myStar[i].z = 0.0f;
			myStar[i].w = 1.0f;

			myStar[i].r = 0.0f;
			myStar[i].g = 0.0f;
			myStar[i].b = 0.0f;
			myStar[i].a = 1.0f;
		}

		degree += 36.0f;
		vertCount++;
	}

	starVerts.clear();

	for (unsigned int i = 0; i < vertCount; i++)
		starVerts.push_back(myStar[i]);

	// Front Face
	indexBufferIndices.push_back(0);	indexBufferIndices.push_back(3);	indexBufferIndices.push_back(2);
	indexBufferIndices.push_back(0);	indexBufferIndices.push_back(4);	indexBufferIndices.push_back(3);
	indexBufferIndices.push_back(0);	indexBufferIndices.push_back(5);	indexBufferIndices.push_back(4);
	indexBufferIndices.push_back(0);	indexBufferIndices.push_back(6);	indexBufferIndices.push_back(5);
	indexBufferIndices.push_back(0);	indexBufferIndices.push_back(7);	indexBufferIndices.push_back(6);
	indexBufferIndices.push_back(0);	indexBufferIndices.push_back(8);	indexBufferIndices.push_back(7);
	indexBufferIndices.push_back(0);	indexBufferIndices.push_back(9);	indexBufferIndices.push_back(8);
	indexBufferIndices.push_back(0);	indexBufferIndices.push_back(10);	indexBufferIndices.push_back(9);
	indexBufferIndices.push_back(0);	indexBufferIndices.push_back(11);	indexBufferIndices.push_back(10);
	indexBufferIndices.push_back(0);	indexBufferIndices.push_back(2);	indexBufferIndices.push_back(11);

	// Back Face
	indexBufferIndices.push_back(1);	indexBufferIndices.push_back(2);	indexBufferIndices.push_back(3);
	indexBufferIndices.push_back(1);	indexBufferIndices.push_back(3);	indexBufferIndices.push_back(4);
	indexBufferIndices.push_back(1);	indexBufferIndices.push_back(4);	indexBufferIndices.push_back(5);
	indexBufferIndices.push_back(1);	indexBufferIndices.push_back(5);	indexBufferIndices.push_back(6);
	indexBufferIndices.push_back(1);	indexBufferIndices.push_back(6);	indexBufferIndices.push_back(7);
	indexBufferIndices.push_back(1);	indexBufferIndices.push_back(7);	indexBufferIndices.push_back(8);
	indexBufferIndices.push_back(1);	indexBufferIndices.push_back(8);	indexBufferIndices.push_back(9);
	indexBufferIndices.push_back(1);	indexBufferIndices.push_back(9);	indexBufferIndices.push_back(10);
	indexBufferIndices.push_back(1);	indexBufferIndices.push_back(10);	indexBufferIndices.push_back(11);
	indexBufferIndices.push_back(1);	indexBufferIndices.push_back(11);	indexBufferIndices.push_back(2);

#pragma endregion

#pragma region VERTEX_BUFFER_SETUP

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = NULL;
	vertexBufferDesc.ByteWidth = sizeof(StarVertex) * starVerts.size();

	D3D11_SUBRESOURCE_DATA subResourceData;

	subResourceData.pSysMem = starVerts.data();
	subResourceData.SysMemPitch = 0;
	subResourceData.SysMemSlicePitch = 0;

	HR(D3DPtr->GetDevice()->CreateBuffer(&vertexBufferDesc, &subResourceData, &vertexBuffer));

#pragma endregion

#pragma region INDEX_BUFFER_SETUP

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = NULL;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * indexBufferIndices.size();

	subResourceData.pSysMem = indexBufferIndices.data();

	HR(D3DPtr->GetDevice()->CreateBuffer(&indexBufferDesc, &subResourceData, &indexBuffer));

#pragma endregion

#pragma region CONSTANT_OBJECT_BUFFER_SETUP

	D3D11_BUFFER_DESC constantObjectBufferDesc;
	ZeroMemory(&constantObjectBufferDesc, sizeof(constantObjectBufferDesc));

	constantObjectBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantObjectBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantObjectBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantObjectBufferDesc.ByteWidth = sizeof(OBJECT_MATRIX);

	subResourceData.pSysMem = &worldMatrixObject;

	HR(D3DPtr->GetDevice()->CreateBuffer(&constantObjectBufferDesc, &subResourceData, &constantObjectBuffer));

#pragma endregion

#pragma region CONSTANT_SCENE_BUFFER_SETUP

	D3D11_BUFFER_DESC constantSceneBufferDesc;
	ZeroMemory(&constantSceneBufferDesc, sizeof(constantSceneBufferDesc));

	constantSceneBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantSceneBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantSceneBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantSceneBufferDesc.ByteWidth = sizeof(SCENE_MATRIX);

	subResourceData.pSysMem = &sceneMatrixObject;

	HR(D3DPtr->GetDevice()->CreateBuffer(&constantSceneBufferDesc, &subResourceData, &constantSceneBuffer));

#pragma endregion

#pragma region INPUT_LAYOUT_SETUP

	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HR(D3DPtr->GetDevice()->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), (const void*)VS_StarVertexShader, sizeof(VS_StarVertexShader), &inputLayout));

#pragma endregion

	// Create shaders
	HR(D3DPtr->GetDevice()->CreateVertexShader((void*)VS_StarVertexShader, sizeof(VS_StarVertexShader), NULL, &vertexShader));
	HR(D3DPtr->GetDevice()->CreatePixelShader((void*)PS_StarPixelShader, sizeof(PS_StarPixelShader), NULL, &pixelShader));

	// Set world matrix
	starWorldMatrix = XMMatrixIdentity();

	return true;
}

void Star::ShutDown()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(constantObjectBuffer);
	SAFE_RELEASE(constantSceneBuffer);
	SAFE_RELEASE(inputLayout);
	SAFE_RELEASE(vertexShader);
	SAFE_RELEASE(pixelShader);
}

bool Star::Render(Direct3D* D3DPtr, CameraClass* cameraPtr)
{
	if (D3DPtr == NULL)
		return false;

	if (cameraPtr == NULL)
		return false;

#pragma region MAP_AND_UNMAP_CONSTANT_BUFFERS

	D3D11_MAPPED_SUBRESOURCE mapSubResource;

	HR(D3DPtr->GetDeviceContext()->Map(constantObjectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapSubResource));
	memcpy_s(mapSubResource.pData, sizeof(starWorldMatrix), &starWorldMatrix, sizeof(starWorldMatrix));
	D3DPtr->GetDeviceContext()->Unmap(constantObjectBuffer, 0);

	SCENE_MATRIX localSceneMatrix;

	localSceneMatrix.viewMatrix = cameraPtr->GetInverseViewMatrix();
	localSceneMatrix.projectionMatrix = cameraPtr->GetProjectionMatrix();

	HR(D3DPtr->GetDeviceContext()->Map(constantSceneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapSubResource));
	memcpy_s(mapSubResource.pData, sizeof(localSceneMatrix), &localSceneMatrix, sizeof(localSceneMatrix));
	D3DPtr->GetDeviceContext()->Unmap(constantSceneBuffer, 0);

#pragma endregion

#pragma region INPUT_ASSEMBLER

	unsigned int strides = sizeof(StarVertex);
	unsigned int offsets = 0;

	D3DPtr->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
	D3DPtr->GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, offsets);
	D3DPtr->GetDeviceContext()->IASetInputLayout(inputLayout);
	D3DPtr->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

#pragma endregion

#pragma region VERTEX_SHADER

	D3DPtr->GetDeviceContext()->VSSetShader(vertexShader, NULL, 0);
	D3DPtr->GetDeviceContext()->VSSetConstantBuffers(0, 1, &constantObjectBuffer);
	D3DPtr->GetDeviceContext()->VSSetConstantBuffers(1, 1, &constantSceneBuffer);

#pragma endregion

#pragma region PIXEL_SHADER

	D3DPtr->GetDeviceContext()->PSSetShader(pixelShader, NULL, 0);

#pragma endregion

	// Draw Indexed
	D3DPtr->GetDeviceContext()->DrawIndexed(indexBufferIndices.size(), 0, 0);

	return true;
}

void Star::RotateModel_X(float angle)
{
	xTimer.Signal();
	starWorldMatrix = XMMatrixMultiply(XMMatrixRotationX(angle * xTimer.Delta()), starWorldMatrix);
}

void Star::RotateModel_Y(float angle)
{
	xTimer.Signal();
	starWorldMatrix = XMMatrixMultiply(XMMatrixRotationY(angle * xTimer.Delta()), starWorldMatrix);
}

void Star::RotateModel_Z(float angle)
{
	xTimer.Signal();
	starWorldMatrix = XMMatrixMultiply(XMMatrixRotationZ(angle * xTimer.Delta()), starWorldMatrix);
}

void Star::TranslateModel(float x, float y, float z)
{
	starWorldMatrix = XMMatrixTranslation(x, y, z);
}