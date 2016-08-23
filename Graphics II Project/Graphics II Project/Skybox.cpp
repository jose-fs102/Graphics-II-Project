#include "Skybox.h"


Skybox::Skybox()
{
	vertexBuffer = 0;
	indexBuffer = 0;
	constantBuffer = 0;
	rasterState = 0;
	shaderResource = 0;
	samplerState = 0;
	inputLayout = 0;
	skyboxVertexShader = 0;
	skyboxPixelShader = 0;

	loader = 0;

	timer.Restart();
}

Skybox::Skybox(const Skybox &)
{
}

Skybox::~Skybox()
{
}

bool Skybox::Initialize(Direct3D *D3Dptr, const char *filename, const wchar_t *textureName)
{
	if (D3Dptr == NULL)
		return false;

	if (loader == NULL)
		loader = new ModelLoader;

	if (!loader)
		return false;

#pragma region LOAD_MODEL

	std::vector<VertexFormat> vertices;
	vertices.clear();

	std::vector<unsigned int> indices;
	indices.clear();

	loader->LoadOBJModel(filename, vertices, indices);

	numVertices = vertices.size();
	numIndices = indices.size();

#pragma endregion

#pragma region VERTEX_BUFFER_SETUP

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = NULL;
	vertexBufferDesc.ByteWidth = sizeof(VertexFormat) * vertices.size();

	D3D11_SUBRESOURCE_DATA subResourceData;

	subResourceData.pSysMem = vertices.data();
	subResourceData.SysMemPitch = 0;
	subResourceData.SysMemSlicePitch = 0;

	HR(D3Dptr->GetDevice()->CreateBuffer(&vertexBufferDesc, &subResourceData, &vertexBuffer));

#pragma endregion

#pragma region INDEX_BUFFER_SETUP

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = NULL;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * indices.size();

	subResourceData.pSysMem = indices.data();

	HR(D3Dptr->GetDevice()->CreateBuffer(&indexBufferDesc, &subResourceData, &indexBuffer));

#pragma endregion

#pragma region CONSTANT_BUFFER_SETUP

	D3D11_BUFFER_DESC constantBufferDesc;
	ZeroMemory(&constantBufferDesc, sizeof(constantBufferDesc));

	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.ByteWidth = sizeof(MATRIX);

	subResourceData.pSysMem = &constBufferData;

	HR(D3Dptr->GetDevice()->CreateBuffer(&constantBufferDesc, &subResourceData, &constantBuffer));

#pragma endregion

#pragma region INPUT_LAYOUT_SETUP

	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UVS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMALS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	HR(D3Dptr->GetDevice()->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), (const void*)VS_SkyboxVertexShader, sizeof(VS_SkyboxVertexShader), &inputLayout));

#pragma endregion

#pragma region SAMPLER_STATE_SETUP

	// Create Sampler State
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MinLOD = (-FLT_MAX);
	samplerDesc.MaxLOD = (FLT_MAX);
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 1.0f;
	samplerDesc.BorderColor[2] = 1.0f;
	samplerDesc.BorderColor[3] = 1.0f;

	HR(D3Dptr->GetDevice()->CreateSamplerState(&samplerDesc, &samplerState));

#pragma endregion

#pragma region RASTERIZER_STATE

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));

	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.ScissorEnable = FALSE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.AntialiasedLineEnable = FALSE;

	HR(D3Dptr->GetDevice()->CreateRasterizerState(&rasterizerDesc, &rasterState));

#pragma endregion

	// Create Textures and Shader Resource
	CreateDDSTextureFromFile(D3Dptr->GetDevice(), textureName, NULL, &shaderResource, 0);

	// Create Shader(s)
	HR(D3Dptr->GetDevice()->CreateVertexShader((void*)VS_SkyboxVertexShader, sizeof(VS_SkyboxVertexShader), NULL, &skyboxVertexShader));
	HR(D3Dptr->GetDevice()->CreatePixelShader((void*)PS_SkyboxPixelShader, sizeof(PS_SkyboxPixelShader), NULL, &skyboxPixelShader));

	// Set World matrix to Identity
	skyboxWorldMatrix = XMMatrixIdentity();

	return true;
}

void Skybox::ShutDown()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(constantBuffer);
	SAFE_RELEASE(shaderResource);
	SAFE_RELEASE(samplerState);
	SAFE_RELEASE(inputLayout);
	SAFE_RELEASE(rasterState);
	SAFE_RELEASE(skyboxVertexShader);
	SAFE_RELEASE(skyboxPixelShader);
}

bool Skybox::Render(Direct3D *D3Dptr, CameraClass *cameraPtr)
{
	if (D3Dptr == NULL)
		return false;

#pragma region MAP_UNMAP

	// Map constant buffer
	D3D11_MAPPED_SUBRESOURCE mapSubResource;

	constBufferData.viewMatrix = cameraPtr->GetInverseViewMatrix();
	constBufferData.projectionMatrix = cameraPtr->GetProjectionMatrix();
	skyboxWorldMatrix.r[3] = cameraPtr->GetViewMatrix().r[3];
	constBufferData.worldMatrix = skyboxWorldMatrix;

	HR(D3Dptr->GetDeviceContext()->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapSubResource));
	memcpy_s(mapSubResource.pData, sizeof(constBufferData), &constBufferData, sizeof(constBufferData));
	D3Dptr->GetDeviceContext()->Unmap(constantBuffer, 0);

#pragma endregion

#pragma region INPUT_ASSEMBLER

	unsigned int strides = sizeof(VertexFormat);
	unsigned int offsets = 0;

	D3Dptr->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
	D3Dptr->GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, offsets);
	D3Dptr->GetDeviceContext()->IASetInputLayout(inputLayout);
	D3Dptr->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

#pragma endregion

#pragma region VERTEX_SHADER

	D3Dptr->GetDeviceContext()->VSSetShader(skyboxVertexShader, NULL, 0);
	D3Dptr->GetDeviceContext()->VSSetConstantBuffers(0, 1, &constantBuffer);

#pragma endregion

#pragma region RASTERIZER

	D3Dptr->GetDeviceContext()->RSSetState(rasterState);

#pragma endregion

#pragma region PIXEL_SHADER

	D3Dptr->GetDeviceContext()->PSSetShader(skyboxPixelShader, NULL, 0);
	D3Dptr->GetDeviceContext()->PSSetSamplers(0, 1, &samplerState);
	D3Dptr->GetDeviceContext()->PSSetShaderResources(0, 1, &shaderResource);

#pragma endregion

	// Draw Indexed
	D3Dptr->GetDeviceContext()->DrawIndexed(numIndices, 0, 0);

	return true;
}