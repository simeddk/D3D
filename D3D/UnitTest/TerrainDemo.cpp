#include "stdafx.h"
#include "TerrainDemo.h"

void TerrainDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(6, 0, 0);
	Context::Get()->GetCamera()->Position(127, 50 , -100);
	//dynamic_cast<Freedom*>(Context::Get()->GetCamera())->Speed(5);

	shader = new Shader(L"08_Terrain.fxo");
	terrain = new Terrain(shader, L"HeightMap256.png");
}

void TerrainDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(terrain);
}

void TerrainDemo::Update()
{
	static UINT pass = 0;
	ImGui::InputInt("Pass", (int*)&pass);
	pass = Math::Clamp(pass, 0, 1);
	terrain->Pass() = pass;

	terrain->Update();
}

void TerrainDemo::Render()
{
	terrain->Render();
}
