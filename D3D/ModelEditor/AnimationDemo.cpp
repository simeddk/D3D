#include "stdafx.h"
#include "AnimationDemo.h"
#include "Converter.h"

void AnimationDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(12, 0, 0);
	Context::Get()->GetCamera()->Position(2, 4, -12);
	((Freedom*)Context::Get()->GetCamera())->Speed(10, 2);

	shader = new Shader(L"13_Animation.fxo");

	Kachujin();

	sky = new CubeSky(L"Environment/Mountain1024.dds");
	sky->Pass(2);

	planeShader = new Shader(L"10_Mesh.fxo");
	plane = new MeshPlane(planeShader, 6, 6);
	plane->GetTransform()->Scale(12, 1, 12);
	plane->DiffuseMap(L"Floor.png");
}

void AnimationDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(sky);
	SafeDelete(planeShader);
	SafeDelete(plane);

	SafeDelete(kachujin);
}

void AnimationDemo::Update()
{
	//Test
	{
		static int clip = 0;
		static float speed = 1.f;
		static float takeTime = 0.1f;

		static bool blendMode = false;
		static float blendAlpha = 0.f;

		ImGui::Checkbox("BlendMode", &blendMode);

		if (blendMode == false) //Tweening
		{
			ImGui::InputInt("Clip", &clip);
			clip = (int)Math::Clamp(clip, 0, 4);

			const char* clipName[] = { "Idle", "Walk", "Run", "Slash", "Uprock" };
			ImGui::Text("%s", clipName[clip]);
			ImGui::SliderFloat("Speed", &speed, 0.1f, 5.f);
			ImGui::SliderFloat("TakeTime", &takeTime, 0.1f, 5.f);

			if (ImGui::Button("Apply"))
				kachujin->PlayTweenMode(clip, speed, takeTime);
		}
		else //Bleding
		{
			ImGui::SliderFloat("BlendAlpha", &blendAlpha, 0, 2);
			kachujin->SetBlendAlpha(blendAlpha);

			if (ImGui::Button("Apply"))
				kachujin->PlayBlendMode(0, 1, 2);
		}


		static Vector3 LightDirection = Vector3(-1, -1, 1);
		ImGui::SliderFloat3("LightDirection", LightDirection, -1, 1);
		shader->AsVector("LightDirection")->SetFloatVector(LightDirection);
		planeShader->AsVector("LightDirection")->SetFloatVector(LightDirection);

		static UINT Pass = 0;
		ImGui::InputInt("Pass", (int*)&Pass);
		Pass %= 2;
		kachujin->Pass(Pass);

	}

	sky->Update();
	plane->Update();

	if (kachujin != nullptr)
		kachujin->Update();
}

void AnimationDemo::Render()
{
	sky->Render();
	plane->Render();

	if (kachujin != nullptr)
		kachujin->Render();
}

void AnimationDemo::Kachujin()
{
	kachujin = new ModelAnimator(shader);
	kachujin->ReadMesh(L"Kachujin/Mesh");
	kachujin->ReadMaterial(L"Kachujin/Mesh");

	kachujin->ReadClip(L"Kachujin/Idle");
	kachujin->ReadClip(L"Kachujin/Walk");
	kachujin->ReadClip(L"Kachujin/Run");
	kachujin->ReadClip(L"Kachujin/Slash");
	kachujin->ReadClip(L"Kachujin/Uprock");

	kachujin->GetTransform()->Scale(0.01f, 0.01f, 0.01f);
	kachujin->GetTransform()->Position(5, 0, 0);
}
