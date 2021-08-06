#pragma once
class MiniBoss : public Object
{
	Sprite* m_Enemy;
	Vec2 m_RandomPosition;
	float m_Hp, m_MaxHp;
	float m_Speed;
	float MoveTime;
	float m_LastMoveTime;
	float AttackTime;
	float AttackDelay;
	bool isBullet;
	bool isMissile;

public:
	MiniBoss(Vec2 Pos);
	~MiniBoss();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);

	void Move();
	void Attack();
	
};

