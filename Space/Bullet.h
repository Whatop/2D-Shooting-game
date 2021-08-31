#pragma once
class Bullet : public Object
{
	Animation* m_Bolt;
	Sprite* m_Bullet;
	Vec2 Dire;
	Vec2 Spawnpoint;
	float m_Speed;
	float DelayTime;
	float DestroyTime;
public:
	Bullet(Vec2 spawnpoint = GetPlayer->m_Position, bool minibullet = false);
	~Bullet();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);

	void Move();
};

 