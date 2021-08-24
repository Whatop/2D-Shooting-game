#pragma once
class RevolutionBullet : public Object
{
	Sprite* m_RBullet;
	float m_Speed;
	float KeepRotation;
	Vec2 m_Dire;
public:
	RevolutionBullet(float r);
	~RevolutionBullet();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);

	void Move();
};

