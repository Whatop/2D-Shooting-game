#pragma once
class Enemy3 : public Object
{
	Sprite* m_Enemy3;
	float m_Hp;
	float m_Speed;

	float MoveTime;
	float m_LastMoveTime;
	float SpawnMove;
	bool isBoomMode;
	int Changecount;
	bool ones;
	Vec2 m_RandomPosition;

	// 미사일 코드
	Vec2 Enemy, Dire, Rad;
	float turnRadian, vrad, Delay;
	float impellent;
	float HomingTime;
	bool isHoming;
	float DestroyTime;
public:
	Enemy3(Vec2 Pos);
	~Enemy3();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);

	void Move();
};

