#pragma once
#define GameInfo GameMgr::GetInst()
#define GetPlayer GameMgr::GetInst()->GetPlayerInfo()

enum class StageScene {
	STAGE1,
	STAGE2,
	STORE,
	NONE
};

struct RankingPlayer
{
public:
	int score;
	std::string name;
};

class GameMgr : public Singleton<GameMgr>
{
	Object* PlayerInfo;

	float MaxHp, Hp;
	float BossMaxHp, BossHp;
	float MiniBossMaxHp, MiniBossHp;
public:
	GameMgr();
	~GameMgr();

	std::vector<RankingPlayer*> Ranks;

	bool m_isCreatePlayer;
	bool m_isCreateUI;
	bool m_DebugMode;
	bool AutoCamera;
	bool CameraStop;
	bool isBossSpawn;
	bool isMiniBossSpawn;
	bool isOneBoss;
	bool isOneMiniBoss;
	
	bool isPause;

	int MaxScore;

	int EnemyCount;

	float m_Score;
	float TempScore;
	// float EnemyScore,TtemScore, Nohitbonus 추가해야될것

	float Player_Coefficient; // 플레이어 공격력 계수

	float SpawnDelay;
	float AddDelay;
	
	int ChargeCount;

	bool CK_BossSpawn;
	bool CK_MiniBossSpawn;
		
	bool isNoHit; //노히트 판정체크

	int HV_ShotType[6];

	int HV_TYPE;
	bool isGunType; // 타입 선택시 바꾸는거 한번만

	bool IT_Shot;
	bool IT_Shotgun;
	bool IT_Charge;
	bool IT_Induce;
	bool IT_Boomerang;
	bool IT_Doubleshot;

	bool isSpawnEnemy;

	RankingPlayer* m_Rank;
	StageScene m_Scene;
	Vec2 BossPosition;

	float ChargeTime;
	bool isChoiceDelay;

	int PetCount;

	bool UP_Charge;

public:
	void Init();
	void Release();

	void CreateUI();
	void ReleaseUI();

public:
	void CreatePlayer();
	void PlayerDeath();
	Object* GetPlayerInfo() { return PlayerInfo; }
	void PlayerUpdate(Object* obj) { PlayerInfo = obj; }
	void PlayerHpUpdate(float maxhp,float hp) { MaxHp = maxhp, Hp = hp; }
	float GetPlayerMaxHp() { return MaxHp; }
	float GetPlayerHp() { return Hp; }
public:
	void BossHpUpdate(float maxhp,float hp) { BossMaxHp = maxhp, BossHp = hp; }
	void MiniBossHpUpdate(float maxhp,float hp) { MiniBossMaxHp = maxhp, MiniBossHp = hp; }
	float GetBossMaxHp() { return BossMaxHp; }
	float GetBossHp() { return BossHp; }
	float GetMiniBossMaxHp() { return MiniBossMaxHp; }
	float GetMiniBossHp() { return MiniBossHp; }
	void SpawnEnemy();
public:
	void Update();
	void Render();

	void AddScore(int score);
	void RankInit();
	void SortRanking();
	
	void CheatKey();

	void AddCard(int card);

	void Reset();
	void BossReset();
	void GunReset();

	void ChocieScene();

	void RemoveCharge();
};

