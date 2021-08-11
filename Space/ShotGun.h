#pragma once
class ShotGun : public Object
{
	Sprite* m_ShotGun;
	Vec2 Dire;
	Vec2 Spawnpoint;
	float m_Speed;
	float DelayTime;
	float DestroyTime;
public:
	ShotGun(float r);
	~ShotGun();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);

	void Move();
};

