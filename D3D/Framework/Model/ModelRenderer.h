#pragma once

class ModelRenderer
{
public:
	ModelRenderer(Shader* shader);
	~ModelRenderer();

	void Update();
	void Render();

public:
	void ReadMesh(wstring file);
	void ReadMaterial(wstring file);

	Model* GetModel() { return model; }

	Transform* AddTransform();
	Transform* GetTransform(UINT instance) { return transforms[instance]; }
	void UpdateTransforms();
	void SetColor(UINT instance, Color& color);

	UINT TransmformCount() { return transforms.size(); }

	void Pass(UINT pass);

private:
	void CreateTexture();

private:
	Shader* shader;
	Model* model;

	vector<Transform*> transforms;
	Matrix worlds[MAX_MESH_INSTANCE];
	VertexBuffer* instanceWorldBuffer;

	Color colors[MAX_MESH_INSTANCE];
	VertexBuffer* instanceColorBuffer;
	
	Matrix boneTransforms[MAX_MODEL_INSTANCE][MAX_MODEL_TRANSFORMS];

	ID3D11Texture2D* texture = nullptr;
	ID3D11ShaderResourceView* transformsSRV;
	ID3DX11EffectShaderResourceVariable* sTransformsSRV;
};