#pragma pack_matrix(row_major)

struct INPUT
{
	float3 position : POSITION;
	float2 uvs : UVS;
	float3 normals : NORMALS;
};

struct OUTPUT
{
	float4 position : POSITION;
	float2 uvs : UVS;
	float3 normals : NORMALS;
	float3 worldPos : WORLDPOS;
	float tessAmount : AMOUNT;
};

cbuffer WORLD_DATA : register(b0)
{
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
	float4x4 cameraPos;
};

// Output to Hull Shader, Hull shader will pass to Domain shader
// and Domain will handle the math stuff previously handled here
OUTPUT main(INPUT input)
{
	OUTPUT sendToRasterizer = (OUTPUT)0;

	float4 localPosition = float4(input.position.xyz, 1);
	float4 anotherPosition = float4(input.position.xyz, 1);

	anotherPosition = mul(anotherPosition, worldMatrix);

	sendToRasterizer.position = localPosition;
	sendToRasterizer.uvs = input.uvs;
	sendToRasterizer.normals = input.normals;

	// Tessellation
	float3 obj = anotherPosition.xyz;
	float3 cam = cameraPos[3].xyz;

	float d = distance(obj, cam);

	float tess = 1 - saturate(d / 10.0f);

	sendToRasterizer.tessAmount = lerp(1, 64, tess);

	return sendToRasterizer;
}