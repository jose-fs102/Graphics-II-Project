#pragma pack_matrix(row_major)

// Output to pixel shader
struct DS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uvs : UVS;
	float3 normals : NORMALS;
	float3 worldPos : WORLDPOS;
	float tessAmount : AMOUNT;
};

// Output control point
struct HS_CONTROL_POINT_OUTPUT
{
	float4 position : POSITION;
	float2 uvs : UVS;
	float3 normals : NORMALS;
	float3 worldPos : WORLDPOS;
	float tessAmount : AMOUNT;
};

// Output patch constant data.
struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor			: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
	// TODO: change/add other stuff
};

// Matrix constant buffer
cbuffer MATRIX_BUFFER : register(b0)
{
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
};

#define NUM_CONTROL_POINTS 3

[domain("tri")]
DS_OUTPUT main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
	DS_OUTPUT Output;

	float3 vertexPosition = domain.x * patch[0].position + domain.y * patch[1].position + domain.z * patch[2].position;
	float2 vertexUV = domain.x * patch[0].uvs.xy + domain.y * patch[1].uvs.xy +domain.z * patch[2].uvs.xy;

	Output.position = mul(float4(vertexPosition, 1), worldMatrix);
	Output.worldPos = Output.position.xyz;

	Output.position = mul(Output.position, viewMatrix);
	Output.position = mul(Output.position, projectionMatrix);

	Output.uvs = vertexUV.xy;
	Output.normals = mul(worldMatrix, patch[0].normals);

	return Output;
}