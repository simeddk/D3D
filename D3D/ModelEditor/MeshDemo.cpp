#include "stdafx.h"
#include "MeshDemo.h"
#include "Converter.h"

void MeshDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(13, 70, 0);
	Context::Get()->GetCamera()->Position(-20, 1, -3);

	shader = new Shader(L"18_Unify.fxo");

	planeMaterial = new Material(shader);
	planeMaterial->DiffuseMap(L"Floor.png");

	plane = new MeshRenderer(shader, new MeshPlane(10, 2));
	plane->AddTransform()->Scale(10, 1, 2);
	plane->UpdateTransforms();

	boxMaterial = new Material(shader);
	boxMaterial->DiffuseMap(L"Box.png");

	box = new MeshRenderer(shader, new MeshCube());
	for (float x = -50; x <= 50; x += 2.5f)
	{
		Transform* transform = box->AddTransform();
		
		transform->Scale(0.25f, 0.25f, 0.25f);
		transform->Position(Vector3(x, 0.25f * 0.5f, 0.f));
		transform->Rotation(0, Math::Random(-(float)D3DX_PI, (float)D3DX_PI), 0);
	}
	box->UpdateTransforms();
}

void MeshDemo::Destroy()
{
	SafeDelete(shader);

	SafeDelete(plane);
	SafeDelete(box);

	SafeDelete(planeMaterial);
	SafeDelete(boxMaterial);
}

void MeshDemo::Update()
{
	plane->Update();
	box->Update();
}

void MeshDemo::Render()
{
	planeMaterial->Render();
	plane->Render();

	boxMaterial->Render();
	box->Render();
}
