#pragma once

#include "Systems/IExecute.h"

class RtvDemo : public IExecute
{
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override {};
	virtual void ResizeScreen() override {};

private:
	void Mesh();
	void Airplane();
	void Kachujin();
	void Weapon();

	void Pass(UINT val);

private:
	Shader* shader;

	RenderTarget* rtv;
	DepthStencil* dsv;

	CubeSky* sky;

	Material* floor;
	Material* stone;
	Material* brick;
	Material* wall;

	MeshRenderer* cube;
	MeshRenderer* plane;
	MeshRenderer* cylinder;
	MeshRenderer* sphere;

	ModelRenderer* airplane;

	ModelAnimator* kachujin;
	ModelRenderer* weapon;
	Transform* weaponTransform;

	vector<MeshRenderer*> meshes;
	vector<ModelRenderer*> models;
	vector<ModelAnimator*> animators;
};