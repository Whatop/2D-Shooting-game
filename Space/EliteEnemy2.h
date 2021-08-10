#pragma once
class EliteEnemy2 : public Object
{
	Sprite* m_EliteEnemy2;
	float m_Hp;
	float m_Speed;

	float MoveTime;
	float m_LastMoveTime;
	float SpawnMove;
	bool ones;

	bool isAttack; 

	Vec2 m_RandomPosition;
public:
	EliteEnemy2(Vec2 Pos);
	~EliteEnemy2();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);

	void Move();
	void Attack();
};

