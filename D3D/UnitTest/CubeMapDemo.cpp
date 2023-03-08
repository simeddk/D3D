#include "stdafx.h"
#include "CubeMapDemo.h"

void CubeMapDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(20, 0, 0);
	Context::Get()->GetCamera()->Position(0, 36, -85);

	shader = new Shader(L"10_Mesh.fxo");
	
	CreateMesh();
	
	sLightDirection = shader->AsVector("LightDirection");

	cubeMapShader = new Shader(L"11_CubeMap.fxo");
	cubeMap = new CubeMap(cubeMapShader);
	cubeMap->Texture(L"Environment/Mountain1024.dds");
	cubeMap->GetTransform()->Position(0, 20, 0);
	cubeMap->GetTransform()->Scale(10, 10, 10);

	sky = new CubeSky(L"Environment/SunsetCube1024.dds");
}

void CubeMapDemo::Destroy()
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

	SafeDelete(cubeMapShader);
	SafeDelete(cubeMap);
	
	SafeDelete(sky);
}

void CubeMapDemo::Update()
{
	ImGui::SliderFloat3("Dirction", lightDirection, -1, 1);
	sLightDirection->SetFloatVector(lightDirection);

	static UINT pass = sky->GetShader()->PassCount() - 1;
	ImGui::InputInt("Sky Pass", (int*)&pass);
	pass %= sky->GetShader()->PassCount();

	sky->Pass(pass);

	quad->Update();
	plane->Update();
	cube->Update();

	for (UINT i = 0; i < 10; i++)
	{
		cylinders[i]->Update();
		spheres[i]->Update();
	}

	cubeMap->Update();

	sky->Update();
}

void CubeMapDemo::Render()
{
	static bool bWireFrame = false;
	ImGui::Checkbox("WireFrame", &bWireFrame);

	quad->Pass(bWireFrame ? 1 : 0);
	plane->Pass(bWireFrame ? 1 : 0);
	cube->Pass(bWireFrame ? 1 : 0);

	sky->Render();

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

	cubeMap->Render();
}

void CubeMapDemo::CreateMesh()
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
