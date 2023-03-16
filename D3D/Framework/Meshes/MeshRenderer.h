#pragma once

#define MAX_MESH_INSTANCE 500

class MeshRenderer
{
public:
	MeshRenderer(Shader* shader, Mesh* mesh);
	~MeshRenderer();

	Mesh* GetMesh() { return mesh; }

	void Update();
	void Render();

	void Pass(UINT val) { mesh->Pass(val); }

	Transform* AddTransform();
	Transform* GetTransform(UINT index) { return transforms[index]; }
	void UpdateTransforms();

	void SetColor(UINT instance, Color& color);

private:
	Mesh* mesh;

	vector<Transform*> transforms;
	Matrix worlds[MAX_MESH_INSTANCE];
	VertexBuffer* instanceWorldBuffer;

	Color colors[MAX_MESH_INSTANCE];
	VertexBuffer* instanceColorBuffer;
};
