#pragma once
#define GameInfo GameMgr::GetInst()
#define GetPlayer GameMgr::GetInst()->GetPlayerInfo()

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

	int MaxScore;
	int EnemyCount;
	float m_Score;
	float SpawnDelay;
	float AddDelay;


	bool CK_BossSpawn;
	bool CK_MiniBossSpawn;

	RankingPlayer* m_Rank;
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
	
};

