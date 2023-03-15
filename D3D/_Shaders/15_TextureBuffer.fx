Texture2DArray<float4> Input;
RWTexture2DArray<float4> Output;

[numthreads(32, 32, 1)]
void CS_Diffuse(uint3 id : SV_DispatchThreadID)
{
	Output[id] = Input.Load(int4(id, 0));
}

[numthreads(32, 32, 1)]
void CS_Red(uint3 id : SV_DispatchThreadID)
{
	float4 color = Input.Load(int4(id, 0));
	Output[id] = float4(color.r, 0, 0, 1);
}

[numthreads(32, 32, 1)]
void CS_Inverse(uint3 id : SV_DispatchThreadID)
{
	float4 color = Input.Load(int4(id, 0));
	Output[id] = 1 - color;
}

[numthreads(32, 32, 1)]
void CS_Gray(uint3 id : SV_DispatchThreadID)
{
	float4 color = Input.Load(int4(id, 0));
	Output[id] = (color.r + color.g + color.b) * 0.333f;
}

technique11 T0
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);

		SetComputeShader(CompileShader(cs_5_0, CS_Diffuse()));
	}

	pass P1
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);

		SetComputeShader(CompileShader(cs_5_0, CS_Red()));
	}

	pass P2
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);

		SetComputeShader(CompileShader(cs_5_0, CS_Inverse()));
	}

	pass P3
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);

		SetComputeShader(CompileShader(cs_5_0, CS_Gray()));
	}
}