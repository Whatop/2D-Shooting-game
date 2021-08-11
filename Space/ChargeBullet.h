#pragma once
class ChargeBullet : public Object
{
	Sprite* m_ChargeBullet;
	Vec2 Dire;
	Vec2 Spawnpoint;
	float m_Speed;
	float DelayTime;
	float DestroyTime;
	bool OneCharge;
public:
	ChargeBullet();
	~ChargeBullet();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);

	void Move();
};

