#pragma once
class MiniMissile : public Object
{
	Animation* m_Missile;
	Sprite* m_ColBox;

	Vec2 Enemy, Dire, Rad;
	float turnRadian, vrad, Delay;
	float impellent;
	float HomingTime;
	float m_Speed;
	float DestroyTime;
	bool isHoming;
	
public:
	MiniMissile(Vec2 Pos,float r=190);
	~MiniMissile();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* other);

	void Move();
};

