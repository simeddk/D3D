#include "stdafx.h"
#include "TerrainDemo.h"

void TerrainDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(6, 0, 0);
	Context::Get()->GetCamera()->Position(127, 50 , -100);
	dynamic_cast<Freedom*>(Context::Get()->GetCamera())->Speed(50);

	shader = new Shader(L"09_Terrain.fxo");
	terrain = new Terrain(shader, L"Terrain/Gray256.png");
	terrain->BaseMap(L"Terrain/Cliff (Layered Rock).jpg");
}

void TerrainDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(terrain);
}

void TerrainDemo::Update()
{
	static UINT pass = shader->PassCount() - 1;
	ImGui::InputInt("Pass", (int*)&pass);
	pass = Math::Clamp(pass, 0, shader->PassCount() - 1);
	terrain->Pass() = pass;

	static Vector3 lightDirection = Vector3(-1, -1, 1);
	ImGui::SliderFloat3("Light Direction", lightDirection, -1, 1);
	shader->AsVector("LightDirection")->SetFloatVector(lightDirection);

	static UINT mode = 1;
	ImGui::RadioButton("Albedo", (int*)&mode, 1);
	ImGui::SameLine();
	ImGui::RadioButton("Lambert", (int*)&mode, 2);
	shader->AsScalar("Mode")->SetInt(mode);

	terrain->Update();
}

void TerrainDemo::Render()
{
	terrain->Render();
}
