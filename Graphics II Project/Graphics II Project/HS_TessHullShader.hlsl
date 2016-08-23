// Input control point
struct VS_CONTROL_POINT_OUTPUT
{
	float4 position : POSITION;
	float2 uvs : UVS;
	float3 normals : NORMALS;
	float3 worldPos : WORLDPOS;
	float tessAmount : AMOUNT;
};

// Output control point, Output to Domain Shader
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

#define NUM_CONTROL_POINTS 3

// Patch Constant Function
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT Output;

	// Insert code to compute Output here
	Output.EdgeTessFactor[0] =
	Output.EdgeTessFactor[1] =
	Output.EdgeTessFactor[2] =
	Output.InsideTessFactor = ip[0].tessAmount; // e.g. could calculate dynamic tessellation factors instead

	return Output;
}

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
HS_CONTROL_POINT_OUTPUT main( 
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip, 
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
	HS_CONTROL_POINT_OUTPUT Output;

	// Insert code to compute Output here
	Output.position = ip[i].position;
	Output.uvs = ip[i].uvs;
	Output.normals = ip[i].normals;
	Output.worldPos = ip[i].worldPos;

	return Output;
}
