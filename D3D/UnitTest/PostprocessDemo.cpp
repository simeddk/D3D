#include "stdafx.h"
#include "PostprocessDemo.h"

void PostprocessDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(20, 0, 0);
	Context::Get()->GetCamera()->Position(1, 36, -85);
	((Freedom*)Context::Get()->GetCamera())->Speed(50, 2);

	shader = new Shader(L"22_NormalMap.fxo");

	float width = D3D::Width(), height = D3D::Height();
	rtv = new RenderTarget(width, height);
	dsv = new DepthStencil(width, height);
	viewport = new Viewport(width, height);

	render2D = new Render2D();
	render2D->GetTransform()->Position(177, 620, 0);
	render2D->GetTransform()->Scale(355, 200, 1);
	render2D->SRV(rtv->SRV());

	postProcess = new PostProcess(L"23_PostProcess.fxo");
	postProcess->SRV(rtv->SRV());

	sky = new CubeSky(L"Environment/Mountain1024.dds");

	Mesh();
	Airplane();
	Kachujin();
	Weapon();
}

void PostprocessDemo::Destroy()
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
	SafeDelete(viewport);

	SafeDelete(postProcess);
	SafeDelete(render2D);
}

void PostprocessDemo::Update()
{
	ImGui::SliderFloat3("LightDirection", Context::Get()->Direction(), -1, 1);

	static UINT pass = postProcess->GetShader()->PassCount() - 1;

	ImGui::InputInt("Pass", (int*)&pass);
	pass %= postProcess->GetShader()->PassCount();
	postProcess->Pass(pass);

	Vector2 PixelSize = Vector2(1 / D3D::Width(), 1 / D3D::Height());
	postProcess->GetShader()->AsVector("PixelSize")->SetFloatVector(PixelSize);

	switch (pass)
	{
	case 4 : 
	{
		ImGui::Separator();

		static float Saturation = 0;
		ImGui::SliderFloat("Saturation", &Saturation, 0, 5);
		postProcess->GetShader()->AsScalar("Saturation")->SetFloat(Saturation);
	}
	break;
	case 5:
	{
		ImGui::Separator();

		static float Sharpness = 0;
		ImGui::SliderFloat("Sharpness", &Sharpness, 0, 1);
		postProcess->GetShader()->AsScalar("Sharpness")->SetFloat(Sharpness);
	}
	break;
	case 6:
	{
		ImGui::Separator();

		static Vector2 NiggleOffset = Vector2(10, 10);
		static Vector2 NiggleAmount = Vector2(0.01f, 0.01f);
		ImGui::SliderFloat2("NiggleOffset", NiggleOffset, 1, 20);
		ImGui::SliderFloat2("NiggleAmount", NiggleAmount, 0.001f, 2);

		postProcess->GetShader()->AsVector("NiggleOffset")->SetFloatVector(NiggleOffset);
		postProcess->GetShader()->AsVector("NiggleAmount")->SetFloatVector(NiggleAmount);
	}
	break;
	case 7:
	{
		ImGui::Separator();

		static Vector2 Scale = Vector2(1, 1);
		static float Power = 2;
		ImGui::SliderFloat2("Scale", Scale, 0.01, 2);
		ImGui::SliderFloat("Power", &Power, 1, 20);

		postProcess->GetShader()->AsVector("Scale")->SetFloatVector(Scale);
		postProcess->GetShader()->AsScalar("Power")->SetFloat(Power);
	}
	break;
	}

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

	postProcess->Update();
	render2D->Update();
}

void PostprocessDemo::PreRender()
{
	viewport->RSSetViewport();
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

void PostprocessDemo::Render()
{
	postProcess->Render();
	render2D->Render();
}

void PostprocessDemo::Mesh()
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

void PostprocessDemo::Airplane()
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

void PostprocessDemo::Kachujin()
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

void PostprocessDemo::Weapon()
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

void PostprocessDemo::Pass(UINT val)
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
