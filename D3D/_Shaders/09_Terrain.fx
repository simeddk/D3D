#include "00_Global.fx"

float3 LightDirection;
Texture2D BaseMap;

struct VertexInput
{
	float4 Position : Position;
	float3 Normal : Normal;
	float2 Uv : Uv;
};

struct VertexOutput
{
	float4 Position : SV_Position;
	float3 Normal : Normal;
	float2 Uv : Uv;
};

VertexOutput VS(VertexInput input)
{
	VertexOutput output;
	
	output.Position = WorldPosition(input.Position);
	output.Position = ViewProjection(output.Position);
	output.Normal = WorldNormal(input.Normal);
	
	output.Uv = input.Uv;
	
	return output;
}


uint Mode;
float4 PS(VertexOutput input) : SV_Target
{
	float3 normal = normalize(input.Normal);
	float lambert = dot(normal, -LightDirection);
	float4 color = BaseMap.Sample(LinearSampler, input.Uv);
	
	if (Mode == 1)
		return color;
		
	return color * lambert;
}

technique11 T0
{
	P_VP(P0, VS, PS)
}