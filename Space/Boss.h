#pragma once
class Boss : public Object
{
	Sprite* m_Boss;

	Sprite* BossBody;
	Sprite* BossTail;
	Sprite* BossWindow;
	Sprite* BossBehind;
	
	Sprite* DestroyBody;
	Sprite* DestroyTail;
	Sprite* ColBoxTop;

	Animation* PilotAttack;
	Animation* Propeller;

	Sprite* m_ColBox;
	Sprite* ColBox[5];// LEFT COLBOX, RIGHT COLBOX, UP COLBOX, DOWN COLBOX, HIT BOX
	int LEFT, RIGHT, UP, DOWN, HIT;
	bool isLeft, isRight, isUp, isDown, isHit;

	Vec2 Dire;
	Vec2 m_RandomPosition;
	float m_MoveWaitingTime;
	float m_LastMoveTime;
	float MoveTime;
	int MoveNum;
	bool isDestroyTop;
	bool isDestroyTail;
	bool isDestroyBody;

	float DelayTime;
	float MS_DelayTime;
	float MS_RpmTime;
	int MS_Num;
	
	float ShootTime;
	float DestroyTime;
	float EffectTime;

	bool isShoot;
	bool isDire;
	int Count;
	bool isBoom;
	bool DieScene;


	float m_MaxHp;
	float m_Hp;
	float TailHp;
	float BodyHp;
	float TopHp;
public:
	Boss();
	~Boss();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);

	void Move();
	void Fire();
	void State();
	void SpawnObstacle();
	void SpawnMissile();
	void DestroyEffect();

	float m_Speed;
	bool isMove;

};

