#include "Lighting.h"


Lighting::Lighting()
{
	constantDirectionalLightBuffer = 0;
	constantPointLightBuffer = 0;

	timer.Restart();
}

Lighting::Lighting(const Lighting &other)
{

}

Lighting::~Lighting()
{

}

bool Lighting::Initialize(Direct3D *D3Dptr)
{
	if (D3Dptr == NULL)
		return false;

#pragma region CONSTANT_DIRECTIONAL_LIGHT_BUFFER_SETUP

	D3D11_BUFFER_DESC constantDirectionalLightBufferDesc;
	ZeroMemory(&constantDirectionalLightBufferDesc, sizeof(constantDirectionalLightBufferDesc));

	constantDirectionalLightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantDirectionalLightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantDirectionalLightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantDirectionalLightBufferDesc.ByteWidth = sizeof(DIRECTIONAL_LIGHTING);

	subResourceData.pSysMem = &constDirLightData;
	subResourceData.SysMemPitch = 0;
	subResourceData.SysMemSlicePitch = 0;

	HR(D3Dptr->GetDevice()->CreateBuffer(&constantDirectionalLightBufferDesc, &subResourceData, &constantDirectionalLightBuffer));

#pragma endregion

#pragma region CONSTANT_POINT_LIGHT_BUFFER_SETUP

	D3D11_BUFFER_DESC constantPointLightBufferDesc;
	ZeroMemory(&constantPointLightBufferDesc, sizeof(constantPointLightBufferDesc));

	constantPointLightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantPointLightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantPointLightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantPointLightBufferDesc.ByteWidth = sizeof(POINT_LIGHT);

	subResourceData.pSysMem = &constPointLightData;

	HR(D3Dptr->GetDevice()->CreateBuffer(&constantPointLightBufferDesc, &subResourceData, &constantPointLightBuffer));

#pragma endregion

	// Directional Lighting
	constDirLightData.direction = { 0.25f, 0.5f, -1.0f, 1.0f };
	constDirLightData.ambient = { 0.0f, 0.0f, 0.0f, 1.0f };
	constDirLightData.color = { 0.0f, 0.0f, 1.0f, 1.0f };

	// Point Light
	constPointLightData.point_color = { 1.0f, 0.0f, 0.0f, 1.0f };
	constPointLightData.point_position = { 0.0f, 10.0f, 0.0f, 1.0f };

	return true;
}

void Lighting::Shutdown()
{
	SAFE_RELEASE(constantDirectionalLightBuffer);
	SAFE_RELEASE(constantPointLightBuffer);
}

bool Lighting::Render(Direct3D *D3Dptr)
{
	if (D3Dptr == NULL)
		return false;

	// Translate the point light
	if (constPointLightData.point_position.x < pointMax && pointIsRight == true)
		TranslatePointLight(0.02f, 0.0f, 0.0f);
	else
		pointIsRight = false;

	if (constPointLightData.point_position.x > pointMin && pointIsRight == false)
		TranslatePointLight(-0.02f, 0.0f, 0.0f);
	else
		pointIsRight = true;

	RotateDirectionalLight(0.0f, 45.0f, 0.0f);

	HR(D3Dptr->GetDeviceContext()->Map(constantDirectionalLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapSubResource));
	memcpy_s(mapSubResource.pData, sizeof(constDirLightData), &constDirLightData, sizeof(constDirLightData));
	D3Dptr->GetDeviceContext()->Unmap(constantDirectionalLightBuffer, 0);

	HR(D3Dptr->GetDeviceContext()->Map(constantPointLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapSubResource));
	memcpy_s(mapSubResource.pData, sizeof(constPointLightData), &constPointLightData, sizeof(constPointLightData));
	D3Dptr->GetDeviceContext()->Unmap(constantPointLightBuffer, 0);

	D3Dptr->GetDeviceContext()->PSSetConstantBuffers(0, 1, &constantDirectionalLightBuffer);
	D3Dptr->GetDeviceContext()->PSSetConstantBuffers(1, 1, &constantPointLightBuffer);

	return true;
}

void Lighting::TranslatePointLight(float x, float y, float z)
{
	XMMATRIX temp;

	temp.r[3].m128_f32[0] = constPointLightData.point_position.x;
	temp.r[3].m128_f32[1] = constPointLightData.point_position.y;
	temp.r[3].m128_f32[2] = constPointLightData.point_position.z;
	temp.r[3].m128_f32[3] = constPointLightData.point_position.w;

	temp = XMMatrixMultiply(temp, XMMatrixTranslation(x, y, z));

	constPointLightData.point_position.x = temp.r[3].m128_f32[0];
	constPointLightData.point_position.y = temp.r[3].m128_f32[1];
	constPointLightData.point_position.z = temp.r[3].m128_f32[2];
	constPointLightData.point_position.w = temp.r[3].m128_f32[3];
}

void Lighting::RotateDirectionalLight(float x, float y, float z)
{
	timer.Signal();

	XMMATRIX temp;

	temp.r[2].m128_f32[0] = constDirLightData.direction.x;
	temp.r[2].m128_f32[1] = constDirLightData.direction.y;
	temp.r[2].m128_f32[2] = constDirLightData.direction.z;
	temp.r[2].m128_f32[3] = constDirLightData.direction.w;

	temp = XMMatrixMultiply(temp, XMMatrixRotationX(XMConvertToRadians(x * timer.Delta())));
	temp = XMMatrixMultiply(temp, XMMatrixRotationY(XMConvertToRadians(y * timer.Delta())));
	temp = XMMatrixMultiply(temp, XMMatrixRotationZ(XMConvertToRadians(z * timer.Delta())));

	constDirLightData.direction.x = temp.r[2].m128_f32[0];
	constDirLightData.direction.y = temp.r[2].m128_f32[1];
	constDirLightData.direction.z = temp.r[2].m128_f32[2];
	constDirLightData.direction.w = temp.r[2].m128_f32[3];
}