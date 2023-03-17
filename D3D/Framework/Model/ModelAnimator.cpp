#include "Framework.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(Shader* shader)
	: shader(shader)
{
	model = new Model();
	
	instanceWorldBuffer = new VertexBuffer(worlds, MAX_MODEL_INSTANCE, sizeof(Matrix), 1, true);
	instanceColorBuffer = new VertexBuffer(colors, MAX_MODEL_INSTANCE, sizeof(Color), 2, true);

	frameBuffer = new ConstantBuffer(&tweenDesc, sizeof(TweenDesc) * MAX_MODEL_INSTANCE);
	blendBuffer = new ConstantBuffer(&blendDesc, sizeof(BlendDesc) * MAX_MODEL_INSTANCE);

	sTransformsSRV = shader->AsSRV("TransformsMap");

	sFrameBuffer = shader->AsConstantBuffer("CB_AnimationFrame");
	sBlendBuffer = shader->AsConstantBuffer("CB_BlendingFrame");

	//Compute Shader
	{
		computeShader = new Shader(L"20_GetBones.fxo");

		sComputeWorld = computeShader->AsMatrix("World");

		sComputeFrameBuffer = computeShader->AsConstantBuffer("CB_AnimationFrame");
		sComputeBlendBuffer = computeShader->AsConstantBuffer("CB_BlendingFrame");
		sComputeTransformsSRV = computeShader->AsSRV("TransformsMap");

		computeBoneBuffer = new StructuredBuffer(nullptr, sizeof(Matrix), MAX_MODEL_TRANSFORMS);
		sComputeInputBoneBuffer = computeShader->AsSRV("InputBones");

		computeWorldBuffer = new StructuredBuffer(nullptr, sizeof(Matrix), MAX_MODEL_TRANSFORMS);
		sComputeInputWorldBuffer = computeShader->AsSRV("InputWorlds");

		ID3D11Texture2D* texture;
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = MAX_MODEL_TRANSFORMS * 4;
		desc.Height = MAX_MODEL_INSTANCE;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;
		Check(D3D::GetDevice()->CreateTexture2D(&desc, nullptr, &texture));

		computeOutputBuffer = new TextureBuffer(texture);
		sComputeOutputBuffer = computeShader->AsUAV("Output");

		SafeRelease(texture);

		attachBones = new Matrix*[MAX_MODEL_INSTANCE];
		for (UINT i = 0; i < MAX_MODEL_TRANSFORMS; i++)
			attachBones[i] = new Matrix[MAX_MODEL_TRANSFORMS];
	}
}

ModelAnimator::~ModelAnimator()
{
	SafeDelete(model);

	for (Transform* transform : transforms)
		SafeDelete(transform);

	SafeDelete(instanceWorldBuffer);
	SafeDelete(instanceColorBuffer);

	SafeDeleteArray(clipTransforms);
	SafeRelease(texture);
	SafeRelease(transformsSRV);

	SafeDelete(frameBuffer);
	SafeDelete(blendBuffer);

	SafeDelete(computeShader);
	SafeDelete(computeWorldBuffer);
	SafeDelete(computeBoneBuffer);
	SafeDelete(computeOutputBuffer);

	for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
		SafeDeleteArray(attachBones[i]);
	SafeDeleteArray(attachBones);
}

void ModelAnimator::Update()
{
	if (texture == nullptr)
	{
		for (ModelMesh* mesh : model->Meshes())
			mesh->SetShader(shader);

		CreateTexture();

		Matrix bones[MAX_MODEL_TRANSFORMS];
		for (UINT i = 0; i < model->BoneCount(); i++)
			bones[i] = model->BoneByIndex(i)->Transform();

		computeBoneBuffer->CopyToInput(bones);
	}

	for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
	{
		if (blendDesc[i].Mode == 0)
			UpdateAnimationFrame(i);
		else
			UpdateBlendingFrame(i);
	}

	frameBuffer->Render();
	blendBuffer->Render();

	frameTime += Time::Delta();
	if (frameTime > (1 / frameRate))
	{
		sComputeFrameBuffer->SetConstantBuffer(frameBuffer->Buffer());
		sComputeBlendBuffer->SetConstantBuffer(blendBuffer->Buffer());
		sComputeTransformsSRV->SetResource(transformsSRV);

		sComputeInputBoneBuffer->SetResource(computeBoneBuffer->SRV());
		sComputeInputWorldBuffer->SetResource(computeWorldBuffer->SRV());
		sComputeOutputBuffer->SetUnorderedAccessView(computeOutputBuffer->UAV());

		computeShader->Dispatch(0, 0, 1, MAX_MODEL_INSTANCE, 1);

		ID3D11Texture2D* texture = computeOutputBuffer->CopyFromOutput();
		D3D11_MAPPED_SUBRESOURCE subResource;
		D3D::GetDC()->Map(texture, 0, D3D11_MAP_READ, 0, &subResource);
		{
			for (UINT i = 0; i < transforms.size(); i++)
			{
				BYTE* start = (BYTE*)subResource.pData + (i * subResource.RowPitch);

				memcpy(attachBones[i], start, sizeof(Matrix) * MAX_MODEL_TRANSFORMS);
			}
		}
		D3D::GetDC()->Unmap(texture, 0);
	}
	frameTime = fmod(frameTime, (1.f / frameRate));

	for (ModelMesh* mesh : model->Meshes())
		mesh->Update();
}

