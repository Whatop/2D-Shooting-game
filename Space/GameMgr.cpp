#include "stdafx.h"
#include "GameMgr.h"
#include "UI.h"
#include "Boss.h"
#include "MiniBoss.h"
#include "Enemy1.h"
#include "Enemy2.h"
#include "Enemy3.h"
#include "EliteEnemy1.h"
#include "EliteEnemy2.h"
#include "Item.h"

#include "MainScene.h"
#include "Stage1.h"

#include "Stage2.h"
#include "StoreScene.h"
#include "AudioLibrary.h"

#include "Coin.h"
#include "Pet.h"

#include <algorithm>
#include <fstream>

GameMgr::GameMgr()
{
	// 시작 플레이어 HP 기본값
	Hp = 100;

	// 사운드 시스템 초기화
	SoundMgr::GetInst()->Init();

	// 대사/효과음 라이브러리 초기화
	AudioLibrary::Init();

}

GameMgr::~GameMgr()
{
	Release();
}

void GameMgr::SpawnPet()
{
	// 1) 기존 펫 제거(중복 소환 방지)
	ObjMgr->DeleteObject("Pet");

	// 2) 재소환 전 카운트 리셋
	PetCount = 0;

	// 3) 보유중인 타입들 그대로 전부 재소환
	for (int type : OwnedPetTypes) {
		ObjMgr->AddObject(new Pet(type), "Pet");
	}
}
void GameMgr::RankInit()
{
	for (auto rank : Ranks)
	{
		delete rank;
	}
	Ranks.clear();

	m_Rank = nullptr;

	LoadRanking();

	if (Ranks.empty())
	{
		RankingPlayer* dummy1 = new RankingPlayer();
		dummy1->name = "dummy1";
		dummy1->score = 99999;

		RankingPlayer* dummy2 = new RankingPlayer();
		dummy2->name = "dummy2";
		dummy2->score = 1000;

		RankingPlayer* dummy3 = new RankingPlayer();
		dummy3->name = "dummy3";
		dummy3->score = 500;

		Ranks.push_back(dummy1);
		Ranks.push_back(dummy2);
		Ranks.push_back(dummy3);
	}

	SortRanking();
	m_Score = 0;
}
void GameMgr::SaveRanking()
{
	std::ofstream fout("Ranking.txt");

	if (!fout.is_open())
		return;

	SortRanking();

	for (auto rank : Ranks)
	{
		if (rank != nullptr)
		{
			fout << rank->name << " " << rank->score << "\n";
		}
	}

	fout.close();
}
void GameMgr::LoadRanking()
{
	std::ifstream fin("Ranking.txt");

	// 파일이 없으면 그냥 종료
	if (!fin.is_open())
		return;

	std::string name;
	int score = 0;

	while (fin >> name >> score)
	{
		RankingPlayer* player = new RankingPlayer();
		player->name = name;
		player->score = score;

		Ranks.push_back(player);
	}

	fin.close();
}
void GameMgr::AddCurrentPlayerRanking(const std::string& name)
{
	RankingPlayer* player = new RankingPlayer();
	player->name = name;
	player->score = static_cast<int>(m_Score);

	Ranks.push_back(player);
	m_Rank = player; // 이번 플레이 결과를 가리키게 함

	SortRanking();
	SaveRanking();
}
void GameMgr::Init()
{
	RankInit();
	// UI / 디버그 / 플레이어 생성 상태 초기화
	m_isCreateUI = false;
	m_DebugMode = false;
	m_isCreatePlayer = false;

	AutoCamera = true;
	CameraStop = false;
	isPause = false;

	EnemyCount = 0;
	SpawnDelay = 0.f;
	AddDelay = 0.f;

	isOneBoss = true;
	isOneMiniBoss = true;
	CK_MiniBossSpawn = false;
	CK_BossSpawn = false;

	BossMaxHp = 0;
	BossHp = 0;
	MiniBossMaxHp = 0;
	MiniBossHp = 0;

	ChargeCount = 0;
	Player_Coefficient = 1.f;
	isSpawnEnemy = false;
	HV_TYPE = 1;

	TempScore = m_Score;

	KillScore = 0.f;
	ItemScore = 0.f;
	BonusScore = 0.f;

	ChargeTime = 0.f;
	isChoiceDelay = true;
	PetCount = 0;

	UP_Charge = false;
	isScoreScene = false;
	HV_Boom = 3;
	MoneyPokeyPos = Vec2(0, 0);
	played = false;
}

