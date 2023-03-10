#include "stdafx.h"
#include "ModelDemo.h"
#include "Converter.h"

void ModelDemo::Initialize()
{
	shader = new Shader(L"12_Model.fxo");

	Tank();
	Kachujin();
	Tower();
	Airplane();

	sky = new CubeSky(L"Environment/SnowCube1024.dds");
	sky->Pass(2);

	planeShader = new Shader(L"10_Mesh.fxo");
	plane = new MeshPlane(planeShader, 6, 6);
	plane->GetTransform()->Scale(12, 1, 12);
	plane->DiffuseMap(L"Floor.png");
}

void ModelDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(sky);
	SafeDelete(planeShader);
	SafeDelete(plane);

	SafeDelete(tank);
	SafeDelete(kachujin);
	SafeDelete(tower);
	SafeDelete(airplane);
}

void ModelDemo::Update()
{
	//Test
	{
		static Vector3 LightDirection = Vector3(-1, -1, 1);
		ImGui::SliderFloat3("LightDirection", LightDirection, -1, 1);
		shader->AsVector("LightDirection")->SetFloatVector(LightDirection);
		planeShader->AsVector("LightDirection")->SetFloatVector(LightDirection);

		static UINT Pass = 0;
		ImGui::InputInt("Pass", (int*)&Pass);
		Pass %= 2;
		tank->Pass(Pass);
		kachujin->Pass(Pass);
		tower->Pass(Pass);
		airplane->Pass(Pass);
		
	}

	sky->Update();
	plane->Update();

	if (tank != nullptr)
		tank->Update();

	if (kachujin != nullptr)
		kachujin->Update();

	if (tower != nullptr)
		tower->Update();

	if (airplane != nullptr)
		airplane->Update();
}

void ModelDemo::Render()
{
	sky->Render();
	plane->Render();

	if (tank != nullptr)
		tank->Render();

	if (kachujin != nullptr)
		kachujin->Render();

	if (tower != nullptr)
		tower->Render();

	if (airplane != nullptr)
		airplane->Render();
}

void ModelDemo::Tank()
{
	tank = new ModelRenderer(shader);
	tank->ReadMesh(L"Tank/Tank");
	tank->ReadMaterial(L"Tank/Tank");
}

void ModelDemo::Kachujin()
{
	kachujin = new ModelRenderer(shader);
	kachujin->ReadMesh(L"Kachujin/Mesh");
	kachujin->ReadMaterial(L"Kachujin/Mesh");
	kachujin->GetTransform()->Scale(0.01f, 0.01f, 0.01f);
	kachujin->GetTransform()->Position(5, 0, 0);
}

void ModelDemo::Tower()
{
	tower = new ModelRenderer(shader);
	tower->ReadMesh(L"Tower/Tower");
	tower->ReadMaterial(L"Tower/Tower");
	tower->GetTransform()->Scale(0.01f, 0.01f, 0.01f);
	tower->GetTransform()->Position(-5, 0, 0);
}

void ModelDemo::Airplane()
{
	airplane = new ModelRenderer(shader);
	airplane->ReadMesh(L"B787/Airplane");
	airplane->ReadMaterial(L"B787/Airplane");
	airplane->GetTransform()->Scale(0.001f, 0.001f, 0.001f);
	airplane->GetTransform()->Position(-10, 0, 0);
}
