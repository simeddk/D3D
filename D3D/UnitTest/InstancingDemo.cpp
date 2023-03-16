#include "stdafx.h"
#include "InstancingDemo.h"

void InstancingDemo::Initialize()
{
	shader = new Shader(L"19_Instancing.fxo");

	perFramne = new PerFrame(shader);

	material = new Material(shader);
	material->DiffuseMap(L"Box.png");

	for (UINT i = 0; i < MAX_INSTANCING_COUNT; i++)
	{
		transforms[i] = new Transform(shader);

		/*transforms[i]->Position(Math::RandomVec3(-30, 30));
		transforms[i]->Scale(Math::RandomVec3(1.f, 2.5f));
		transforms[i]->RotationDegree(Math::RandomVec3(-180.f, 180.f));*/

		worlds[i] = transforms[i]->World();

		colors[i] = Color(1, 1, 1, 1);
		if (i % 3 == 0)
			colors[i] = Math::RandomColor4();
	}

	CreateMesh();

	instanceWorldBuffer = new VertexBuffer(worlds, MAX_INSTANCING_COUNT, sizeof(Matrix), 1);
	instanceColorBuffer = new VertexBuffer(colors, MAX_INSTANCING_COUNT, sizeof(Color), 2);
}

void InstancingDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(vertexBuffer);
	SafeDelete(indexBuffer);

	SafeDelete(perFramne);
	SafeDelete(material);

	for (UINT i = 0; i < MAX_INSTANCING_COUNT; i++)
		SafeDelete(transforms[i]);

	SafeDelete(instanceWorldBuffer);
	SafeDelete(instanceColorBuffer);
}

void InstancingDemo::Update()
{
	perFramne->Update();
}

void InstancingDemo::Render()
{
	perFramne->Render();
	material->Render();

	vertexBuffer->Render();
	indexBuffer->Render();

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	instanceWorldBuffer->Render();
	instanceColorBuffer->Render();

	shader->DrawIndexedInstanced(0, 2, indices.size(), MAX_INSTANCING_COUNT);
}

void InstancingDemo::CreateMesh()
{
	float w = 0.5f;
	float h = 0.5f;
	float d = 0.5f;

	//Front
	vertices.push_back(Mesh::VertexMesh(-w, -h, -d, 0, 1, 0, 0, -1));
	vertices.push_back(Mesh::VertexMesh(-w, +h, -d, 0, 0, 0, 0, -1));
	vertices.push_back(Mesh::VertexMesh(+w, +h, -d, 1, 0, 0, 0, -1));
	vertices.push_back(Mesh::VertexMesh(+w, -h, -d, 1, 1, 0, 0, -1));

	//Back
	vertices.push_back(Mesh::VertexMesh(-w, -h, +d, 1, 1, 0, 0, 1));
	vertices.push_back(Mesh::VertexMesh(+w, -h, +d, 0, 1, 0, 0, 1));
	vertices.push_back(Mesh::VertexMesh(+w, +h, +d, 0, 0, 0, 0, 1));
	vertices.push_back(Mesh::VertexMesh(-w, +h, +d, 1, 0, 0, 0, 1));

	//Top
	vertices.push_back(Mesh::VertexMesh(-w, +h, -d, 0, 1, 0, 1, 0));
	vertices.push_back(Mesh::VertexMesh(-w, +h, +d, 0, 0, 0, 1, 0));
	vertices.push_back(Mesh::VertexMesh(+w, +h, +d, 1, 0, 0, 1, 0));
	vertices.push_back(Mesh::VertexMesh(+w, +h, -d, 1, 1, 0, 1, 0));

	//Bottom
	vertices.push_back(Mesh::VertexMesh(-w, -h, -d, 1, 1, 0, -1, 0));
	vertices.push_back(Mesh::VertexMesh(+w, -h, -d, 0, 1, 0, -1, 0));
	vertices.push_back(Mesh::VertexMesh(+w, -h, +d, 0, 0, 0, -1, 0));
	vertices.push_back(Mesh::VertexMesh(-w, -h, +d, 1, 0, 0, -1, 0));

	//Left
	vertices.push_back(Mesh::VertexMesh(-w, -h, +d, 0, 1, -1, 0, 0));
	vertices.push_back(Mesh::VertexMesh(-w, +h, +d, 0, 0, -1, 0, 0));
	vertices.push_back(Mesh::VertexMesh(-w, +h, -d, 1, 0, -1, 0, 0));
	vertices.push_back(Mesh::VertexMesh(-w, -h, -d, 1, 1, -1, 0, 0));

	//Right
	vertices.push_back(Mesh::VertexMesh(+w, -h, -d, 0, 1, 1, 0, 0));
	vertices.push_back(Mesh::VertexMesh(+w, +h, -d, 0, 0, 1, 0, 0));
	vertices.push_back(Mesh::VertexMesh(+w, +h, +d, 1, 0, 1, 0, 0));
	vertices.push_back(Mesh::VertexMesh(+w, -h, +d, 1, 1, 1, 0, 0));

	indices = 
	{
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};

	vertexBuffer = new VertexBuffer(&vertices[0], vertices.size(), sizeof(Mesh::VertexMesh));
	indexBuffer = new IndexBuffer(&indices[0], indices.size());
}

