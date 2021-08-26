#pragma once
class EliteEnemy1 : public Object
{
	Sprite* m_EliteEnemy1;
	float m_Hp;
	float m_Speed;

	float MoveTime;
	float m_LastMoveTime;
	float SpawnMove;
	bool ones;

	Vec2 m_RandomPosition;

	bool OneDamege;
	bool DamegeCoolTime;
public:
	EliteEnemy1(Vec2 Pos);
	~EliteEnemy1();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);

	void Move();
};

