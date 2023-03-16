#pragma once
#include "Systems/IExecute.h"

#define MAX_INSTANCING_COUNT 5000

class InstancingDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {};
	virtual void Render() override;
	virtual void PostRender() override {};
	virtual void ResizeScreen() override {};

private:
	void CreateMesh();

private:
	Shader* shader;
	Material* material;

	vector<Mesh::VertexMesh> vertices;
	vector<UINT> indices;

	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	VertexBuffer* instanceWorldBuffer;
	VertexBuffer* instanceColorBuffer;

	PerFrame* perFramne;
	Transform* transforms[MAX_INSTANCING_COUNT];
	Matrix worlds[MAX_INSTANCING_COUNT];
	Color colors[MAX_INSTANCING_COUNT];
};