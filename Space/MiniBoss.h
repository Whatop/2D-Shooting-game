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
	enum class P2Step { Exit, Idle, AoE_Windup, AoE_Fire, Return, P1Boost,Hold };

	bool        m_InPhase2 = false;

	// AoE(장판) 관련
	float       m_AoETime = 0.f;     // windup 경과
	Vec2        m_AoETarget;           // 장판 타겟(플레이어 위치 스냅샷)
	int         m_AoEAlpha = 0;       // 0→255 경고 알파

	// 돌진 관련
	int   m_RedlineFires = 0;        // 레드라인 발사 횟수(2회면 복귀)
	bool  m_Phase1Boost = false;    // 1페 강화 모드
	Vec2  m_ReturnPos;               // 복귀 목표 지점
	float m_RotateSpeed = D3DXToRadian(180.f);
	float m_ExitSpeed = 900.f;

	P2Step      m_P2Step = P2Step::Idle;
	float       m_StepT = 0.f;     // 스텝 내 경과시간
	float       m_DashWait = 0.f;     // 2~3초 랜덤 대기
	Vec2        m_DashDir = Vec2(0, 0);
	float       m_DashSpeed = 1200.f;  // 돌진 속도
	float       m_Windup = 0.8f;    // 돌진 준비시간
	float       m_DashDur = 0.35f;   // 돌진 지속
	float       m_Recovery = 0.4f;    // 후딜

	float m_AoEHold = 0.f;      // AoE 유지 시간
	float m_FanSpawnTimer = 0.f; // FireFanWall 주기용
	float m_EnemySpawnTimer = 0.f; // FireFanWall 주기용


	// 충돌 판정 확장
	float       m_HitboxScale = 1.0f;  // 돌진 직후 1.8x로 키웠다가 복구
	Vec2        m_BaseColSize = Vec2(140, 140); // 보스 충돌 기본 크기(프로젝트에 맞춰 조정)

	// 내부 유틸
	void EnterPhase2();
	void Phase2Update(float t);
	void AoE_WindupTick(float t);
	void AoE_FireTick();
	void Dash_WindupTick(float t);
	void DashingTick(float t);
	void RecoverTick(float t);

	void SetHitbox(float scale);
	void StartRandomDashWait(); // 2~3초 랜덤 대기 설정
	float frand(float a, float b) { return a + (b - a) * (float(rand()) / float(RAND_MAX)); }

	// 기존 유틸 선언 옆에 추가
	void FireWallGrid(int cols, int rows, float spacingX, float spacingY, float speed);
	void FireFanWall();

public:
	MiniBoss(Vec2 Pos);
	~MiniBoss();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);

	void Move();
	void Attack();
	
};

