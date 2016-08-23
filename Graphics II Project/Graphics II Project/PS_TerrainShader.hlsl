struct INPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

float4 main(INPUT input) : SV_TARGET
{
	return input.color;
}