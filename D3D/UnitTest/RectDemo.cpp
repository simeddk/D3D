#include "stdafx.h"
#include "RectDemo.h"

void RectDemo::Initialize()
{
	shader = new Shader(L"04_Rect.fxo");

	vertices[0].Posision = Vector3(-0.5f, -0.5f, 0.0f);
	vertices[1].Posision = Vector3(-0.5f, +0.5f, 0.0f);
	vertices[2].Posision = Vector3(+0.5f, -0.5f, 0.0f);
	vertices[3].Posision = Vector3(+0.5f, -0.5f, 0.0f);
	vertices[4].Posision = Vector3(-0.5f, +0.5f, 0.0f);
	vertices[5].Posision = Vector3(+0.5f, +0.5f, 0.0f);

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = sizeof(Vertex) * 6;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = vertices;

	Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));

}

void RectDemo::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
}

void RectDemo::Update()
{
}

void RectDemo::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	static int pass = 0;
	ImGui::InputInt("Pass", &pass);
	pass %= 2;

	shader->Draw(0, pass, 6);

}
