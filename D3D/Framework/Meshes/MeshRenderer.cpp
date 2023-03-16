#include "Framework.h"
#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(Shader* shader, Mesh* mesh)
	: mesh(mesh)
{
	Pass(0);
	mesh->SetShader(shader);

	instanceWorldBuffer = new VertexBuffer(worlds, MAX_MESH_INSTANCE, sizeof(Matrix), 1, true);
	instanceColorBuffer = new VertexBuffer(colors, MAX_MESH_INSTANCE, sizeof(Color), 2, true);
}

MeshRenderer::~MeshRenderer()
{
	for (Transform* transform : transforms)
		SafeDelete(transform);

	SafeDelete(instanceWorldBuffer);
	SafeDelete(instanceColorBuffer);
	SafeDelete(mesh);
}

void MeshRenderer::Update()
{
	mesh->Update();
}

void MeshRenderer::Render()
{
	instanceWorldBuffer->Render();
	instanceColorBuffer->Render();

	mesh->Render(transforms.size());
}

Transform* MeshRenderer::AddTransform()
{
	Transform* transform = new Transform();
	transforms.push_back(transform);

	colors[transforms.size() - 1] = Color(0, 0, 0, 1);

	return transform;
}

void MeshRenderer::UpdateTransforms()
{
	for (UINT i = 0; i < transforms.size(); i++)
		memcpy(worlds[i], transforms[i]->World(), sizeof(Matrix));

	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(instanceWorldBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, worlds, sizeof(Matrix) * MAX_MESH_INSTANCE);
	}
	D3D::GetDC()->Unmap(instanceWorldBuffer->Buffer(), 0);

	D3D::GetDC()->Map(instanceColorBuffer->Buffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, colors, sizeof(Color) * MAX_MESH_INSTANCE);
	}
	D3D::GetDC()->Unmap(instanceColorBuffer->Buffer(), 0);

}

void MeshRenderer::SetColor(UINT instance, Color& color)
{
	colors[instance] = color;
}


