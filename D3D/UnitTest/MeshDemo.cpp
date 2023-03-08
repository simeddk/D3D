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
	SafeDelete(cube);

	for (UINT i = 0; i < 10; i++)
	{
		SafeDelete(cylinders[i])
		SafeDelete(spheres[i])
	}
}

void MeshDemo::Update()
{
	ImGui::SliderFloat3("Dirction", lightDirection, -1, 1);
	sLightDirection->SetFloatVector(lightDirection);

	quad->Update();
	plane->Update();
	cube->Update();

	for (UINT i = 0; i < 10; i++)
	{
		cylinders[i]->Update();
		spheres[i]->Update();
	}
}

void MeshDemo::Render()
{
	static bool bWireFrame = false;
	ImGui::Checkbox("WireFrame", &bWireFrame);

	quad->Pass(bWireFrame ? 1 : 0);
	plane->Pass(bWireFrame ? 1 : 0);
	cube->Pass(bWireFrame ? 1 : 0);

	for (UINT i = 0; i < 10; i++)
	{
		cylinders[i]->Pass(bWireFrame ? 1 : 0);
		spheres[i]->Pass(bWireFrame ? 1 : 0);
		
		cylinders[i]->Render();
		spheres[i]->Render();
	}

	quad->Render();
	plane->Render();
	cube->Render();
}

void MeshDemo::CreateMesh()
{
	quad = new MeshQuad(shader);
	quad->DiffuseMap(L"Box.png");

	plane = new MeshPlane(shader, 2.5f, 2.5f);
	plane->GetTransform()->Scale(12, 1, 12);
	plane->DiffuseMap(L"Floor.png");

	cube = new MeshCube(shader);
	cube->GetTransform()->Position(0, 5, 0);
	cube->GetTransform()->Scale(20, 10, 20);
	cube->DiffuseMap(L"Stones.png");

	for (UINT i = 0; i < 5; i++)
	{
		cylinders[i * 2 + 0] = new MeshCylinder(shader, 0.3f, 0.5f, 3.f, 20, 20);
		cylinders[i * 2 + 0]->GetTransform()->Position(-30, 6, (float)i * 15 -15.f);
		cylinders[i * 2 + 0]->GetTransform()->Scale(5, 5, 5);
		cylinders[i * 2 + 0]->DiffuseMap(L"Bricks.png");

		cylinders[i * 2 + 1] = new MeshCylinder(shader, 0.3f, 0.5f, 3.f, 20, 20);
		cylinders[i * 2 + 1]->GetTransform()->Position(+30, 6, (float)i * 15 - 15.f);
		cylinders[i * 2 + 1]->GetTransform()->Scale(5, 5, 5);
		cylinders[i * 2 + 1]->DiffuseMap(L"Bricks.png");

		spheres[i * 2 + 0] = new MeshSphere(shader, 0.5f);
		spheres[i * 2 + 0]->GetTransform()->Position(-30, 15.5f, (float)i * 15 - 15.f);
		spheres[i * 2 + 0]->GetTransform()->Scale(5, 5, 5);
		spheres[i * 2 + 0]->DiffuseMap(L"Wall.png");

		spheres[i * 2 + 1] = new MeshSphere(shader, 0.5f);
		spheres[i * 2 + 1]->GetTransform()->Position(+30, 15.5f, (float)i * 15 - 15.f);
		spheres[i * 2 + 1]->GetTransform()->Scale(5, 5, 5);
		spheres[i * 2 + 1]->DiffuseMap(L"Wall.png");
	}
}
