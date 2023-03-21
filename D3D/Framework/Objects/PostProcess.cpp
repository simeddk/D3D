#include "Framework.h"
#include "PostProcess.h"

PostProcess::PostProcess(wstring shaderFile)
	: Renderer(shaderFile)
{
	Vertex vertices[6];
	vertices[0].Position = Vector3(-1, -1, 0);
	vertices[1].Position = Vector3(-1, +1, 0);
	vertices[2].Position = Vector3(+1, -1, 0);
	vertices[3].Position = Vector3(+1, -1, 0);
	vertices[4].Position = Vector3(-1, +1, 0);
	vertices[5].Position = Vector3(+1, +1, 0);

	vertexBuffer = new VertexBuffer(vertices, 6, sizeof(Vertex));
	sDiffuseMap = shader->AsSRV("DiffuseMap");
}

PostProcess::~PostProcess()
{
}

void PostProcess::Update()
{
	Super::Update();
}

void PostProcess::Render()
{
	Super::Render();

	shader->Draw(0, Pass(), 6);
}

void PostProcess::SRV(ID3D11ShaderResourceView* srv)
{
	sDiffuseMap->SetResource(srv);
}