void GameMgr::Release()
{
	// 랭킹 저장
	SaveRanking();

	// RankInit / LoadRanking 에서 new로 만든 랭킹 데이터 전부 해제
	for (auto rank : Ranks)
	{
		delete rank;
	}

	Ranks.clear();
	m_Rank = nullptr;
}
void GameMgr::CreateUI()
{
	// 아직 UI가 없을 때만 생성
	if (!m_isCreateUI)
	{
		UI::GetInst()->Init();
		m_isCreateUI = true;
		std::cout << "UI 생성" << std::endl;
	}
}

void GameMgr::ReleaseUI()
{
	// UI 정리 및 싱글톤 해제
	UI::GetInst()->Release();
	UI::ReleaseInst();
	m_isCreateUI = false;
	std::cout << "UI 삭제" << std::endl;
}

void GameMgr::CreatePlayer()
{
	// 현재 HP를 넘겨서 플레이어 생성
	ObjMgr->AddObject(new Player(Hp), "Player");

	m_isCreatePlayer = true;
}

void GameMgr::PlayerDeath()
{
	// 플레이어 사망 시 생성 상태 해제 후 관련 오브젝트 삭제
	m_isCreatePlayer = false;
	ObjMgr->DeleteObject("Player");
	ObjMgr->DeleteObject("pBullet");
}

void GameMgr::Update()
{
	// UI가 있으면 UI 먼저 업데이트
	if (m_isCreateUI)
		UI::GetInst()->Update();

	// 점수와 돈은 목표치(MaxScore, MaxMoney)까지 서서히 증가
	AddScore(MaxScore);
	AddMoney(MaxMoney);

	// 차지 시간 누적
	ChargeTime += dt;

	// 선택 씬/연출 대기 중이 아닐 때 타이머 진행
	if (!isChoiceDelay) {
		SpawnDelay += dt;

		// 일정 시간 지나면 다시 전투 상태를 정리
		if (SpawnDelay > 10) {
			isSpawnEnemy = false;
			isChoiceDelay = true;
			isBossSpawn = false;
			isMiniBossSpawn = false;
			SpawnDelay = 40.f;
			EnemyCount = 0;
		}
	}

	// F1: 사운드 테스트용 흔적 코드
	if (INPUT->GetKey(VK_F1) == KeyState::DOWN) {
		//SoundMgr::StopAll();
		//SoundMgr::SetMasterVolume(1.0f);
		//
		//static SoundMgr* test = nullptr;
		//if (!test) test = new SoundMgr("Sound/SND_TXT2.wav", false); // loop=false
		//// per-sound 볼륨 최대로
		//// test->volumeSetting(1.0f);
		//// 또는 volumeUp 반복 호출
		//for (int i = 0; i < 20; ++i) test->volumeUp();
		//
		//test->stop();  // 중첩 방지
		//test->play();  // 재생
		//ObjMgr->AddObject(new Item(INPUT->GetMousePos()), "Heal");
		//UI::GetInst()->PushMessage(L"타자음 테스트 문장!", false);
	}

	// ESC: 일시정지 토글
	if (INPUT->GetKey(VK_ESCAPE) == KeyState::DOWN) {
		if (!isPause) {
			isPause = true;
			std::cout << "일시정지 ON" << std::endl;
			SoundMgr::GetInst()->PauseAll();
		}
		else {
			isPause = false;
			std::cout << "일시정지 OFF" << std::endl;
			SoundMgr::GetInst()->ResumeAll();
		}

		// 현재는 ESC 누를 때 Heal 아이템도 하나 생성됨
		ObjMgr->AddObject(new Item(INPUT->GetMousePos()), "Heal");
	}

	// 볼륨 업: Numpad '+' 또는 일반 '+' 키
	if (INPUT->GetKey(VK_ADD) == KeyState::DOWN ||
		INPUT->GetKey(VK_OEM_PLUS) == KeyState::DOWN)
	{
		SoundMgr::MasterVolumeUp(0.1f);
	}
	// 볼륨 다운: Numpad '-' 또는 일반 '-' 키
	else if (INPUT->GetKey(VK_SUBTRACT) == KeyState::DOWN ||
		INPUT->GetKey(VK_OEM_MINUS) == KeyState::DOWN)
	{
		SoundMgr::MasterVolumeDown(0.1f);
	}

	// 최대 점수가 매우 높아졌을 때 감사 음성/메시지 1회 출력
	if (MaxScore > 99999 && !played) {
		SoundMgr::GetInst()->StopAll();

		SoundMgr* sfx = new SoundMgr("Sound/Thank_you.wav", false);
		sfx->play();
		sfx->volumeSetting(1);

		UI::GetInst()->PushMessage(L"축하해요!!! 점수를 많이 모으셨군요,\n 열심히 다음부분을 개발해보겠습니다! ", true);

		played = true; // 중복 재생 방지
	}

	// 치트키/디버그키 처리
	CheatKey();
}

