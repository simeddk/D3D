matrix World, View, Projection;
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
	
	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);
	
	output.Normal = mul(input.Normal, (float3x3)World);
	
	output.Uv = input.Uv;
	
	return output;
}

SamplerState LinearSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

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
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}