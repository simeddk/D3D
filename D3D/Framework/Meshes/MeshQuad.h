#pragma once

class MeshQuad : public Mesh
{
public:
	MeshQuad(Shader* shader);
	~MeshQuad();

private:
	virtual void Create() override;

};
