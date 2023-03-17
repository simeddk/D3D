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

		static UINT instaceID = 0;
		ImGui::SliderInt("Instance ID", (int*)&instaceID, 0, airplane->TransformCount() - 1);
		Transform* transform = airplane->GetTransform(instaceID);

		static Vector3 rotation;
		ImGui::SliderFloat3("Rotation", rotation, -180, 180);
		transform->RotationDegree(rotation);

		airplane->SetColor(0, Color(1, 0, 0, 1));
		airplane->SetColor(airplane->TransformCount() - 1, Color(0, 1, 0, 1));
		airplane->UpdateTransforms();
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

	for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* transform = tank->AddTransform();
		transform->Position(x, 0, 5);
		transform->RotationDegree(0, Math::Random(-180, 180), 0);
		transform->Scale(0.1f, 0.1f, 0.1);
	}
	tank->UpdateTransforms();
}

void ModelDemo::Tower()
{
	tower = new ModelRenderer(shader);
	tower->ReadMesh(L"Tower/Tower");
	tower->ReadMaterial(L"Tower/Tower");

	for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* transform = tower->AddTransform();
		transform->Position(x, 0, 7.5f);
		transform->RotationDegree(0, Math::Random(-180, 180), 0);
		transform->Scale(0.003f, 0.003f, 0.003);
	}
	tower->UpdateTransforms();
}

void ModelDemo::Airplane()
{
	airplane = new ModelRenderer(shader);
	airplane->ReadMesh(L"B787/Airplane");
	airplane->ReadMaterial(L"B787/Airplane");
	
	for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* transform = airplane->AddTransform();
		transform->Position(x, 0, 2.5f);
		transform->RotationDegree(0, Math::Random(-180, 180), 0);
		transform->Scale(0.00025f, 0.00025f, 0.00025f);
	}
	airplane->UpdateTransforms();
}
