#pragma pack_matrix(row_major)

struct INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
};

struct OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

cbuffer MATRIX_DATA : register(b0)
{
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
}

OUTPUT main(INPUT input)
{
	OUTPUT sendToRasterizer = (OUTPUT)0;

	float4 localPosition = float4(input.position.xyz, 1);

	localPosition = mul(localPosition, worldMatrix);
	localPosition = mul(localPosition, viewMatrix);
	localPosition = mul(localPosition, projectionMatrix);

	sendToRasterizer.position = localPosition;
	sendToRasterizer.color = input.color;

	return sendToRasterizer;
}