#pragma once
class Player : public Object
{
	Sprite* m_Player;// = ColBox
	Sprite* ColBox[5];// LEFT COLBOX, RIGHT COLBOX, UP COLBOX, DOWN COLBOX, HIT BOX
	bool AutoCamera;
	int LEFT, RIGHT, UP, DOWN, HIT;
	bool isLeft, isRight, isUp, isDown, isHit;
public:
	Player();
	~Player();
	

	void Init();
	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);
	void Move();
	void CollisionBox();

	//m_Weapon = int or string
	float m_Hp, m_Speed;
};