void GameMgr::Render()
{
	// UI가 있을 때만 렌더
	if (m_isCreateUI)
		UI::GetInst()->Render();
}

void GameMgr::AddScore(int maxscore)
{
	// 현재 점수가 목표 점수보다 낮으면 조금씩 따라가게 증가
	if (int(m_Score) < maxscore) {
		if (int(m_Score) + 4000 < maxscore)
			m_Score += 90; // 차이가 크면 빨리 증가
		else
			m_Score += 10; // 차이가 적으면 천천히 증가
	}
	// 목표보다 커졌으면 강제로 맞춤
	else if (int(m_Score) > maxscore) {
		m_Score = maxscore;
	}
}

void GameMgr::AddMoney(int money)
{
	// 현재 돈이 목표 돈보다 낮으면 1씩 증가
	if (int(m_Money) < money) {
		m_Money += 1;
	}
	// 목표보다 커졌으면 강제로 맞춤
	else if (int(m_Money) > money) {
		m_Money = money;
	}
}

void GameMgr::RemoveMoney(int money)
{
	// 목표 돈(MaxMoney)에서 차감
	MaxMoney -= money;
}


// 점수 높은 순으로 정렬하기 위한 비교 함수
bool Sort(const RankingPlayer* pSour, const RankingPlayer* pDest)
{
	return (pSour->score > pDest->score);
}

void GameMgr::SortRanking()
{
	// 랭킹 내림차순 정렬
	std::sort(Ranks.begin(), Ranks.end(), Sort);
}

