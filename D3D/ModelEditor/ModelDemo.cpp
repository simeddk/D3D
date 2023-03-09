#include "stdafx.h"
#include "ModelDemo.h"
#include "Converter.h"

void ModelDemo::Initialize()
{
	shader = new Shader(L"12_Model.fxo");

	Tank();
	Kachujin();

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
		
	}

	sky->Update();
	plane->Update();

	if (tank != nullptr)
		tank->Update();

	if (kachujin != nullptr)
		kachujin->Update();
}

void ModelDemo::Render()
{
	sky->Render();
	plane->Render();

	if (tank != nullptr)
		tank->Render();

	if (kachujin != nullptr)
		kachujin->Render();
}

void ModelDemo::Tank()
{
	tank = new ModelRenderer(shader);
	tank->ReadMesh(L"Tank/Tank");
}

void ModelDemo::Kachujin()
{
	kachujin = new ModelRenderer(shader);
	kachujin->ReadMesh(L"Kachujin/Mesh");

	kachujin->GetTransform()->Scale(0.01f, 0.01f, 0.01f);
	kachujin->GetTransform()->Position(5, 0, 0);
}
