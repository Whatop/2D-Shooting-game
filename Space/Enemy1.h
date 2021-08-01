#pragma once
class Enemy1 : public Object
{
	Sprite* m_Enemy;
	Sprite* m_ColBox[5];
	Vec2 m_RandomPosition;
	float m_Hp;
	float m_Speed;
	float MoveTime;
	float m_LastMoveTime;
	int MoveNum, LEFT, RIGHT, DOWN, UP;
	bool isRight, isLeft, isDown, isUp;
public:
	Enemy1(Vec2 Pos);
	~Enemy1();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);

	void Move();
};

