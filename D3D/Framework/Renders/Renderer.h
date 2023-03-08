#pragma once

class Renderer
{
public:
	Renderer(Shader* shader);
	Renderer(wstring shaderFile);
	virtual ~Renderer();

private:
	void Initialize();

public:
	Shader* GetShader() { return shader; }

	UINT& Pass() { return pass; }
	void Pass(UINT val) { pass = val; }

	virtual void Update();
	virtual void Render();

	Transform* GetTransform() { return transform; }

protected:
	void Topology(D3D11_PRIMITIVE_TOPOLOGY val) { topology = val; }

protected:
	Shader* shader;

	Transform* transform;
	VertexBuffer* vertexBuffer = nullptr;
	IndexBuffer* indexBuffer = nullptr;

	UINT vertexCount = 0;
	UINT indexCount = 0;

private:
	PerFrame* perFrame;

	UINT pass = 0;
	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	bool bCreatedShader = false;
};