void ModelAnimator::Render()
{
	sFrameBuffer->SetConstantBuffer(frameBuffer->Buffer());
	sBlendBuffer->SetConstantBuffer(blendBuffer->Buffer());

	sTransformsSRV->SetResource(transformsSRV);

	instanceWorldBuffer->Render();
	instanceColorBuffer->Render();

	for (ModelMesh* mesh : model->Meshes())
	{
		mesh->Render(transforms.size());
	}
}

void ModelAnimator::UpdateAnimationFrame(UINT instance)
{
	TweenDesc& desc = tweenDesc[instance];
	ModelClip* clip = model->ClipByIndex(desc.Curr.Clip);

	desc.Curr.RunningTime += Time::Delta();

	float time = 1 / clip->FrameRate() / desc.Curr.Speed;

	if (desc.Curr.Time >= 1.f)
	{
		desc.Curr.RunningTime = 0;

		desc.Curr.CurrFrame = (desc.Curr.CurrFrame + 1) % clip->FrameCount();
		desc.Curr.NextFrame = (desc.Curr.CurrFrame + 1) % clip->FrameCount();
	}
	desc.Curr.Time = desc.Curr.RunningTime / time;

	if (desc.Next.Clip > -1)
	{
		ModelClip* nextClip = model->ClipByIndex(desc.Next.Clip);

		desc.ChangeTime += Time::Delta();
		desc.TweenTime = desc.ChangeTime / desc.TakeTime;

		if (desc.TweenTime >= 1.f)
		{
			desc.Curr = desc.Next;

			desc.Next.Clip = -1;
			desc.Next.CurrFrame = 0;
			desc.Next.NextFrame = 0;
			desc.Next.Time = 0;
			desc.Next.RunningTime = 0;
			desc.ChangeTime = 0;
			desc.TweenTime = 0;
		}
		else
		{
			desc.Next.RunningTime += Time::Delta();
			float time = 1 / clip->FrameRate() / desc.Next.Speed;

			if (desc.Next.Time >= 1.f)
			{
				desc.Next.RunningTime = 0;

				desc.Next.CurrFrame = (desc.Next.CurrFrame + 1) % clip->FrameCount();
				desc.Next.NextFrame = (desc.Next.CurrFrame + 1) % clip->FrameCount();
			}
			desc.Next.Time = desc.Next.RunningTime / time;
		}
	}
}

void ModelAnimator::UpdateBlendingFrame(UINT instance)
{
	BlendDesc& desc = blendDesc[instance];

	for (UINT i = 0; i < 3; i++)
	{
		ModelClip* clip = model->ClipByIndex(desc.Clip[i].Clip);

		desc.Clip[i].RunningTime += Time::Delta();

		float time = 1 / clip->FrameRate() / desc.Clip[i].Speed;

		if (desc.Clip[i].Time >= 1.f)
		{
			desc.Clip[i].RunningTime = 0;

			desc.Clip[i].CurrFrame = (desc.Clip[i].CurrFrame + 1) % clip->FrameCount();
			desc.Clip[i].NextFrame = (desc.Clip[i].CurrFrame + 1) % clip->FrameCount();
		}
		desc.Clip[i].Time = desc.Clip[i].RunningTime / time;
	}

}

void ModelAnimator::ReadMesh(wstring file)
{
	model->ReadMesh(file);
}

void ModelAnimator::ReadMaterial(wstring file)
{
	model->ReadMaterial(file);
}

void ModelAnimator::ReadClip(wstring file)
{
	model->ReadClip(file);
}

void ModelAnimator::PlayTweenMode(UINT instance, UINT clip, float speed, float takeTime)
{
	blendDesc[instance].Mode = 0;
	tweenDesc[instance].Next.Clip = clip;
	tweenDesc[instance].Next.Speed = speed;
	tweenDesc[instance].TakeTime = takeTime;
}

void ModelAnimator::PlayBlendMode(UINT instance, UINT clip1, UINT clip2, UINT clip3)
{
	blendDesc[instance].Mode = 1;
	blendDesc[instance].Clip[0].Clip = clip1;
	blendDesc[instance].Clip[1].Clip = clip2;
	blendDesc[instance].Clip[2].Clip = clip3;
}

void ModelAnimator::SetBlendAlpha(UINT instance, float alpha)
{
	alpha = Math::Clamp(alpha, 0.f, 2.f);
	blendDesc[instance].Alpha = alpha;
}

void ModelAnimator::Pass(UINT pass)
{
	for (ModelMesh* mesh : model->Meshes())
		mesh->Pass(pass);
}

