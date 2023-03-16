#include "stdafx.h"
#include "ModelDemo.h"
#include "Converter.h"

void ModelDemo::Initialize()
{
	shader = new Shader(L"18_Unify.fxo");

	Tank();
	Tower();
	Airplane();

	sky = new CubeSky(L"Environment/SnowCube1024.dds");
	sky->Pass(2);
}

void ModelDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(sky);

	SafeDelete(tank);
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
	}

	sky->Update();

	if (tank != nullptr)
		tank->Update();

	if (tower != nullptr)
		tower->Update();

	if (airplane != nullptr)
		airplane->Update();
}

void ModelDemo::Render()
{
	sky->Render();

	if (tank != nullptr)
	{
		tank->Pass(1);
		tank->Render();
	}

	if (tower != nullptr)
	{
		tower->Pass(1);
		tower->Render();
	}

	if (airplane != nullptr)
	{
		airplane->Pass(1);
		airplane->Render();
	}
}

void ModelDemo::Tank()
{
	tank = new ModelRenderer(shader);
	tank->ReadMesh(L"Tank/Tank");
	tank->ReadMaterial(L"Tank/Tank");
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
