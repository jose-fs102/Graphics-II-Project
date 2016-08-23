#pragma pack_matrix(row_major)

struct INPUT
{
	float3 position : POSITION;
	float2 uvs : UVS;
	float3 normals : NORMALS;
};

struct OUTPUT
{
	float4 position : SV_POSITION;
	float3 uvs : UVS;
};

cbuffer WORLD_DATA : register(b0)
{
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
};

OUTPUT main(INPUT input)
{
	OUTPUT sendToRasterizer = (OUTPUT)0;

	float4 Position = float4(input.position.xyz, 1);

	Position = mul(Position, worldMatrix);
	Position = mul(Position, viewMatrix);
	Position = mul(Position, projectionMatrix);

	sendToRasterizer.position = Position;
	sendToRasterizer.uvs = input.position;

	return sendToRasterizer;
}