#include "stdafx.h"
#include "MeshDemo.h"

void MeshDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(20, 0, 0);
	Context::Get()->GetCamera()->Position(0, 36, -85);

	shader = new Shader(L"10_Mesh.fxo");
	
	CreateMesh();
	
	sLightDirection = shader->AsVector("LightDirection");
}

void MeshDemo::Destroy()
{
	SafeDelete(shader);

	SafeDelete(quad);
	SafeDelete(plane);
}

void MeshDemo::Update()
{
	ImGui::SliderFloat3("Dirction", lightDirection, -1, 1);
	sLightDirection->SetFloatVector(lightDirection);

	quad->Update();
	plane->Update();
}

void MeshDemo::Render()
{
	static bool bWireFrame = false;
	ImGui::Checkbox("WireFrame", &bWireFrame);

	quad->Pass(bWireFrame ? 1 : 0);
	plane->Pass(bWireFrame ? 1 : 0);

	quad->Render();
	plane->Render();
}

void MeshDemo::CreateMesh()
{
	quad = new MeshQuad(shader);
	quad->DiffuseMap(L"Box.png");

	plane = new MeshPlane(shader, 2.5f, 2.5f);
	plane->Scale(12, 1, 12);
	plane->DiffuseMap(L"Floor.png");
}
