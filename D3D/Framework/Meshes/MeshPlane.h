#pragma once

class MeshPlane : public Mesh
{
public:
	MeshPlane(Shader* shader, float offsetU = 1.f, float offsetV = 1.f);
	~MeshPlane();

private:
	virtual void Create() override;

private:
	float offsetU, offsetV;
};
