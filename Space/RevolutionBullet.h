#pragma once
class RevolutionBullet : public Object
{
	Sprite* m_RBullet;
	float KeepRotation;
	Vec2 Dire;
	Vec2 Spawnpoint;
	float m_Speed;
	float DelayTime;
	float DestroyTime;
	bool OneCharge;
public:
	RevolutionBullet(float r);
	~RevolutionBullet();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);

	void Move();
	void RMove();
};

