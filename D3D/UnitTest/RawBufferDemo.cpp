#include "stdafx.h"
#include "RawBufferDemo.h"

void RawBufferDemo::Initialize()
{
	Shader* shader = new Shader(L"14_ByteAddress.fx");

	UINT count = 10 * 8 * 3 * 2;

	struct Output
	{
		UINT GroupID[3];
		UINT GroupThreadID[3];
		UINT DispatchThreadID[3];
		UINT GroupIndex;
		float Result;
	};

	float* data = new float[count];
	for (UINT i = 0; i < count; i++)
		data[i] = Math::Random(0.f, 1000.f);

	RawBuffer* rawBuffer = new RawBuffer(data, sizeof(float) * count, sizeof(Output) * count);

	shader->AsSRV("Input")->SetResource(rawBuffer->SRV());
	shader->AsUAV("Output")->SetUnorderedAccessView(rawBuffer->UAV());
	shader->Dispatch(0, 0, 2, 1, 1);

	Output* output = new Output[count];
	rawBuffer->CopyFromOutput(output);

	FILE* file;
	fopen_s(&file, "../RawBuffer.csv", "w");

	for (UINT i = 0; i < count; i++)
	{
		Output temp = output[i];

		fprintf
		(
			file,
			"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%f\n",
			temp.GroupID[0], temp.GroupID[1], temp.GroupID[2],
			temp.GroupThreadID[0], temp.GroupThreadID[1], temp.GroupThreadID[2],
			temp.DispatchThreadID[0], temp.DispatchThreadID[1], temp.DispatchThreadID[2],
			temp.GroupIndex,
			temp.Result
		);
	}
	fclose(file);
}
