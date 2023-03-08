#include "Framework.h"
#include "ModelMesh.h"

//------------------------------------------------------------
//ModelBone
//------------------------------------------------------------
ModelBone::ModelBone()
{
	
}

ModelBone::~ModelBone()
{

}

//------------------------------------------------------------
//ModelMesh
//------------------------------------------------------------
ModelMesh::ModelMesh()
{
	boneBuffer = new ConstantBuffer(&boneDesc, sizeof(BoneDesc));
}

ModelMesh::~ModelMesh()
{
	SafeDelete(transform);
	SafeDelete(perFrame);

	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);

	SafeDelete(vertexBuffer);
	SafeDelete(indexBuffer);

	SafeDelete(boneBuffer);

	for (ModelMeshPart* part : meshParts)
		SafeDelete(part);
}

void ModelMesh::Binding(Model* model)
{
	vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(Model::VertexModel));
	indexBuffer = new IndexBuffer(indices, indexCount);

	for (ModelMeshPart* part : meshParts)
		part->Binding(model);
}

void ModelMesh::Pass(UINT val)
{
	for (ModelMeshPart* part : meshParts)
		part->Pass(val);
}

void ModelMesh::SetShader(Shader* shader)
{
	this->shader = shader;

	//Todo
}

void ModelMesh::Update()
{
}

void ModelMesh::Render()
{
}

void ModelMesh::Transforms(Matrix* transforms)
{
}

void ModelMesh::SetTransform(Transform* transform)
{
}
