#pragma once

class MeshCube : public Mesh
{
public:
	MeshCube();
	~MeshCube();

private:
	virtual void Create() override;

};
