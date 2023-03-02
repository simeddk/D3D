#include "stdafx.h"
#include "TriangleDemo.h"

void TriangleDemo::Initialize()
{
	shader = new Shader(L"03_Pass.fxo");

	//Tri 1
	{
		vertices[0].Posision = Vector3(-0.5f, +0.0f, 0.0f);
		vertices[1].Posision = Vector3(+0.0f, +0.5f, 0.0f);
		vertices[2].Posision = Vector3(+0.5f, +0.0f, 0.0f);

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * 3;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}

	//Tri 2
	{
		vertices2[0].Posision = Vector3(-0.5f, +0.0f - 0.5f, 0.0f);
		vertices2[1].Posision = Vector3(+0.0f, +0.5f - 0.5f, 0.0f);
		vertices2[2].Posision = Vector3(+0.5f, +0.0f - 0.5f, 0.0f);

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * 3;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices2;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer2));
	}
}

void TriangleDemo::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
	SafeRelease(vertexBuffer2);
}

void TriangleDemo::Update()
{
}

void TriangleDemo::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	shader->Draw(0, 0, 3);

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer2, &stride, &offset);
	shader->Draw(0, 1, 3);
}
