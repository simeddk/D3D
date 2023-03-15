#pragma once

struct ColliderObject
{
public:
	ColliderObject();
	~ColliderObject();

public:
	class Collider* Collision = nullptr;
	class Transform* World = nullptr;
	class Transform* Init = nullptr;
};

class Collider
{
public:
	Collider(Transform* transform, Transform* init = nullptr);
	~Collider();

	void Render(Color color = Color(0, 1, 0, 1));

private:
	Transform* transform;
	Transform* init = nullptr;

	Vector3 lines[8];
};