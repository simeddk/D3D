matrix World;
matrix View;
matrix Projection;

struct VertexInput
{
	float4 Position : Position;
};

struct VertexOutput
{
	float4 Position : SV_Position;
};

VertexOutput VS(VertexInput input)
{
	VertexOutput output;
	
	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);
	
	return output;
}

RasterizerState FillMode_WireFrame
{
	FillMode = WireFrame;
};

float4 PS(VertexOutput input) : SV_Target
{
	return float4(0, 1, 0, 1);
}

//-------------------------------------------------------------------
//Height Color
//-------------------------------------------------------------------
struct VertexOutput_HeightColor
{
	float4 Position : SV_Position;
	float4 Color : Color;
	float3 wPosition : Position1;
};

float4 GetHeightColor(float y)
{
	if (y > 20.f)
		return float4(1, 1, 1, 1);
	
	if (y > 10.f)
		return float4(0, 1, 1, 1);
	
	if (y > 5.f)
		return float4(0, 1, 0, 1);

	return float4(0, 0.3, 0, 1);
}

VertexOutput_HeightColor VS_HeightColor(VertexInput input)
{
	VertexOutput_HeightColor output;
	
	output.Position = mul(input.Position, World);
	output.wPosition = output.Position.xyz;
	//output.Color = GetHeightColor(output.Position.y);
	
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);
	
	return output;
}

float4 PS_HeightColor(VertexOutput_HeightColor input) : SV_Target
{
	return GetHeightColor(input.wPosition.y);
}

//-------------------------------------------------------------------
//Lamber
//-------------------------------------------------------------------
struct VertexInput_Lambert
{
	float4 Position : Position;
	float3 Normal : Normal;
};

struct VertexOutput_Lambert
{
	float4 Position : SV_Position;
	float4 Color : Color;
	float3 wPosition : Position1;
	float3 Normal : Normal;
};

VertexOutput_Lambert VS_Lambert(VertexInput_Lambert input)
{
	VertexOutput_Lambert output;
	
	output.Position = mul(input.Position, World);
	output.wPosition = output.Position.xyz;
	
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);
	
	output.Normal = mul(input.Normal, (float3x3)World);
	
	return output;
}

float3 LightDirection;
float4 PS_Lambert(VertexOutput_Lambert input) : SV_Target
{
	float3 normal = normalize(input.Normal);
	float4 color = GetHeightColor(input.wPosition.y);
	float lambert = dot(normal, -LightDirection);
	
	return color * lambert;
}

float4 PS_HalfLambert(VertexOutput_Lambert input) : SV_Target
{
	float3 normal = normalize(input.Normal);
	float4 color = GetHeightColor(input.wPosition.y);
	float lambert = dot(normal, -LightDirection) * 0.5f + 0.5f;
	
	return color * lambert;
}

technique11 T0
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}

	pass P1
	{
		SetRasterizerState(FillMode_WireFrame);

		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}

	pass P2
	{
		SetVertexShader(CompileShader(vs_5_0, VS_HeightColor()));
		SetPixelShader(CompileShader(ps_5_0, PS_HeightColor()));
	}

	pass P3
	{
		SetVertexShader(CompileShader(vs_5_0, VS_Lambert()));
		SetPixelShader(CompileShader(ps_5_0, PS_Lambert()));
	}

	pass P4
	{
		SetVertexShader(CompileShader(vs_5_0, VS_Lambert()));
		SetPixelShader(CompileShader(ps_5_0, PS_HalfLambert()));
	}
	
}