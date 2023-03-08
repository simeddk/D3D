#pragma once

//------------------------------------------------------------
//ModelBone
//------------------------------------------------------------
class ModelBone
{
public:
	friend class Model;

private:
	ModelBone();
	~ModelBone();

private:
	int index;
	wstring name;

	int parentIndex;
	ModelBone* parent;

	Matrix transform;
	vector<ModelBone*> children;
};

//------------------------------------------------------------
//ModelMesh
//------------------------------------------------------------
class ModelMesh
{
public:
	friend class Model;

private:
	ModelMesh();
	~ModelMesh();

	void Binding(Model* model);

public:
	void Pass(UINT val);

	void SetShader(Shader* shader);

	void Update();
	void Render();

	int BoneIndex() { return boneIndex; }
	void Transforms(Matrix* transforms);

	void SetTransform(Transform* transform);

private:
	struct BoneDesc
	{
		Matrix Transforms[MAX_MODEL_TRANSFORMS];

		UINT BoneIndex;
		float Padding[3];
	} boneDesc;

private:
	Shader* shader;

	Transform* transform = nullptr;
	PerFrame* perFrame = nullptr;

	int boneIndex;
	ModelBone* bone;

	VertexBuffer* vertexBuffer;
	Model::VertexModel* vertices;
	UINT vertexCount;

	IndexBuffer* indexBuffer;
	UINT* indices;
	UINT indexCount;

	ConstantBuffer* boneBuffer;
	ID3DX11EffectConstantBuffer* sBoneBuffer;

	vector<class ModelMeshPart*> meshParts;
};

//------------------------------------------------------------
//ModelMeshPart
//------------------------------------------------------------
class ModelMeshPart
{



};