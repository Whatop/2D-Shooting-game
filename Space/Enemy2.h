#pragma once
class Enemy2 : public Object
{
	Sprite* m_Enemy2;
	float m_Hp;
	float m_Speed;

	float MoveTime;
	float m_LastMoveTime;

	float SpawnMove;
	bool ones;

	Vec2 m_RandomPosition;


	bool OneDamege;
	float DamegeCoolTime;
public:
	Enemy2(Vec2 Pos);
	~Enemy2();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);

	void Move();
};

