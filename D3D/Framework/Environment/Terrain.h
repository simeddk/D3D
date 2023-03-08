#pragma once

class Terrain : public Renderer
{
public:
	Terrain(Shader* shader, wstring heighMapFile);
	~Terrain();

	void Update();
	void Render();

	void BaseMap(wstring file);

private:
	void CreateVertexData();
	void CreateIndexData();
	void CreateNormalData();

private:
	struct VertexTerrain
	{
		Vector3 Position = Vector3(0, 0, 0);
		Vector3 Normal = Vector3(0, 1, 0);
		Vector2 Uv = Vector2(0, 0);
	};

private:
	UINT width, height;

	VertexTerrain* vertices;
	UINT* indices;

	Texture* heightMap;
	Texture* baseMap = nullptr;
};
