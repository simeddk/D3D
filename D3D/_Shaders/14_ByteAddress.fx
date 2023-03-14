ByteAddressBuffer Input;
RWByteAddressBuffer Output;

struct Group
{
	uint3 GroupID;
	uint3 GroupThreadID;
	uint3 DispatchThreadID;
	uint GroupIndex;
};

struct ComputeThreadID
{
	uint3 GroupID : SV_GroupID;
	uint3 GroupThreadID : SV_GroupThreadID;
	uint3 DispatchThreadID : SV_DispatchThreadID;
	uint GroupIndex : SV_GroupIndex;
};

[numthreads(10, 8, 3)]
void CS(ComputeThreadID input)
{
	Group group;
	group.GroupID = asuint(input.GroupID);
	group.GroupThreadID = asuint(input.GroupThreadID);
	group.DispatchThreadID = asuint(input.DispatchThreadID);
	group.GroupIndex = asuint(input.GroupIndex);
	
	uint inAddress = (input.GroupID.x * 240 + input.GroupIndex) * 4;
	//float value = asfloat(40.f);
	float value = asfloat(Input.Load(inAddress)) + 50000;
	
	uint outAddress = (input.GroupID.x * 240 + input.GroupIndex) * 11 * 4;
	
	Output.Store3(outAddress + 0, asuint(group.GroupID));
	Output.Store3(outAddress + 12, asuint(group.GroupThreadID));
	Output.Store3(outAddress + 24, asuint(group.DispatchThreadID));
	Output.Store(outAddress + 36, asuint(group.GroupIndex));
	Output.Store(outAddress + 40, asuint(value));
}

technique11 T0
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);

		SetComputeShader(CompileShader(cs_5_0, CS()));
	}
}