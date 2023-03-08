#include "Framework.h"
#include "Renderer.h"

Renderer::Renderer(Shader* shader)
	: shader(shader)
	, bCreatedShader(false)
{
	Initialize();
}

Renderer::Renderer(wstring shaderFile)
	: bCreatedShader(true)
{
	shader = new Shader(shaderFile);

	Initialize();
}

Renderer::~Renderer()
{
	SafeDelete(transform);
	SafeDelete(perFrame);

	SafeDelete(vertexBuffer);
	SafeDelete(indexBuffer);

	if (bCreatedShader == true)
		SafeDelete(shader);
}

void Renderer::Initialize()
{
	transform = new Transform(shader);
	perFrame = new PerFrame(shader);
}

void Renderer::Update()
{
	perFrame->Update();
	transform->Update();
}

void Renderer::Render()
{
	if (vertexBuffer != nullptr)
	{
		vertexBuffer->Render();

		if (indexBuffer != nullptr)
			indexBuffer->Render();
	}

	D3D::GetDC()->IASetPrimitiveTopology(topology);

	perFrame->Render();
	transform->Render();
}
