#pragma once
class DoubleBullet : public Object
{
	Animation* m_Spark;
	Sprite* m_DoubleBullet;
	Vec2 Dire;
	Vec2 Spawnpoint;
	float m_Speed;
	float DelayTime;
	float DestroyTime;
public:
	DoubleBullet(Vec2 Pos);
	~DoubleBullet();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);

	void Move();
};


