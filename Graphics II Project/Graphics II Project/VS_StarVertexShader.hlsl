#pragma pack_matrix(row_major)

struct INPUT_VERTEX
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct OUTPUT_VERTEX
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

cbuffer OBJECT : register (b0)
{
	float4x4 worldMatrix;
}

cbuffer SCENE : register(b1)
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
}

OUTPUT_VERTEX main(INPUT_VERTEX input)
{
	OUTPUT_VERTEX sendToRasterizer = (OUTPUT_VERTEX)0;

	float4 localArray = input.position;

	localArray = mul(localArray, worldMatrix);
	localArray = mul(localArray, viewMatrix);
	localArray = mul(localArray, projectionMatrix);

	sendToRasterizer.position = localArray;

	sendToRasterizer.color = input.color;

	return sendToRasterizer;
}