matrix World;
matrix View;
matrix Projection;

struct VertexInput
{
	float4 Position : Position;
	uint VertexID : SV_VertexID;
};

struct VertexOutput
{
	float4 Position : SV_Position;
	float3 Color : Color;
};

VertexOutput VS(VertexInput input)
{
	VertexOutput output;
	
	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);
	
	float3 colors[] =
	{
		float3(1, 0, 0),
		float3(0, 1, 0),
		float3(0, 0, 1),
		float3(0, 0, 0),
	};
	
	output.Color = colors[input.VertexID];
	
	return output;
}

RasterizerState FillMode_WireFrame
{
	FillMode = WireFrame;
};

float4 PS_Grid(VertexOutput input) : SV_Target
{
	return float4(input.Color, 1);
}

float4 PS_Cube(VertexOutput input) : SV_Target
{
	return float4(0, 1, 0, 1);
}

technique11 T0
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS_Grid()));
	}

	pass P1
	{
		SetRasterizerState(FillMode_WireFrame);

		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS_Grid()));
	}

	pass P2
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS_Cube()));
	}
}