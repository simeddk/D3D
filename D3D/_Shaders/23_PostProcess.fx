#include "00_Global.fx"
#include "00_Light.fx"
#include "00_Render.fx"

struct VertexOutput
{
	float4 Position : SV_Position;
	float2 Uv : Uv;
};

VertexOutput VS(float4 Position : Position)
{
	VertexOutput output;
	output.Position = Position;
	output.Uv.x = Position.x * 0.5f + 0.5f;
	output.Uv.y = -Position.y * 0.5f + 0.5f;
	
	return output;
}

float4 PS_Diffuse(VertexOutput input) : SV_Target
{
	return DiffuseMap.Sample(LinearSampler, input.Uv);
}

float4 PS_Grayscale(VertexOutput input) : SV_Target
{
	float4 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv);
	float average = (diffuse.r + diffuse.g + diffuse.b) / 3;

	return float4(average, average, average, 1);
}

float4 PS_Grayscale2(VertexOutput input) : SV_Target
{
	float4 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv);
	float3 grayscale = float3(0.299f, 0.587f, 0.0114f);
	float average = dot(diffuse.rgb, grayscale);

	return float4(average, average, average, 1);
}

float3x3 ColorMatrix = float3x3
(
	0.393, 0.769, 0.189, //R
    0.349, 0.686, 0.168, //G
    0.272, 0.534, 0.131 //B
);

float4 PS_Hercules(VertexOutput input) : SV_Target
{
	float4 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv);
	float3 result = mul(ColorMatrix, diffuse.rgb);
	
	return float4(result, 1);
}

float Saturation = 0;
float4 PS_Saturation(VertexOutput input) : SV_Target
{
	float3 grayscale = float3(0.299f, 0.587f, 0.0114f);
	float4 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv);
	
	float average = dot(diffuse.rgb, grayscale);

	diffuse.rgb = lerp(average, diffuse, Saturation);
	diffuse.a = 1.f;
	
	return diffuse;
}

float2 PixelSize;
float Sharpness = 0;
float4 PS_Sharpness(VertexOutput input) : SV_Target
{
	float4 center = DiffuseMap.Sample(LinearSampler, input.Uv);
	float4 top = DiffuseMap.Sample(LinearSampler, input.Uv + float2(0, -PixelSize.y));
	float4 bottom = DiffuseMap.Sample(LinearSampler, input.Uv + float2(0, +PixelSize.y));
	float4 left = DiffuseMap.Sample(LinearSampler, input.Uv + float2(-PixelSize.x, 0));
	float4 right = DiffuseMap.Sample(LinearSampler, input.Uv + float2(+PixelSize.x, 0));
	
	float4 edge = center * 4 - top - bottom - left - right;
	
	float3 grayscale = float3(0.299f, 0.587f, 0.0114f);
	float average = dot(edge.rgb, grayscale);
	
	return center + Sharpness * float4(average, average, average, 1);
}

float2 NiggleOffset = float2(10, 10);
float2 NiggleAmount = float2(0.01f, 0.01f);
float4 PS_Niggle(VertexOutput input) : SV_Target
{
	float2 uv = input.Uv;
	
	uv.x += sin(Time + uv.x * NiggleOffset.x) * NiggleAmount.x;
	uv.y += cos(Time + uv.y * NiggleOffset.y) * NiggleAmount.y;

	return DiffuseMap.Sample(LinearSampler, uv);
}

float2 Scale = float2(1, 1);
float Power = 2;
float4 PS_Vignette(VertexOutput input) : SV_Target
{
	float4 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv);
	
	float radius = length((input.Uv - 0.5f) * 2 / Scale);
	float vignette = pow(abs(radius + 1e-6), Power);

	return diffuse * saturate(1 - vignette);
}

struct PixelOut
{
	float4 Color0 : SV_Target0;
	float4 Color1 : SV_Target1;
	float4 Color2 : SV_Target2;
};


technique11 T0
{
	P_VP(P0, VS, PS_Diffuse)
	P_VP(P1, VS, PS_Grayscale)
	P_VP(P2, VS, PS_Grayscale2)
	P_VP(P3, VS, PS_Hercules)
	P_VP(P4, VS, PS_Saturation)
	P_VP(P5, VS, PS_Sharpness)
	P_VP(P6, VS, PS_Niggle)
	P_VP(P7, VS, PS_Vignette)
}

