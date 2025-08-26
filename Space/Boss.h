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

	float bonusTime;

	enum class AoEStep { Idle, Windup, Fire, Hold };
	AoEStep m_AoEStep = AoEStep::Idle;

	float m_AoEStepT = 0.f;     // 상태 경과 시간
	float m_AoEAlpha = 0;       // 장판 알파값
	float m_AoEHold = 0.f;      // Hold 시간
	Sprite* m_Pattern = nullptr; // 장판 스프라이트

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

	float SpawnMove;
	bool ones;

	float m_MaxHp;
	float m_Hp;
	float TailHp;
	float BodyHp;
	float TopHp;

	float ChangeTime;

	bool OneDamege;
	float DamegeCoolTime;
	float BonusTime; // 보너스
public:
	Boss(Vec2 Pos);
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

	void EnterAoE();          // AoE 시작할 때 초기화
	void AoEUpdate(float t); // Update에서 상태머신 돌리기
	void AoE_Windup(float t);
	void AoE_Fire();

	void FireWallGrid(int cols, int rows, float spacingX, float spacingY, float speed);
	void FireFanWall();
	float m_Speed;
	bool isMove;

};

