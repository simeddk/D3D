matrix World;
matrix View;
matrix Projection;

Texture2D BaseMap;

struct VertexInput
{
	float4 Position : Position;
	float2 Uv : Uv;
};

struct VertexOutput
{
	float4 Position : SV_Position;
	float2 Uv : Uv;
};

VertexOutput VS(VertexInput input)
{
	VertexOutput output;
	
	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);
	
	output.Uv = input.Uv;
	
	return output;
}

RasterizerState FillMode_WireFrame
{
	FillMode = WireFrame;
};


SamplerState WrapSampler
{
	AddressU = Wrap;
	AddressV = Wrap;
};

SamplerState MirrorSampler
{
	AddressU = Mirror;
	AddressV = Mirror;
};

SamplerState ClampSampler
{
	AddressU = Clamp;
	AddressV = Clamp;
};

SamplerState BorderSampler
{
	AddressU = Border;
	AddressV = Border;

	BorderColor = float4(0, 1, 1, 1);
};

uint Address;
float4 PS_Address(VertexOutput input) : SV_Target
{
	if (Address == 0)
		return BaseMap.Sample(WrapSampler, input.Uv);
	
	if (Address == 1)
		return BaseMap.Sample(MirrorSampler, input.Uv);
	
	if (Address == 2)
		return BaseMap.Sample(ClampSampler, input.Uv);
	
	if (Address == 3)
		return BaseMap.Sample(BorderSampler, input.Uv);
	
	return BaseMap.Sample(WrapSampler, input.Uv);
}

SamplerState PointSampler
{
	Filter = MIN_MAG_MIP_POINT;
};

SamplerState LinearSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
};


uint Filter;
float4 PS_Filter(VertexOutput input) : SV_Target
{
	if (Filter == 0)
		return BaseMap.Sample(PointSampler, input.Uv);
	
	if (Filter == 1)
		return BaseMap.Sample(LinearSampler, input.Uv);
	
	return BaseMap.Sample(PointSampler, input.Uv);
}

technique11 T0
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS_Address()));
	}

	pass P1
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS_Filter()));
	}
}