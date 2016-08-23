TextureCube Tcube : register(t0);
SamplerState sampleState : register(s0);

struct INPUT
{
	float4 position : SV_POSITION;
	float3 uvs : UVS;
};

float4 main(INPUT input) : SV_TARGET
{
	return float4(Tcube.Sample(sampleState, input.uvs));
}