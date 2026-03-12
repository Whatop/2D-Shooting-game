#pragma once

// 전역처럼 GameMgr 싱글톤에 접근하기 위한 매크로
#define GameInfo GameMgr::GetInst()

// 현재 플레이어 오브젝트를 바로 얻기 위한 매크로
#define GetPlayer GameMgr::GetInst()->GetPlayerInfo()

// 상점에서 "즉시 적용"하지 않고, 임시로 누적해둘 정보
struct PendingShop {
	int goldCost = 0;          // 총 차감 예정 골드
	int cardUp[6] = { 0 };     // 무기 타입별 카드 업그레이드 개수
	int petAdd[6] = { 0 };     // 무기 타입별 펫 추가 개수

	// 누적된 상점 정보 초기화
	void Clear() {
		goldCost = 0;
		for (int i = 0; i < 6; ++i) { cardUp[i] = 0; petAdd[i] = 0; }
	}
};

// 현재 어떤 씬에 있는지 구분하는 enum
enum class StageScene {
	STAGE1,
	STAGE2,
	STORE,
	NONE
};

// 랭킹용 플레이어 정보 구조체
struct RankingPlayer
{
public:
	int score;         // 점수
	std::string name;  // 이름
};

// 게임 전체 진행 상태를 관리하는 싱글톤 매니저
class GameMgr : public Singleton<GameMgr>
{
	// 상점에서 누적시켜둘 요청 버킷
	PendingShop pendingShop;

	// 현재 플레이어 오브젝트 포인터
	Object* PlayerInfo;

	// 플레이어 / 보스 / 미니보스 체력 정보
	float MaxHp, Hp;
	float BossMaxHp, BossHp;
	float MiniBossMaxHp, MiniBossHp;

public:
	GameMgr();
	~GameMgr();

	// 플레이어 HP 강제 설정
	void SetHp(float hp);
	// 현재 플레이 결과를 랭킹에 추가
	void AddCurrentPlayerRanking(const std::string& name);
	// 돈 포키(혹은 돈 관련 오브젝트) 위치 저장용
	Vec2 MoneyPokeyPos;

	// 랭킹 데이터 저장
	std::vector<RankingPlayer*> Ranks;

	// 플레이어/UI 생성 여부
	bool m_isCreatePlayer;
	bool m_isCreateUI;

	// 디버그 모드 여부
	bool m_DebugMode;

	// 카메라 자동 이동 관련
	bool AutoCamera;
	bool CameraStop;

	// 보스 / 미니보스 현재 등장 여부
	bool isBossSpawn;
	bool isMiniBossSpawn;

	// "이번 사이클에 한 번만" 관련 플래그
	bool isOneBoss;
	bool isOneMiniBoss;

	// 일시정지 여부
	bool isPause;

	// 특정 1회성 사운드 재생 여부
	bool played;

	// 목표 점수 / 목표 돈
	int MaxScore;
	int MaxMoney;

	// 현재 적 수
	int EnemyCount;

	// 현재 점수 및 계산용 점수
	float m_Score;
	float TempScore;
	float KillScore, ItemScore, BonusScore;

	// 현재 돈
	float m_Money;

	// 각종 계수
	float Player_Coefficient; // 플레이어 공격력 계수
	float Pet_Coefficient;    // 펫 공격력 계수
	float Enemy_Coefficient;  // 적 공격력 계수

	// 적 스폰 타이머 / 기타 추가 타이머
	float SpawnDelay;
	float AddDelay;

	// 차지 공격 관련 카운트
	int ChargeCount;

	// 디버그/강제 보스 스폰용 체크 플래그
	bool CK_BossSpawn;
	bool CK_MiniBossSpawn;

	// 노히트 판정 체크
	bool isNoHit;

	// 무기 타입별 레벨/강화 수치 배열
	int HV_ShotType[6];

	// 현재 선택된 무기 타입
	int HV_TYPE;

	// 폭탄 관련 보유 수치
	int HV_Boom;

	// 현재 스테이지 번호
	int Stage = 1;

	// 타입 선택 시 한 번만 바꾸기 위한 플래그
	bool isGunType;

