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

#include <algorithm>

GameMgr::GameMgr()
{
}

GameMgr::~GameMgr()
{
}

void GameMgr::Init()
{
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

	BossMaxHp = 0, BossHp = 0;
	MiniBossMaxHp = 0, MiniBossHp = 0;
	ChargeCount = 0;
	Player_Coefficient = 1.f;
	isSpawnEnemy = false;
	HV_TYPE = 1;
	TempScore = m_Score;

	KillScore = 330.f;
	ItemScore = 213.f;
	BounsScore = 770.f;

	ChargeTime = 0.f;
	isChoiceDelay = true;
	PetCount = 0;

	//밑에 카드 
	UP_Charge = false;
	
	// 보스 잡고 점수화면 
	isScoreScene = false;
}

void GameMgr::Release()
{
}

void GameMgr::CreateUI()
{
	if (!m_isCreateUI)
	{
		UI::GetInst()->Init();
		m_isCreateUI = true;
		std::cout << "UI 생성" << std::endl;
	}
}

void GameMgr::ReleaseUI()
{
	UI::GetInst()->Release();
	UI::GetInst()->ReleaseInst();
	m_isCreateUI = false;
	std::cout << "UI 삭제" << std::endl;
}

void GameMgr::CreatePlayer()
{
	if (GameInfo->m_Scene == StageScene::STAGE1)
		ObjMgr->AddObject(new Player(), "Player");
	else
		ObjMgr->AddObject(new Player(Hp), "Player");

	m_isCreatePlayer = true;
}

void GameMgr::PlayerDeath()
{
	m_isCreatePlayer = false;
	ObjMgr->DeleteObject("Player");
	ObjMgr->DeleteObject("pBullet");
}


void GameMgr::Update()
{

	if (m_isCreateUI)
		UI::GetInst()->Update();

	AddScore(MaxScore);
	ChargeTime += dt;
	if (!isChoiceDelay) {
		SpawnDelay += dt;
		if (SpawnDelay > 10) {
			isSpawnEnemy = false;
			isChoiceDelay = true;
			isBossSpawn = false;
			isMiniBossSpawn = false;
			SpawnDelay = 40.f;
			EnemyCount = 0;
		}
	}
}

void GameMgr::Render()
{
	if (m_isCreateUI)
		UI::GetInst()->Render();
}


void GameMgr::AddScore(int maxscore)
{
	if (int(m_Score) < maxscore) {
		if (int(m_Score) + 4000 < maxscore)
			m_Score += 90;

		else
			m_Score += 10;
	}
	else if (int(m_Score) > maxscore) {
		m_Score = maxscore;
	}
}

void GameMgr::RankInit()
{
	RankingPlayer* dummy1 = new RankingPlayer();
	dummy1->name = "dummy1";
	dummy1->score = rand() % 10000;;

	RankingPlayer* dummy2 = new RankingPlayer();
	dummy2->name = "dummy2";
	dummy2->score = rand() % 10000;;

	RankingPlayer* dummy3 = new RankingPlayer();
	dummy3->name = "dummy3";
	dummy3->score = rand() % 10000;

	m_Rank = new RankingPlayer();
	m_Rank->name = "Player(Temp)";
	m_Rank->score = 0;

	Ranks.push_back(dummy1);
	Ranks.push_back(dummy2);
	Ranks.push_back(dummy3);
	Ranks.push_back(m_Rank);
	m_Score = 0;
}

bool Sort(const RankingPlayer* pSour, const RankingPlayer* pDest)
{
	return (pSour->score > pDest->score);
}

void GameMgr::SortRanking()
{
	std::sort(Ranks.begin(), Ranks.end(), Sort);
}

