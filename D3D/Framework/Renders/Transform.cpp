#include "Framework.h"
#include "Transform.h"

Transform::Transform()
	: shader(nullptr)
	, buffer(nullptr)
	, position(0, 0, 0)
	, scale(1, 1, 1)
	, rotation(0, 0, 0)
{
	D3DXMatrixIdentity(&bufferDesc.World);
}

Transform::Transform(Shader* shader)
	: position(0, 0, 0)
	, scale(1, 1, 1)
	, rotation(0, 0, 0)
{
	SetShader(shader);
	D3DXMatrixIdentity(&bufferDesc.World);
}

Transform::~Transform()
{
	SafeDelete(buffer);
}

void Transform::Set(Transform* transform)
{
	position = transform->position;
	scale = transform->scale;
	rotation = transform->rotation;

	UpdateWorld();
}

void Transform::SetShader(Shader* shader)
{
	this->shader = shader;

	buffer = new ConstantBuffer(&bufferDesc, sizeof(BufferDesc));
	sBuffer = this->shader->AsConstantBuffer("CB_World");
}

void Transform::Position(float x, float y, float z)
{
}

void Transform::Position(Vector3& vec)
{
}

void Transform::Position(Vector3* vec)
{
}

void Transform::Rotation(float x, float y, float z)
{
}

void Transform::Rotation(Vector3& vec)
{
}

void Transform::Rotation(Vector3* vec)
{
}

void Transform::RotationDegree(float x, float y, float z)
{
}

void Transform::RotationDegree(Vector3& vec)
{
}

void Transform::RotationDegree(Vector3* vec)
{
}

void Transform::Scale(float x, float y, float z)
{
}

void Transform::Scale(Vector3& vec)
{
}

void Transform::Scale(Vector3* vec)
{
}

Vector3 Transform::Forward()
{
	return Vector3(bufferDesc.World._31, bufferDesc.World._32, bufferDesc.World._33);
}

Vector3 Transform::Up()
{
	return Vector3(bufferDesc.World._21, bufferDesc.World._22, bufferDesc.World._23);
}

Vector3 Transform::Right()
{
	return Vector3(bufferDesc.World._11, bufferDesc.World._12, bufferDesc.World._13);
}

void Transform::World(Matrix& matrix)
{
	//D3DXMatrixDecompose(,)
	Math::MatrixDecompose()
}

void Transform::UpdateWorld()
{
	Matrix S, R, T;
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixRotationYawPitchRoll(&R, rotation.y, rotation.x, rotation.z);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);

	Matrix world = S * R * T;
	bufferDesc.World = world;
}

void Transform::Update()
{
}

void Transform::Render()
{
	if (shader == nullptr) return;

	buffer->Render();
	sBuffer->SetConstantBuffer(buffer->Buffer());
}
