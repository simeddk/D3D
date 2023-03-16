#include "stdafx.h"
#include "AnimationDemo.h"
#include "Converter.h"

void AnimationDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(12, 0, 0);
	Context::Get()->GetCamera()->Position(2, 4, -12);
	((Freedom*)Context::Get()->GetCamera())->Speed(10, 2);

	shader = new Shader(L"18_Unify.fxo");

	Kachujin();

}

void AnimationDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(weapon);

	SafeDelete(kachujin);
	SafeDelete(colliderObject);
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


	}

	
	if (kachujin != nullptr)
	{
		kachujin->Update();

		kachujin->GetAttachBones(bones);
		colliderObject->World->World(bones[40]);

		Transform* weaponTransform = weapon->GetTransform();
		weaponTransform->World(weaponInitTransform->World() * bones[40]);
		weapon->Update();
	}
}

void AnimationDemo::Render()
{
	
	if (kachujin != nullptr)
	{
		kachujin->Pass(2);
		kachujin->Render();

		colliderObject->Collision->Render();

		weapon->Pass(1);
		weapon->Render();
	}
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

	colliderObject = new ColliderObject();
	colliderObject->Init->Position(-2.9f, 1.45f, -50.0f);
	colliderObject->Init->Scale(5, 5, 75);
	colliderObject->Init->Rotation(0, 0, 1);
	ZeroMemory(&bones, sizeof(Matrix) * MAX_MODEL_TRANSFORMS);

	weapon = new ModelRenderer(shader);
	weapon->ReadMesh(L"Weapon/Sword");
	weapon->ReadMaterial(L"Weapon/Sword");

	weaponInitTransform = new Transform();
	weaponInitTransform->Position(-2.9f, 1.45f, -6.45f);
	weaponInitTransform->Scale(0.5f, 0.5f, 0.75f);
	weaponInitTransform->Rotation(0, 0, 1);
}
