#pragma once
class ShotGun : public Object
{
	Animation* m_ShotGun;
	Sprite* m_ColBox;
	Vec2 Dire;
	Vec2 Spawnpoint;
	float m_Speed;
	float DelayTime;
	float DestroyTime;
public:
	ShotGun(float r,Vec2 spawnPos = GetPlayer->m_Position);
	~ShotGun();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);

	void Move();
};

