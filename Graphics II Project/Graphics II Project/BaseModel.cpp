#include "BaseModel.h"


BaseModel::BaseModel()
{
	vertexBuffer = 0;
	indexBuffer = 0;
	constantBuffer = 0;
	rasterState = 0;
	shaderResource = 0;
	samplerState = 0;
	inputLayout = 0;
	VertexShader = 0;
	Pixelshader = 0;
	HullShader = 0;
	DomainShader = 0;

	loader = 0;

	timer.Restart();
}

BaseModel::~BaseModel()
{
	// cleanup happens in shutdown()
}

bool BaseModel::Initialize(Direct3D* D3Dptr, const char * filename, const wchar_t* textureName)
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

#pragma region RASTER_STATE_SETUP

	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));

	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.FrontCounterClockwise = FALSE;
	rasterDesc.DepthBias = 0;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = TRUE;
	rasterDesc.ScissorEnable = FALSE;
	rasterDesc.MultisampleEnable = FALSE;
	rasterDesc.AntialiasedLineEnable = FALSE;

	HR(D3Dptr->GetDevice()->CreateRasterizerState(&rasterDesc, &rasterState));

#pragma endregion

#pragma region INPUT_LAYOUT_SETUP

	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UVS", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMALS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	HR(D3Dptr->GetDevice()->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), (const void*)VS_BaseModelVertexShader, sizeof(VS_BaseModelVertexShader), &inputLayout));

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

	// Create Textures and Shader Resource
	HR(CreateDDSTextureFromFile(D3Dptr->GetDevice(), textureName, 0, &shaderResource, 0));

	// Create Shaders
	HR(D3Dptr->GetDevice()->CreateVertexShader((void*)VS_BaseModelVertexShader, sizeof(VS_BaseModelVertexShader), NULL, &VertexShader));
	HR(D3Dptr->GetDevice()->CreatePixelShader((void*)PS_BaseModelPixelShader, sizeof(PS_BaseModelPixelShader), NULL, &Pixelshader));
	HR(D3Dptr->GetDevice()->CreateHullShader((void*)HS_TessHullShader, sizeof(HS_TessHullShader), NULL, &HullShader));
	HR(D3Dptr->GetDevice()->CreateDomainShader((void*)DS_TessDomainShader, sizeof(DS_TessDomainShader), NULL, &DomainShader));

	// Set World matrix to Identity
	modelWorldMatrix = XMMatrixIdentity();

	return true;
}

void BaseModel::ShutDown()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(constantBuffer);
	SAFE_RELEASE(shaderResource);
	SAFE_RELEASE(rasterState);
	SAFE_RELEASE(samplerState);
	SAFE_RELEASE(inputLayout);
	SAFE_RELEASE(VertexShader);
	SAFE_RELEASE(Pixelshader);
	SAFE_RELEASE(HullShader);
	SAFE_RELEASE(DomainShader);
}

bool BaseModel::Render(Direct3D* D3Dptr, CameraClass* cameraPtr, bool tessellate)
{
	if (D3Dptr == NULL)
		return false;

	if (cameraPtr == NULL)
		return false;

#pragma region MAP_UNMAP

	D3D11_MAPPED_SUBRESOURCE mapSubResource;

	// Matrix Data
	constBufferData.viewMatrix = cameraPtr->GetInverseViewMatrix();
	constBufferData.projectionMatrix = cameraPtr->GetProjectionMatrix();
	constBufferData.worldMatrix = modelWorldMatrix;
	constBufferData.viewMatrixNotInversed = cameraPtr->GetViewMatrix();

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
	D3Dptr->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

#pragma endregion

#pragma region VERTEX_SHADER

	D3Dptr->GetDeviceContext()->VSSetShader(VertexShader, NULL, 0);
	D3Dptr->GetDeviceContext()->VSSetConstantBuffers(0, 1, &constantBuffer);

#pragma endregion

#pragma region HULL_SHADER

	D3Dptr->GetDeviceContext()->HSSetShader(HullShader, NULL, 0);

#pragma endregion

#pragma region DOMAIN_SHADER

	D3Dptr->GetDeviceContext()->DSSetShader(DomainShader, NULL, 0);
	D3Dptr->GetDeviceContext()->DSSetConstantBuffers(0, 1, &constantBuffer);

#pragma endregion

#pragma region RASTERIZER

	D3Dptr->GetDeviceContext()->RSSetState(rasterState);

#pragma endregion

#pragma region PIXEL_SHADER

	D3Dptr->GetDeviceContext()->PSSetShader(Pixelshader, NULL, 0);
	D3Dptr->GetDeviceContext()->PSSetSamplers(0, 1, &samplerState);
	D3Dptr->GetDeviceContext()->PSSetShaderResources(0, 1, &shaderResource);

#pragma endregion

	// Draw
	D3Dptr->GetDeviceContext()->DrawIndexed(numIndices, 0, 0);

	// Set tessellation shaders and state to null
	D3Dptr->GetDeviceContext()->HSSetShader(NULL, NULL, NULL);

	D3Dptr->GetDeviceContext()->DSSetShader(NULL, NULL, NULL);

	D3Dptr->GetDeviceContext()->RSSetState(NULL);

	return true;
}

void BaseModel::RotateModel_X(float angle)
{
	timer.Signal();

	modelWorldMatrix = XMMatrixMultiply(XMMatrixRotationX(angle * timer.Delta()), modelWorldMatrix);
}

void BaseModel::RotateModel_Y(float angle)
{
	timer.Signal();

	modelWorldMatrix = XMMatrixMultiply(XMMatrixRotationY(angle * timer.Delta()), modelWorldMatrix);
}

void BaseModel::RotateModel_Z(float angle)
{
	timer.Signal();

	modelWorldMatrix = XMMatrixMultiply(XMMatrixRotationZ(angle * timer.Delta()), modelWorldMatrix);
}

void BaseModel::TranslateModel(float x, float y, float z)
{
	modelWorldMatrix = XMMatrixTranslation(x, y, z);
}

void BaseModel::TurnOnWireframe(Direct3D* D3Dptr)
{
	if (rasterState)
		SAFE_RELEASE(rasterState);

	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));

	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.FrontCounterClockwise = FALSE;
	rasterDesc.DepthBias = 0;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = TRUE;
	rasterDesc.ScissorEnable = FALSE;
	rasterDesc.MultisampleEnable = FALSE;
	rasterDesc.AntialiasedLineEnable = FALSE;

	HR(D3Dptr->GetDevice()->CreateRasterizerState(&rasterDesc, &rasterState));
}

void BaseModel::TurnOffWireframe(Direct3D* D3Dptr)
{
	if (rasterState)
		SAFE_RELEASE(rasterState);

	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(rasterDesc));

	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.FrontCounterClockwise = FALSE;
	rasterDesc.DepthBias = 0;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = TRUE;
	rasterDesc.ScissorEnable = FALSE;
	rasterDesc.MultisampleEnable = FALSE;
	rasterDesc.AntialiasedLineEnable = FALSE;

	HR(D3Dptr->GetDevice()->CreateRasterizerState(&rasterDesc, &rasterState));
}