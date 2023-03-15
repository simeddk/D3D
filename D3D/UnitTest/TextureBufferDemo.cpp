#include "stdafx.h"
#include "TextureBufferDemo.h"

void TextureBufferDemo::Initialize()
{
	shader = new Shader(L"15_TextureBuffer.fxo");
	texture = new Texture(L"Environment/Compute.png");

	textureBuffer = new  TextureBuffer(texture->GetTexture());
	shader->AsSRV("Input")->SetResource(textureBuffer->SRV());
	shader->AsUAV("Output")->SetUnorderedAccessView(textureBuffer->UAV());

	render2D = new Render2D();
	render2D->GetTransform()->Scale(D3D::Width(), D3D::Height(), 1);
	render2D->GetTransform()->Position(D3D::Width() * 0.5f, D3D::Height() * 0.5f, 0);
	render2D->SRV(textureBuffer->OutSRV());
}

void TextureBufferDemo::Destroy()
{
	//SafeDelete(texture);
	//SafeDelete(textureBuffer);
	//SafeDelete(render2D);
}

void TextureBufferDemo::Update()
{
	UINT width = textureBuffer->Width();
	UINT height = textureBuffer->Height();

	float x = ((float)width / 32) < 1.f  ? 1.f : (float)width / 32;
	float y = ((float)height / 32) < 1.f ? 1.f : (float)height / 32;

	UINT groupX = (UINT)ceilf(x);
	UINT groupY = (UINT)ceilf(y);

	if (ImGui::Button("Diffuse"))
		shader->Dispatch(0, 0, groupX, groupY, 1);

	if (ImGui::Button("Red"))
		shader->Dispatch(0, 1, groupX, groupY, 1);

	if (ImGui::Button("Inverse"))
		shader->Dispatch(0, 2, groupX, groupY, 1);

	if (ImGui::Button("Gray"))
		shader->Dispatch(0, 3, groupX, groupY, 1);

	render2D->Update();
}

void TextureBufferDemo::Render()
{
	render2D->Render();
}
