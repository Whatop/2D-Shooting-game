#pragma once
class Boss : public Object
{
	Sprite* m_Boss;

	Sprite* BossBody;
	Sprite* BossTail;

	Sprite* BossBehind;
	
	Sprite* DestroyBody;
	Sprite* DestroyTail;
	Sprite* DestroyTop;

	Animation* PilotAttack;
	Animation* Propeller;



	Vec2 Dire;
	
	float DelayTime;
	float ShootTime;
	bool isShoot;

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