void GameMgr::CheatKey()
{
	// D: 화면 흔들림 테스트
	if (INPUT->GetKey('D') == KeyState::DOWN) {
		Camera::GetInst()->isVibration = true;
		Camera::GetInst()->ShakeTimeY = 0;
	}

	//Camera::GetInst()->isVibration = true;
	//Camera::GetInst()->ShakeTimeY = 0;
	//Camera::GetInst()->isVibration = true;
	//Camera::GetInst()->ShakeTimeX = 0; 
	//ObjMgr->AddObject(new Item(Vec2(Camera::GetInst()->m_Position.x + 1500, 350)), "Heal");
	//MaxScore += 3000;

	// V: 디버그 모드 토글
	if (INPUT->GetKey('V') == KeyState::DOWN)
	{
		if (m_DebugMode)
		{
			m_DebugMode = false;
			std::cout << "디버깅 모드 비 활성화" << std::endl;
		}
		else if (!m_DebugMode)
		{
			m_DebugMode = true;
			std::cout << "디버깅 모드 활성화" << std::endl;
		}
	}

	// F2: 메인씬 이동
	if (INPUT->GetKey(VK_F2) == KeyState::DOWN) {
		SceneDirector::GetInst()->ChangeScene(new MainScene());
	}

	// F3: 스테이지1 이동
	if (INPUT->GetKey(VK_F3) == KeyState::DOWN && m_Scene != StageScene::STAGE1) {
		SceneDirector::GetInst()->ChangeScene(new Stage1());
	}

	// F4: 스테이지2 이동
	if (INPUT->GetKey(VK_F4) == KeyState::DOWN && m_Scene != StageScene::STAGE2) {
		SceneDirector::GetInst()->ChangeScene(new Stage2());
	}

	// F5: 상점씬 이동
	if (INPUT->GetKey(VK_F5) == KeyState::DOWN && m_Scene != StageScene::STORE) {
		SceneDirector::GetInst()->ChangeScene(new StoreScene());
	}

	// I: 적 전부 제거 + 보스 상태 초기화
	if (INPUT->GetKey('I') == KeyState::DOWN) {
		ObjMgr->DeleteObject("Enemy");
		EnemyCount = 0;
		isBossSpawn = false;
		isMiniBossSpawn = false;
	}

	// O: 미니보스 강제 소환 플래그
	if (INPUT->GetKey('O') == KeyState::DOWN) {
		isBossSpawn = false;
		isMiniBossSpawn = true;
		CK_MiniBossSpawn = true;
		CK_BossSpawn = false;
	}

	// P: 보스 강제 소환 플래그
	if (INPUT->GetKey('P') == KeyState::DOWN) {
		isBossSpawn = true;
		isMiniBossSpawn = false;
		CK_BossSpawn = true;
		CK_MiniBossSpawn = false;
	}

	// F1: 카메라 정지/해제 토글
	if (INPUT->GetKey(VK_F1) == KeyState::DOWN) {
		if (!CameraStop) {
			CameraStop = true;
			std::cout << "AUTO 카메라 OFF" << std::endl;
		}
		else {
			CameraStop = false;
			std::cout << "AUTO 카메라 ON" << std::endl;
		}
	}
}

void GameMgr::AddCard(int card)
{
	// 단발 = 1, 샷건 = 2, 차지 = 3, 유도 = 4, 부메랑 = 5, 더블 = 6;
	HV_ShotType[card] += 1;
}

void GameMgr::Reset()
{
}

void GameMgr::BossReset()
{
	// 보스류 등장 상태 초기화
	isMiniBossSpawn = false;
	isBossSpawn = false;
}

void GameMgr::GunReset()
{
	// 무기 타입 수치 전부 1로 초기화
	for (int i = 0; i < 6; i++)
		HV_ShotType[i] = 1;
}

void GameMgr::ChoiceScene()
{
	// 선택지/연출 씬 진입 전 적 스폰 타이머 초기화
	SpawnDelay = 0.f;

	// 선택 지연 상태 진입
	isChoiceDelay = false;

	// 현재 적 제거
	ObjMgr->DeleteObject("Enemy");
}

void GameMgr::RemoveCharge()
{
	// 차지 수 감소
	if (ChargeCount > 0)
	ChargeCount--;
}

void GameMgr::SpawnCoin(Vec2 Pos)
{
	// 해당 위치에 코인 생성
	ObjMgr->AddObject(new Coin(Pos), "Coin");
}

void GameMgr::SetHp(float i) {
	// HP 직접 설정
	Hp = i;
}

