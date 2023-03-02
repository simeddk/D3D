#include "stdafx.h"
#include "VertexTopologyDemo.h"

void VertexTopologyDemo::Initialize()
{
	shader = new Shader(L"03_Pass.fxo");

	vertices[0].Posision = Vector3(0, 0, 0);
	vertices[1].Posision = Vector3(1, 0, 0);

	vertices[2].Posision = Vector3(0, +0.5f, 0);
	vertices[3].Posision = Vector3(1, +0.5f, 0);

	vertices[4].Posision = Vector3(0, -0.5f, 0);
	vertices[5].Posision = Vector3(1, -0.5f, 0);

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.ByteWidth = sizeof(Vertex) * 6;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA subResource = { 0 };
	subResource.pSysMem = vertices;

	Check(D3D::GetDevice()->CreateBuffer(&desc, &subResource, &vertexBuffer));
}

void VertexTopologyDemo::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
}

void VertexTopologyDemo::Update()
{
	
}

void VertexTopologyDemo::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	//Topology Test
	{
		static bool bStrip = false;
		ImGui::Checkbox("Strip", &bStrip);

		D3D11_PRIMITIVE_TOPOLOGY topologies[2] =
		{
			D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
			D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
		};

		D3D::GetDC()->IASetPrimitiveTopology(bStrip ? topologies[1] : topologies[0]);
	}

	//Pass Test
	static int pass;
	{
		ImGui::InputInt("Pass", &pass);
		pass = (int)Math::Clamp(pass, 0, 3);

		static Color color;
		ImGui::ColorEdit3("Color", color);
		shader->AsVector("CustomColor")->SetFloatVector(color);
	}

	//Start Location Test
	static int startLocation;
	{
		ImGui::SliderInt("Start", &startLocation, 0, 6);
	}


	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	shader->Draw(0, pass, 6, startLocation);
}
