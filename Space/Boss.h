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
	Sprite* DestroyTop;

	Animation* PilotAttack;
	Animation* Propeller;

	Sprite* m_ColBox;
	Sprite* ColBox[5];// LEFT COLBOX, RIGHT COLBOX, UP COLBOX, DOWN COLBOX, HIT BOX
	int LEFT, RIGHT, UP, DOWN, HIT;
	bool isLeft, isRight, isUp, isDown, isHit;

	Vec2 Dire;
	
	float DelayTime;
	float ShootTime;
	bool isShoot;
	bool isDire;
	int Count;
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
};

