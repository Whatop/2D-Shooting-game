#pragma once
class MiniBoss : public Object
{
	Sprite* m_Enemy;

	Sprite* m_Pattern;

	Vec2 m_RandomPosition;
	float m_Hp, m_MaxHp;
	float m_Speed;
	float MoveTime;
	float m_LastMoveTime;
	float AttackTime;
	float AttackDelay;
	float SpawnMove;

	bool ones;
	bool isBullet;
	bool isMissile;
	int pattern;
	
	bool OneDamege;
	float DamegeCoolTime;

	bool isPattern;
	float Acc;
	bool One;
	
	int PatternCount;
	float PatternTime;

	bool Over;
	bool Under;

	float BulletPattern;
public:
	MiniBoss(Vec2 Pos);
	~MiniBoss();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);

	void Move();
	void Attack();
	
};

