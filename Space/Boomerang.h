#pragma once
class Boomerang : public Object
{
	Sprite* m_Boomerang;
	Vec2 Dire;
	Vec2 Spawnpoint;
	float m_Speed;
	float DelayTime;
	float DestroyTime;
public:
	Boomerang();
	~Boomerang();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);

	void Move();
};

