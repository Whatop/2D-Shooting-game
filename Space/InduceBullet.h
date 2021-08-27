#pragma once
class InduceBullet : public Object
{
	Animation* m_Induce;
	Sprite* m_InduceBullet;
	Vec2 Dire;
	Vec2 Spawnpoint;
	float m_Speed;
	float DelayTime;
	float DestroyTime;

	Vec2 Enemy, Rad;
	float turnRadian, vrad, Delay;
	float impellent;
	float HomingTime;
	bool isHoming;

public:
	InduceBullet(Vec2 spawnpoint = GetPlayer->m_Position);
	~InduceBullet();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);

	void Iduce();
	void Move();
};

