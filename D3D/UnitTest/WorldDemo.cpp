#include "stdafx.h"
#include "WorldDemo.h"

void WorldDemo::Initialize()
{
	shader = new Shader(L"05_World.fxo");

	//Create Vertex
	{
		vertices[0].Posision = Vector3(-0.5f, -0.5f, 0.0f);
		vertices[1].Posision = Vector3(-0.5f, +0.5f, 0.0f);
		vertices[2].Posision = Vector3(+0.5f, -0.5f, 0.0f);
		vertices[3].Posision = Vector3(+0.5f, +0.5f, 0.0f);

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * 4;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = vertices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));
	}

	//Create Index
	{
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 2;
		indices[4] = 1;
		indices[5] = 3;

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(UINT) * 6;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResource = { 0 };
		subResource.pSysMem = indices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &indexBuffer));
	}

	D3DXMatrixIdentity(&world);
}

void WorldDemo::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
	SafeRelease(indexBuffer);
}

void WorldDemo::Update()
{
#ifdef Matrix Memeber Edit
	if (Keyboard::Get()->Press(VK_SHIFT))
	{
		if (Keyboard::Get()->Press(VK_RIGHT))
			world._11 += 2.0f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_LEFT))
			world._11 -= 2.0f * Time::Delta();

		if (Keyboard::Get()->Press(VK_UP))
			world._22 += 2.0f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_DOWN))
			world._22 -= 2.0f * Time::Delta();
	}
	else
	{
		if (Keyboard::Get()->Press(VK_RIGHT))
			world._41 += 2.0f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_LEFT))
			world._41 -= 2.0f * Time::Delta();

		if (Keyboard::Get()->Press(VK_UP))
			world._42 += 2.0f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_DOWN))
			world._42 -= 2.0f * Time::Delta();
	}
#endif
	static Vector3 position = Vector3(0, 0, 0);
	static Vector3 scale = Vector3(2, 1, 1);

	if (Keyboard::Get()->Press(VK_RIGHT))
		position.x += 2.0f * Time::Delta();
	else if (Keyboard::Get()->Press(VK_LEFT))
		position.x -= 2.0f * Time::Delta();

	if (Keyboard::Get()->Press(VK_UP))
		position.y += 2.0f * Time::Delta();
	else if (Keyboard::Get()->Press(VK_DOWN))
		position.y -= 2.0f * Time::Delta();

	Matrix S, T;
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);
	world = S * T;

	shader->AsMatrix("World")->SetMatrix(world);
	shader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	shader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());
}

void WorldDemo::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	static int pass = 0;
	ImGui::InputInt("Pass", &pass);
	pass %= 2;

	shader->DrawIndexed(0, pass, 6);

}
