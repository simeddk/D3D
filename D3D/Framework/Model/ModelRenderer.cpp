#include "Framework.h"
#include "ModelRenderer.h"

ModelRenderer::ModelRenderer(Shader* shader)
	: shader(shader)
{
	model = new Model();

	transform = new Transform(shader);
}

ModelRenderer::~ModelRenderer()
{
	SafeDelete(model);
	SafeDelete(transform);
}

void ModelRenderer::Update()
{
	if (bRead == true)
	{
		bRead = false;

		for (ModelMesh* mesh : model->Meshes())
			mesh->SetShader(shader);

		UpdateTransforms();
	}

	for (ModelMesh* mesh : model->Meshes())
		mesh->Update();
}

void ModelRenderer::Render()
{
	for (ModelMesh* mesh : model->Meshes())
	{
		mesh->SetTransform(transform);
		mesh->Render();
	}
}

void ModelRenderer::ReadMesh(wstring file)
{
	model->ReadMesh(file);
}

void ModelRenderer::ReadMaterial(wstring file)
{
	model->ReadMaterial(file);
	bRead = true;
}

void ModelRenderer::Pass(UINT pass)
{
	for (ModelMesh* mesh : model->Meshes())
		mesh->Pass(pass);
}

void ModelRenderer::UpdateTransforms()
{
	for (UINT i = 0; i < model->BoneCount(); i++)
	{
		ModelBone* bone = model->BoneByIndex(i);
		transforms[i] =  bone->Transform();
	}

	for (ModelMesh* mesh : model->Meshes())
		mesh->Transforms(transforms);
}
