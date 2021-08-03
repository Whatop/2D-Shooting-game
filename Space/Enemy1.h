#pragma once
class Enemy1 : public Object
{
	Sprite* m_Enemy;
	Vec2 m_RandomPosition;
	float m_Hp;
	float m_Speed;
	float MoveTime;
	float m_LastMoveTime;
public:
	Enemy1(Vec2 Pos);
	~Enemy1();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);

	void Move();
};

