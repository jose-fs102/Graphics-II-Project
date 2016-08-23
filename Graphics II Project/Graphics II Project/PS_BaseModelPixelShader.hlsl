texture2D tex2D : register(t0);
SamplerState sampleState : register(s0);

struct INPUT
{
	float4 position : SV_POSITION;
	float2 uvs : UVS;
	float3 normals : NORMALS;
	float3 worldPos : WORLDPOS;
};

cbuffer DIRECTIONAL_LIGHT : register(b0)
{
	float4 dir_direction;
	float4 dir_ambient;
	float4 dir_color;
}

cbuffer POINT_LIGHT : register(b1)
{
	float4 point_position;
	float4 point_color;
	float3 attenuation;
	float point_range;
}

float4 main(INPUT input) : SV_TARGET
{
	input.normals = normalize(input.normals);

	// Directional Lighting
	float4 surfaceColor = tex2D.Sample(sampleState, input.uvs);

	if (surfaceColor.a < 0.1f)
		discard;

	float4 directional_ambient = surfaceColor * dir_ambient;

	float directional_lightRatio = saturate(dot(dir_direction, input.normals));

	float4 directional_diffuse = directional_lightRatio * dir_color * surfaceColor;

	float4 directional_result = directional_ambient + directional_diffuse;

	// Point Lighting
	float3 lightDirection = normalize(point_position - input.worldPos);
	
	float point_lightRatio = saturate(dot(lightDirection, input.normals));

	float4 point_result = point_lightRatio * point_color * surfaceColor;

	float4 finalColor = point_result + directional_result;

	return finalColor;
}