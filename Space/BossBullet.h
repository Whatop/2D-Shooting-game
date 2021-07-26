#pragma once
class BossBullet : public Object
{
	Animation* m_BossBullet;
	float DestroyTime;
	float m_Speed;
	Vec2 Dire;
public:
	BossBullet(Vec2 spawnPos, Vec2 dir);
	~BossBullet();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);

	void Move();

};

