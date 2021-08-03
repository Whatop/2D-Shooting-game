#pragma once
class RotationBullet : public Object
{
	Sprite* m_Bullet;
	Vec2 m_Dire;
	float m_Speed;
public:
	RotationBullet(Vec2 Pos,float r);
	~RotationBullet();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);

	void Move();
};