	// 현재 보유한 펫 공격 타입들을 순서대로 저장
	std::vector<int> OwnedPetTypes;

	// 편의 함수: 보유 펫 타입 추가
	void AddOwnedPetType(int type) { OwnedPetTypes.push_back(type); }

	// 편의 함수: 보유 펫 타입 목록 초기화 + 펫 카운트 초기화
	void ClearOwnedPetTypes() { OwnedPetTypes.clear(); PetCount = 0; }

	// 현재 보유 펫 타입 기준으로 펫 재소환
	void SpawnPet();

	// 아이템 보유/적용 상태
	bool IT_Shot;
	bool IT_Shotgun;
	bool IT_Charge;
	bool IT_Induce;
	bool IT_Boomerang;
	bool IT_Doubleshot;

	// 적 생성 가능 여부
	bool isSpawnEnemy;

	// 현재 플레이어 랭킹용 정보
	RankingPlayer* m_Rank;

	// 현재 씬 상태
	StageScene m_Scene;

	// 보스 위치 저장
	Vec2 BossPosition;

	// 차지 시간 누적
	float ChargeTime;

	// 선택지/연출 딜레이 여부
	bool isChoiceDelay;

	// 현재 펫 개수
	int PetCount;

	// 하단 카드 쪽 차지 강화 여부
	bool UP_Charge;

	// 점수 화면 진입 여부
	bool isScoreScene;

public:
	// 게임매니저 기본 상태 초기화
	void Init();

	// 해제용 함수
	void Release();

	// UI 생성
	void CreateUI();

	// UI 해제
	void ReleaseUI();
	// 랭킹 파일 저장 / 불러오기
	void SaveRanking();
	void LoadRanking();
public:
	// 플레이어 생성
	void CreatePlayer();

	// 스테이지2 적 웨이브 전용 생성
	void SpawnEnemyStageTwo();

	// 플레이어 사망 처리
	void PlayerDeath();

	// 현재 플레이어 오브젝트 반환
	Object* GetPlayerInfo() { return PlayerInfo; }

	// 플레이어 오브젝트 갱신
	void PlayerUpdate(Object* obj) { PlayerInfo = obj; }

	// 플레이어 체력 정보 갱신
	void PlayerHpUpdate(float maxhp, float hp) { MaxHp = maxhp, Hp = hp; }

	// 플레이어 최대 체력 반환
	float GetPlayerMaxHp() { return MaxHp; }

	// 플레이어 현재 체력 반환
	float GetPlayerHp() { return Hp; }

public:
	// 보스 체력 정보 갱신
	void BossHpUpdate(float maxhp, float hp) { BossMaxHp = maxhp, BossHp = hp; }

	// 미니보스 체력 정보 갱신
	void MiniBossHpUpdate(float maxhp, float hp) { MiniBossMaxHp = maxhp, MiniBossHp = hp; }

	// 보스 최대 체력 반환
	float GetBossMaxHp() { return BossMaxHp; }

	// 보스 현재 체력 반환
	float GetBossHp() { return BossHp; }

	// 미니보스 최대 체력 반환
	float GetMiniBossMaxHp() { return MiniBossMaxHp; }

	// 미니보스 현재 체력 반환
	float GetMiniBossHp() { return MiniBossHp; }

	// 적 스폰 로직
	void SpawnEnemy();

public:
	// 매 프레임 갱신
	void Update();

	// 렌더
	void Render();

	// 점수 증가 처리
	void AddScore(int score);

	// 돈 증가 처리
	void AddMoney(int money);

	// 돈 감소 처리
	void RemoveMoney(int money);

	// 랭킹 데이터 초기화
	void RankInit();

	// 랭킹 정렬
	void SortRanking();

	// 치트키 / 디버그 입력 처리
	void CheatKey();

	// 카드 강화 추가
	void AddCard(int card);

	// 전체 상태 리셋
	void Reset();

	// 보스 상태 리셋
	void BossReset();

	// 총기 타입 관련 수치 리셋
	void GunReset();

	// 선택 씬 진입 처리
	void ChoiceScene();

	// 차지 수 감소
	void RemoveCharge();

	// 코인 생성
	void SpawnCoin(Vec2 Pos);
};