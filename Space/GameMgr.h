#pragma once

#define GameInfo GameMgr::GetInst()
#define GetPlayer GameMgr::GetInst()->GetPlayerInfo()

struct PendingShop {
	int goldCost = 0;          // 총 차감 예정 골드
	int cardUp[6] = { 0 };      // 무기 타입별 카드 업그레이드 개수
	int petAdd[6] = { 0 };      // 무기 타입별 펫 추가 개수

	void Clear() {
		goldCost = 0;
		for (int i = 0; i < 6; ++i) { cardUp[i] = 0; petAdd[i] = 0; }
	}
};
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
	PendingShop pendingShop;   // 상점에서 누적시켜둘 요청 버킷
	Object* PlayerInfo;

	float MaxHp, Hp;
	float BossMaxHp, BossHp;
	float MiniBossMaxHp, MiniBossHp;
public:
	GameMgr();
	~GameMgr();

	Vec2 MoneyPokeyPos;
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
	bool played;

	int MaxScore;
	int MaxMoney;

	int EnemyCount;

	float m_Score;
	float TempScore;
	float KillScore, ItemScore, BonusScore;
	
	float m_Money;

	float Player_Coefficient; // 플레이어 공격력 계수
	float Pet_Coefficient; // 펫 공격력 계수
	float Enemy_Coefficient; // 적 공격력 계수

	float SpawnDelay;
	float AddDelay;
	
	int ChargeCount;

	bool CK_BossSpawn;
	bool CK_MiniBossSpawn;
		
	bool isNoHit; //노히트 판정체크

	int HV_ShotType[6];

	int HV_TYPE;

	int HV_Boom;
	int Stage = 1;   // 현재 스테이지

	bool isGunType; // 타입 선택시 바꾸는거 한번만
	std::vector<int> OwnedPetTypes;  // 구입한 펫들의 공격타입을 순서대로 보관
	// 편의 함수(원하면 .cpp로 빼도 됨)
	void AddOwnedPetType(int type) { OwnedPetTypes.push_back(type); }
	void ClearOwnedPetTypes() { OwnedPetTypes.clear(); PetCount = 0; }

	void SpawnPet();
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

	bool isScoreScene;

public:
	void Init();
	void Release();

	void CreateUI();
	void ReleaseUI();

public:
	void CreatePlayer();
	void SpawnEnemyStageTwo();
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
	void AddMoney(int money);
	void RemoveMoney(int money);
	void RankInit();
	void SortRanking();
	
	void CheatKey();

	void AddCard(int card);

	void Reset();
	void BossReset();
	void GunReset();

	void ChocieScene();

	void RemoveCharge();

	void SpawnCoin(Vec2 Pos);


};

