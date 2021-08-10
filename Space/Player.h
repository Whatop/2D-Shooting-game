#pragma once
class Player : public Object
{
	Sprite* m_Player;// = ColBox
	Sprite* ColBox[5];// LEFT COLBOX, RIGHT COLBOX, UP COLBOX, DOWN COLBOX, HIT BOX
	Sprite* Defense;
	int LEFT, RIGHT, UP, DOWN, HIT;
	bool isLeft, isRight, isUp, isDown, isHit;
	float m_MaxHp, m_Hp, m_Speed, m_Rpm;
	float defenseTime;
	bool ones;
	float TempTime;
	float Damage_Received;

	// 단발, 샷건 , 차지, 유도, 부메랑, 
	int m_GunType;
	int shot, shotgun, charge, induce, boomerang;
	
public:
	Player();
	~Player();
	

	void Init();
	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);
	void Move();
	void CollisionBox();

	void GunType();


	//m_Weapon = int or string
	float RpmDelayTime;
};

