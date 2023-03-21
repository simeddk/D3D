#include "stdafx.h"
#include "RtvDemo.h"

void RtvDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(20, 0, 0);
	Context::Get()->GetCamera()->Position(1, 36, -85);
	((Freedom*)Context::Get()->GetCamera())->Speed(50, 2);

	shader = new Shader(L"22_NormalMap.fxo");

	rtv = new RenderTarget();
	dsv = new DepthStencil();

	sky = new CubeSky(L"Environment/Mountain1024.dds");

	Mesh();
	Airplane();
	Kachujin();
	Weapon();
}

void RtvDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(sky);

	//Matrerials
	SafeDelete(floor);
	SafeDelete(stone);
	SafeDelete(brick);
	SafeDelete(wall);

	//Mesh
	SafeDelete(plane);
	SafeDelete(cube);
	SafeDelete(cylinder);
	SafeDelete(sphere);

	//Model
	SafeDelete(airplane);
	SafeDelete(kachujin);
	SafeDelete(weapon);

	//RTV
	SafeDelete(rtv);
	SafeDelete(dsv);
}

void RtvDemo::Update()
{
	ImGui::SliderFloat3("LightDirection", Context::Get()->Direction(), -1, 1);

	sky->Update();

	cube->Update();
	plane->Update();
	cylinder->Update();
	sphere->Update();

	airplane->Update();
	kachujin->Update();

	Matrix worlds[MAX_MODEL_TRANSFORMS];
	for (UINT i = 0; i < kachujin->TransformCount(); i++)
	{
		kachujin->GetAttachBones(i, worlds);
		weapon->GetTransform(i)->World(weaponTransform->World() * worlds[40]);
	}
	weapon->UpdateTransforms();
	weapon->Update();
}

void RtvDemo::PreRender()
{
	rtv->PreRender(dsv);

	sky->Render();

	Pass(0);

	wall->Render();
	sphere->Render();

	brick->Render();
	cylinder->Render();

	stone->Render();
	cube->Render();

	floor->Render();
	plane->Render();

	airplane->Render();
	kachujin->Render();
	weapon->Render();
}

void RtvDemo::Render()
{
	if (Keyboard::Get()->Down(VK_SPACE))
		rtv->SaveTexture(L"../ScreenShot.png");

	sky->Render();
	shader->AsSRV("DiffuseMap")->SetResource(rtv->SRV());
	cube->Render();
}

void RtvDemo::Mesh()
{
	//Create Material
	{
		floor = new Material(shader);
		floor->DiffuseMap("Floor.png");
		floor->Specular(1, 1, 1, 20);
		floor->SpecularMap("Floor_Specular.png");
		floor->NormalMap("Floor_Normal.png");

		stone = new Material(shader);
		stone->DiffuseMap("Stones.png");
		stone->Specular(1, 1, 1, 20);
		stone->SpecularMap("Stones_Specular.png");
		stone->Emissive(0.15f, 0.15f, 0.15f, 0.3f);
		stone->NormalMap("Stones_Normal.png");

		brick = new Material(shader);
		brick->DiffuseMap("Bricks.png");
		brick->Specular(1, 1, 1, 20);
		brick->SpecularMap("Bricks_Specular.png");
		brick->Emissive(0.15f, 0.15f, 0.15f, 0.3f);
		brick->NormalMap("Bricks_Normal.png");

		wall = new Material(shader);
		wall->DiffuseMap("Wall.png");
		wall->Specular(1, 1, 1, 20);
		wall->SpecularMap("Wall_Specular.png");
		wall->Emissive(0.15f, 0.15f, 0.15f, 0.3f);
		wall->NormalMap("Wall_Normal.png");
	}


	//Create Mesh
	{
		Transform* transform = NULL;

		cube = new MeshRenderer(shader, new MeshCube());
		transform = cube->AddTransform();
		transform->Position(0, 5, 0);
		transform->Scale(20, 10, 20);

		plane = new MeshRenderer(shader, new MeshPlane(2.5f, 2.5f));
		transform = plane->AddTransform();
		transform->Scale(12, 1, 12);


		cylinder = new MeshRenderer(shader, new MeshCylinder(0.3f, 0.5f, 3.0f, 20, 20));
		sphere = new MeshRenderer(shader, new MeshSphere(0.5f, 20, 20));
		for (UINT i = 0; i < 5; i++)
		{
			transform = cylinder->AddTransform();
			transform->Position(-30, 6, (float)i * 15.0f - 15.0f);
			transform->Scale(5, 5, 5);

			transform = cylinder->AddTransform();
			transform->Position(+30, 6, (float)i * 15.0f - 15.0f);
			transform->Scale(5, 5, 5);


			transform = sphere->AddTransform();
			transform->Position(-30, 15.5f, (float)i * 15.0f - 15.0f);
			transform->Scale(5, 5, 5);

			transform = sphere->AddTransform();
			transform->Position(+30, 15.5f, (float)i * 15.0f - 15.0f);
			transform->Scale(5, 5, 5);
		}
	}

	cube->UpdateTransforms();
	plane->UpdateTransforms();
	cylinder->UpdateTransforms();
	sphere->UpdateTransforms();

	meshes.push_back(cube);
	meshes.push_back(plane);
	meshes.push_back(cylinder);
	meshes.push_back(sphere);
}