void GameMgr::CheatKey()
{
	if (INPUT->GetKey('D') == KeyState::DOWN) {
		Camera::GetInst()->isVibration = true;
		Camera::GetInst()->ShakeTimeY = 0;

	}

	//Camera::GetInst()->isVibration = true;
	//Camera::GetInst()->ShakeTimeY = 0;

	//Camera::GetInst()->isVibration = true;
	//Camera::GetInst()->ShakeTimeX = 0; 

	//ObjMgr->AddObject(new Item(Vec2(Camera::GetInst()->m_Position.x + 1500, 350)), "Heal");

	//	MaxScore += 3000;
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

	if (INPUT->GetKey(VK_F2) == KeyState::DOWN) {
		SceneDirector::GetInst()->ChangeScene(new MainScene());
	}
	if (INPUT->GetKey(VK_F3) == KeyState::DOWN && m_Scene != StageScene::STAGE1) {
		SceneDirector::GetInst()->ChangeScene(new Stage1());
	}
	if (INPUT->GetKey(VK_F4) == KeyState::DOWN && m_Scene != StageScene::STAGE2) {
		SceneDirector::GetInst()->ChangeScene(new Stage2());
	}
	if (INPUT->GetKey(VK_F5) == KeyState::DOWN && m_Scene != StageScene::STORE) {
		SceneDirector::GetInst()->ChangeScene(new StoreScene());
	}
	if (INPUT->GetKey('I') == KeyState::DOWN) {
		ObjMgr->DeleteObject("Enemy");
		EnemyCount = 0;
		isBossSpawn = false;
		isMiniBossSpawn = false;
	}
	if (INPUT->GetKey('O') == KeyState::DOWN) {
		//ObjMgr->DeleteObject("Enemy");
		//vradEnemyCount = 0;
		isBossSpawn = false;
		isMiniBossSpawn = true;
		CK_MiniBossSpawn = true;
		CK_BossSpawn = false;
	}
	if (INPUT->GetKey('P') == KeyState::DOWN) {
		//	ObjMgr->DeleteObject("Enemy");
		//	EnemyCount = 0;
		isBossSpawn = true;
		isMiniBossSpawn = false;
		CK_BossSpawn = true;
		CK_MiniBossSpawn = false;
	}

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

	if (INPUT->GetKey(VK_ESCAPE) == KeyState::DOWN) {
		if (!isPause) {
			isPause = true;
			std::cout << "일시정지 ON" << std::endl;
		}
		else {
			isPause = false;
			std::cout << "일시정지 OFF" << std::endl;
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
	isMiniBossSpawn = false;
	isBossSpawn = false;
}

void GameMgr::GunReset()
{
	for (int i = 0; i < 6; i++)
		HV_ShotType[i] = 1;
}

void GameMgr::ChocieScene()
{
	SpawnDelay = 0.f;

	isChoiceDelay = false;
	ObjMgr->DeleteObject("Enemy");
}

void GameMgr::RemoveCharge()
{
	ChargeCount--;
}

void GameMgr::SpawnEnemy()
{
	if (isSpawnEnemy) {
		SpawnDelay += dt;
		//ObjMgr->AddObject(new Boss(), "Boss");
		//ObjMgr->AddObject(new MiniSBoss(Vec2(1920/2+500,1080/2)), "Boss");
		if (m_Scene == StageScene::STAGE2) {
			if (EnemyCount <= 0 && m_Score >= 1500 + TempScore && !isMiniBossSpawn && isOneBoss && isOneMiniBoss || CK_MiniBossSpawn) {
				ObjMgr->AddObject(new MiniBoss(Vec2(Camera::GetInst()->m_Position.x + 1920 + 500, rand() % 450 + 100)), "Enemy");
				isOneMiniBoss = false;
				CK_MiniBossSpawn = false;
			}
		}
		else if (m_Scene == StageScene::STAGE1) {
			isOneMiniBoss = false;
			CK_MiniBossSpawn = false;
		}
		if (EnemyCount <= 0 && m_Score >= 4500 + TempScore && !isBossSpawn && isOneBoss && !isOneMiniBoss || CK_BossSpawn) {
			ObjMgr->AddObject(new Boss(Vec2(Camera::GetInst()->m_Position.x + 1920 + 500, 310)), "Enemy");
			isOneBoss = false;
			CK_BossSpawn = false;
		}
		if ((EnemyCount <= 0 && EnemyCount <= 7 || SpawnDelay > 30.f) && !isMiniBossSpawn && !isBossSpawn && isOneBoss) {
			if (isOneMiniBoss) {
				ObjMgr->AddObject(new Enemy1(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 430 + 100)), "Enemy");
				ObjMgr->AddObject(new Enemy2(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 430 + 100)), "Enemy");
				if (rand() % 2 == 0)
					ObjMgr->AddObject(new EliteEnemy1(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 430 + 100)), "Enemy");
				else
					ObjMgr->AddObject(new EliteEnemy2(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 430 + 100)), "Enemy");


				SpawnDelay = 5.f;
				if (m_Scene == StageScene::STAGE2) {
					if (rand() % 2 == 0)
						ObjMgr->AddObject(new Enemy3(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 430 + 100)), "Enemy");
					SpawnDelay = 15.f;
				}
			}
			else {
				ObjMgr->AddObject(new Enemy1(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 430 + 100)), "Enemy");
				ObjMgr->AddObject(new Enemy2(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 430 + 100)), "Enemy");
				ObjMgr->AddObject(new EliteEnemy1(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 430 + 100)), "Enemy");
				ObjMgr->AddObject(new EliteEnemy2(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 430 + 100)), "Enemy");
				SpawnDelay = 0.f;
				if (m_Scene == StageScene::STAGE2) {
					ObjMgr->AddObject(new Enemy3(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 430 + 100)), "Enemy");
					SpawnDelay = 15.f;
				}
			}
		}
	}
}
