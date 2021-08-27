#pragma once
class Boomerang : public Object
{
	Animation* m_Crossed;
	Sprite* m_Boomerang;
	Vec2 Dire;
	Vec2 Spawnpoint;
	float m_Speed;
	float DelayTime;
	float DestroyTime;
public:
	Boomerang(Vec2 spawnpoint = GetPlayer->m_Position);
	~Boomerang();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);

	void Move();
};

