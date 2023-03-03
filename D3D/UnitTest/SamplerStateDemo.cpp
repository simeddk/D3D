#include "stdafx.h"
#include "SamplerStateDemo.h"

void SamplerStateDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(0, 0, 0);
	Context::Get()->GetCamera()->Position(0, 0, -5);
	dynamic_cast<Freedom*>(Context::Get()->GetCamera())->Speed(5);

	shader = new Shader(L"07_SamplerState.fxo");

	//Create Vertex
	{
		vertices[0].Posision = Vector3(-0.5f, -0.5f, 0.0f);
		vertices[1].Posision = Vector3(-0.5f, +0.5f, 0.0f);
		vertices[2].Posision = Vector3(+0.5f, -0.5f, 0.0f);
		vertices[3].Posision = Vector3(+0.5f, +0.5f, 0.0f);

		vertices[0].Uv = Vector2(0, 2);
		vertices[1].Uv = Vector2(0, 0);
		vertices[2].Uv = Vector2(2, 2);
		vertices[3].Uv = Vector2(2, 0);

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

	texture = new Texture(L"Box.png");
}

void SamplerStateDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(texture);

	SafeRelease(vertexBuffer);
	SafeRelease(indexBuffer);
}

void SamplerStateDemo::Update()
{
	if (ImGui::Button("Open Texture"))
	{
		function<void(wstring)> onOpenFileDialog = bind(&SamplerStateDemo::OnOpenFileDialog, this, placeholders::_1);
		Path::OpenFileDialog(L"", Path::ImageFilter, L"../../_Textures/", onOpenFileDialog, D3D::GetDesc().Handle);
	}

	static UINT address = 0;
	ImGui::InputInt("Address", (int*)&address);
	address = Math::Clamp(address, 0, 3);
	shader->AsScalar("Address")->SetInt(address);

	static UINT filter = 0;
	ImGui::InputInt("Filter", (int*)&filter);
	filter = Math::Clamp(filter, 0, 1);
	shader->AsScalar("Filter")->SetInt(filter);

	shader->AsMatrix("World")->SetMatrix(world);
	shader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	shader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());
}

void SamplerStateDemo::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	shader->AsSRV("BaseMap")->SetResource(texture->SRV());

	shader->DrawIndexed(0, 1, 6);

}

void SamplerStateDemo::OnOpenFileDialog(wstring path)
{
	SafeDelete(texture);
	texture = new Texture(path);
}