void GameMgr::SpawnEnemy()
{
	// 적 생성이 허용된 상태에서만 진행
	if (isSpawnEnemy) {
		SpawnDelay += dt;

		// 스테이지2에서는 조건 만족 시 미니보스 생성
		if (m_Scene == StageScene::STAGE2) {
			if ((EnemyCount <= 0 && m_Score >= 1500 + TempScore && !isMiniBossSpawn && isOneBoss && isOneMiniBoss) || CK_MiniBossSpawn){
				ObjMgr->AddObject(new MiniBoss(Vec2(Camera::GetInst()->m_Position.x + 1920 + 500, rand() % 330 + 50)), "Enemy");
				isOneMiniBoss = false;
				CK_MiniBossSpawn = false;
			}
		}
		// 스테이지1에서는 미니보스 관련 플래그 조정
		else if (m_Scene == StageScene::STAGE1) {
			isOneMiniBoss = false;
			CK_MiniBossSpawn = false;
		}

		// 보스 생성 조건
		if ((EnemyCount <= 0 && m_Score >= 4500 + TempScore && !isBossSpawn && isOneBoss && !isOneMiniBoss) || CK_BossSpawn)
		{
			ObjMgr->AddObject(new Boss(Vec2(Camera::GetInst()->m_Position.x + 1920 + 500, 433.f / 2.f)), "Enemy");
			isOneBoss = false;
			CK_BossSpawn = false;
		}

		// 일반 적 웨이브 생성 조건
		if ((EnemyCount <= 0 || SpawnDelay > 30.f) && !isMiniBossSpawn && !isBossSpawn && isOneBoss) {
			if (isOneMiniBoss) {
				// 미니보스 전 단계 웨이브
				ObjMgr->AddObject(new Enemy1(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 330 + 50)), "Enemy");
				ObjMgr->AddObject(new Enemy2(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 330 + 50)), "Enemy");

				if (rand() % 2 == 0)
					ObjMgr->AddObject(new EliteEnemy1(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 330 + 50)), "Enemy");
				else
					ObjMgr->AddObject(new EliteEnemy2(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 330 + 50)), "Enemy");

				SpawnDelay = 5.f;

				if (m_Scene == StageScene::STAGE2) {
					if (rand() % 2 == 0)
						ObjMgr->AddObject(new Enemy3(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 330 + 50)), "Enemy");
					SpawnDelay = 15.f;
				}
			}
			else {
				// 일반 후반 웨이브
				ObjMgr->AddObject(new Enemy1(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 330 + 50)), "Enemy");
				ObjMgr->AddObject(new Enemy2(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 330 + 50)), "Enemy");
				ObjMgr->AddObject(new EliteEnemy1(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 230 + 80)), "Enemy");
				ObjMgr->AddObject(new EliteEnemy2(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 230 + 80)), "Enemy");

				SpawnDelay = 0.f;

				if (m_Scene == StageScene::STAGE2) {
					ObjMgr->AddObject(new Enemy3(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 330 + 50)), "Enemy");
					SpawnDelay = 15.f;
				}
			}
		}
	}
}

void GameMgr::SpawnEnemyStageTwo() {
	// 스테이지2 전용 적 웨이브 한 번에 생성
	ObjMgr->AddObject(new Enemy1(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 330 + 50)), "Enemy");
	ObjMgr->AddObject(new Enemy2(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 330 + 50)), "Enemy");
	ObjMgr->AddObject(new EliteEnemy1(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 230 + 80)), "Enemy");
	ObjMgr->AddObject(new EliteEnemy2(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 230 + 80)), "Enemy");
	ObjMgr->AddObject(new Enemy3(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 330 + 50)), "Enemy");
	ObjMgr->AddObject(new Enemy3(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 330 + 50)), "Enemy");
	ObjMgr->AddObject(new Enemy3(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 330 + 50)), "Enemy");
}