Transform* ModelAnimator::AddTransform()
{
	Transform* transform = new Transform();
	transforms.push_back(transform);

	colors[transforms.size() - 1] = Color(0, 0, 0, 1);

	return transform;
}


void ModelAnimator::SetColor(UINT instance, Color& color)
{
	colors[instance] = color;
}

void ModelAnimator::UpdateTransforms()
{
	for (UINT i = 0; i < transforms.size(); i++)
		memcpy(worlds[i], transforms[i]->World(), sizeof(Matrix));

	computeWorldBuffer->CopyToInput(worlds);

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(instanceWorldBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, worlds, sizeof(Matrix) * MAX_MODEL_INSTANCE);
	}
	D3D::GetDC()->Unmap(instanceWorldBuffer->Buffer(), 0);

	D3D::GetDC()->Map(instanceColorBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, colors, sizeof(Color) * MAX_MODEL_INSTANCE);
	}
	D3D::GetDC()->Unmap(instanceColorBuffer->Buffer(), 0);
}

void ModelAnimator::GetAttachBones(UINT instance, Matrix* matrix)
{
	memcpy(matrix, attachBones[instance], sizeof(Matrix) * MAX_MODEL_TRANSFORMS);
}

void ModelAnimator::CreateTexture()
{
	clipTransforms = new ClipTransform[model->ClipCount()];
	for (UINT i = 0; i < model->ClipCount(); i++)
		CreateClipTransform(i);

	//Create Texture
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = MAX_MODEL_TRANSFORMS * 4;
		desc.Height = MAX_MODEL_KEYFRAMES;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ArraySize = model->ClipCount();
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;

		UINT pageSize = MAX_MODEL_TRANSFORMS * MAX_MODEL_KEYFRAMES * sizeof(Matrix);

		void* p = VirtualAlloc(nullptr, pageSize * model->ClipCount(), MEM_RESERVE, PAGE_READWRITE);

		for (UINT c = 0; c < model->ClipCount(); c++)
		{
			UINT start = c * pageSize;

			for (UINT k = 0; k < MAX_MODEL_KEYFRAMES; k++)
			{
				void* temp = (BYTE*)p + MAX_MODEL_TRANSFORMS * k * sizeof(Matrix) + start;
				VirtualAlloc(temp, MAX_MODEL_TRANSFORMS * sizeof(Matrix), MEM_COMMIT, PAGE_READWRITE);

				memcpy(temp, clipTransforms[c].Transforms[k], MAX_MODEL_TRANSFORMS * sizeof(Matrix));
			}
		}

		D3D11_SUBRESOURCE_DATA* subResource = new D3D11_SUBRESOURCE_DATA[model->ClipCount()];
		for (UINT c = 0; c < model->ClipCount(); c++)
		{
			void* temp = (BYTE*)p + c * pageSize;
			subResource[c].pSysMem = temp;
			subResource[c].SysMemPitch = MAX_MODEL_TRANSFORMS * sizeof(Matrix);
			subResource[c].SysMemSlicePitch = pageSize;
		}
		Check(D3D::GetDevice()->CreateTexture2D(&desc, subResource, &texture));

		SafeDeleteArray(subResource);
		VirtualFree(p, 0, MEM_RELEASE);
	}

	//Create SRV
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.MipLevels = 1;
		desc.Texture2DArray.ArraySize = model->ClipCount();

		Check(D3D::GetDevice()->CreateShaderResourceView(texture, &desc, &transformsSRV));
	}
}

void ModelAnimator::CreateClipTransform(UINT index)
{
	Matrix* bones = new Matrix[MAX_MODEL_TRANSFORMS];

	ModelClip* clip = model->ClipByIndex(index);

	for (UINT f = 0; f < clip->FrameCount(); f++)
	{
		for (UINT b = 0; b < model->BoneCount(); b++)
		{
			ModelBone* bone = model->BoneByIndex(b);

			Matrix inv = bone->Transform();
			D3DXMatrixInverse(&inv, nullptr, &inv);

			Matrix parent;
			int parentIndex = bone->ParentIndex();

			if (parentIndex < 0)
				D3DXMatrixIdentity(&parent);
			else
				parent = bones[parentIndex];

			Matrix animation;

			ModelKeyFrame* frame = clip->Keyframe(bone->Name());

			if (frame != nullptr)
			{
				ModelKeyFrameData& data = frame->Transforms[f];

				Matrix S, R, T;
				D3DXMatrixScaling(&S, data.Scale.x, data.Scale.y, data.Scale.z);
				D3DXMatrixRotationQuaternion(&R, &data.Rotation);
				D3DXMatrixTranslation(&T, data.Translation.x, data.Translation.y, data.Translation.z);

				animation = S * R * T;
			}
			else
			{
				D3DXMatrixIdentity(&animation);
			}

			bones[b] = animation * parent;
			clipTransforms[index].Transforms[f][b] = inv * bones[b];
			//MeshBone(Comp) * animation(Local) * parent
		}
	}


}