void RtvDemo::Airplane()
{
	airplane = new ModelRenderer(shader);
	airplane->ReadMesh(L"B787/Airplane");
	airplane->ReadMaterial(L"B787/Airplane");

	Transform* transform = airplane->AddTransform();
	transform->Position(2.0f, 9.91f, 2.0f);
	transform->Scale(0.004f, 0.004f, 0.004f);
	airplane->UpdateTransforms();

	models.push_back(airplane);
}

void RtvDemo::Kachujin()
{
	kachujin = new ModelAnimator(shader);
	kachujin->ReadMesh(L"Kachujin/Mesh");
	kachujin->ReadMaterial(L"Kachujin/Mesh");
	kachujin->ReadClip(L"Kachujin/Idle");
	kachujin->ReadClip(L"Kachujin/Walk");
	kachujin->ReadClip(L"Kachujin/Run");
	kachujin->ReadClip(L"Kachujin/Slash");
	kachujin->ReadClip(L"Kachujin/Uprock");


	Transform* transform = nullptr;

	transform = kachujin->AddTransform();
	transform->Position(0, 0, -30);
	transform->Scale(0.075f, 0.075f, 0.075f);
	kachujin->PlayTweenMode(0, 0, 1.0f);

	transform = kachujin->AddTransform();
	transform->Position(-15, 0, -30);
	transform->Scale(0.075f, 0.075f, 0.075f);
	kachujin->PlayTweenMode(1, 1, 1.0f);

	transform = kachujin->AddTransform();
	transform->Position(-30, 0, -30);
	transform->Scale(0.075f, 0.075f, 0.075f);
	kachujin->PlayTweenMode(2, 2, 0.75f);

	transform = kachujin->AddTransform();
	transform->Position(15, 0, -30);
	transform->Scale(0.075f, 0.075f, 0.075f);
	kachujin->PlayBlendMode(3, 0, 1, 2);
	kachujin->SetBlendAlpha(3, 1.75f);

	transform = kachujin->AddTransform();
	transform->Position(30, 0, -32.5f);
	transform->Scale(0.075f, 0.075f, 0.075f);
	kachujin->PlayTweenMode(4, 4, 0.75f);

	kachujin->UpdateTransforms();

	animators.push_back(kachujin);
}

void RtvDemo::Weapon()
{
	weapon = new ModelRenderer(shader);
	weapon->ReadMesh(L"Weapon/Sword");
	weapon->ReadMaterial(L"Weapon/Sword");

	UINT count = kachujin->TransformCount();
	for (UINT i = 0; i < count; i++)
		weapon->AddTransform();

	weapon->UpdateTransforms();
	models.push_back(weapon);


	weaponTransform = new Transform();
	weaponTransform->Position(-2.9f, 1.45f, -6.45f);
	weaponTransform->Scale(0.5f, 0.5f, 0.75f);
	weaponTransform->Rotation(0, 0, 1);
}

void RtvDemo::Pass(UINT val)
{
	for (MeshRenderer* mesh : meshes)
		mesh->Pass(val);

	val++;
	for (ModelRenderer* model : models)
		model->Pass(val);

	val++;
	for (ModelAnimator* animation : animators)
		animation->Pass(val);
}
