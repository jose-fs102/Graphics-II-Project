#pragma once

#include"Direct3D.h"
#include"XTime.h"

class Lighting
{
public:

	ID3D11Buffer* constantDirectionalLightBuffer;
	ID3D11Buffer* constantPointLightBuffer;

	D3D11_SUBRESOURCE_DATA subResourceData;
	D3D11_MAPPED_SUBRESOURCE mapSubResource;

	struct DIRECTIONAL_LIGHTING
	{
		XMFLOAT4 direction;
		XMFLOAT4 ambient;
		XMFLOAT4 color;
	};

	struct POINT_LIGHT
	{
		XMFLOAT4 point_position;
		XMFLOAT4 point_color;
		XMFLOAT3 attenuation;
		float point_range;
	};

	DIRECTIONAL_LIGHTING constDirLightData;
	POINT_LIGHT constPointLightData;

	bool pointIsRight = true;
	int pointMin = -50;
	int pointMax = 50;

	bool dirIsBack = true;
	int dirMin = -50;
	int dirMax = 50;

	XTime timer;
public:

	Lighting();
	Lighting(const Lighting&);
	~Lighting();

	bool Initialize(Direct3D*);
	void Shutdown();
	bool Render(Direct3D*);

	void TranslatePointLight(float, float, float);
	void RotateDirectionalLight(float x, float y, float z);
};