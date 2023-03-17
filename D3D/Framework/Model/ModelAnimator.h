#pragma once

class ModelAnimator
{
public:
	ModelAnimator(Shader* shader);
	~ModelAnimator();

	void Update();
	void Render();

private:
	void UpdateAnimationFrame(UINT instance);
	void UpdateBlendingFrame(UINT instance);

public:
	void ReadMesh(wstring file);
	void ReadMaterial(wstring file);
	void ReadClip(wstring file);

	void PlayTweenMode(UINT instance, UINT clip, float speed = 1.f, float takeTime = 1.f);
	void PlayBlendMode(UINT instance, UINT clip1, UINT clip2, UINT clip3);
	void SetBlendAlpha(UINT instance, float alpha);

public:
	void Pass(UINT pass);
	
	Transform* AddTransform();
	Transform* GetTransform(UINT instance) { return transforms[instance]; }
	void UpdateTransforms();
	void SetColor(UINT instance, Color& color);

	UINT TransformCount() { return transforms.size(); }

	Model* GetModel() { return model; }

	void GetAttachBones(UINT instance, Matrix* matrix);

private:
	void CreateTexture();
	void CreateClipTransform(UINT index);

private:
	struct ClipTransform
	{
		Matrix** Transforms;

		ClipTransform()
		{
			Transforms = new Matrix*[MAX_MODEL_KEYFRAMES];

			for (UINT i = 0; i < MAX_MODEL_KEYFRAMES; i++)
				Transforms[i] = new Matrix[MAX_MODEL_TRANSFORMS];
		}

		~ClipTransform()
		{
			for (UINT i = 0; i < MAX_MODEL_KEYFRAMES; i++)
				SafeDeleteArray(Transforms[i]);

			SafeDeleteArray(Transforms);
		}
	};
	ClipTransform* clipTransforms;

private:
	ID3D11Texture2D* texture = nullptr;
	ID3D11ShaderResourceView* transformsSRV;
	ID3DX11EffectShaderResourceVariable* sTransformsSRV;

private:
	struct KeyFrameDesc
	{
		int Clip = 0;

		UINT CurrFrame = 0;
		UINT NextFrame = 0;

		float Time = 0.f;
		float RunningTime = 0.f;

		float Speed = 1.f;

		Vector2 Padding;
	};

private:
	struct TweenDesc
	{
		float TakeTime = 0.1f;
		float TweenTime = 0.f;
		float ChangeTime = 0.f;
		float Padding;

		KeyFrameDesc Curr;
		KeyFrameDesc Next;

		TweenDesc()
		{
			Curr.Clip = 0;
			Next.Clip = -1;
		}
	} tweenDesc[MAX_MODEL_INSTANCE];
	
	ConstantBuffer* frameBuffer;
	ID3DX11EffectConstantBuffer* sFrameBuffer;

private:
	struct BlendDesc
	{
		UINT Mode = 0;
		float Alpha = 0;
		Vector2 Padding;

		KeyFrameDesc Clip[3];
	} blendDesc[MAX_MODEL_INSTANCE];

	ConstantBuffer* blendBuffer;
	ID3DX11EffectConstantBuffer* sBlendBuffer;

private:
	Shader* shader;
	Model* model;

	vector<Transform*> transforms;
	Matrix worlds[MAX_MESH_INSTANCE];
	VertexBuffer* instanceWorldBuffer;

	Color colors[MAX_MESH_INSTANCE];
	VertexBuffer* instanceColorBuffer;

private:
	float frameRate = 60.f;
	float frameTime = 0.f;

	Matrix** attachBones;

	Shader* computeShader;

	ID3DX11EffectMatrixVariable* sComputeWorld; //ActorTransform -> CS
	ID3DX11EffectConstantBuffer* sComputeFrameBuffer; //Clip ~ Clip -> CS
	ID3DX11EffectConstantBuffer* sComputeBlendBuffer; //BlendSpace -> CS
	ID3DX11EffectShaderResourceVariable* sComputeTransformsSRV; //x(Bone), y(Frame), z(clip) -> CS

	StructuredBuffer* computeWorldBuffer; //Instancing ActorTransform(worlds[]) -> CS Input
	ID3DX11EffectShaderResourceVariable* sComputeInputWorldBuffer; //compteWorldBuffer -> CS SRV

	StructuredBuffer* computeBoneBuffer; //model->BoneByIndex(i) -> CS Input
	ID3DX11EffectShaderResourceVariable* sComputeInputBoneBuffer; //CS SRV

	TextureBuffer* computeOutputBuffer; //CS Output
	ID3DX11EffectUnorderedAccessViewVariable* sComputeOutputBuffer; //CS UAV
};
