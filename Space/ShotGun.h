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
	ShotGun(float r,Vec2 spawnPos , bool minibullet);
	~ShotGun();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);

	void Move();
};

