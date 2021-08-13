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

	MaxHp = 0, Hp = 0;
	BossMaxHp = 0, BossHp = 0;
	MiniBossMaxHp = 0, MiniBossHp = 0;
	ChargeCount = 0;
	Player_Coefficient = 1.f;
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
	}
}

void GameMgr::ReleaseUI()
{
	UI::GetInst()->Release();
	UI::GetInst()->ReleaseInst();
	m_isCreateUI = false;
}

void GameMgr::CreatePlayer()
{
	ObjMgr->AddObject(new Player(), "Player");
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
	if (INPUT->GetKey(VK_F9) == KeyState::DOWN) {
		if (!isPause) {
			isPause = true;
			std::cout << "일시정지 ON" << std::endl;
		}
		else {
			isPause = false;
			std::cout << "일시정지 OFF" << std::endl;
		}
	}


	if (m_isCreateUI)
		UI::GetInst()->Update();
	AddScore(MaxScore);
}

void GameMgr::Render()
{
	if (m_isCreateUI)
		UI::GetInst()->Render();
}


void GameMgr::AddScore(int maxscore)
{
	if (int(m_Score) < maxscore) {
		if (int(m_Score)+4000 < maxscore)
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

void GameMgr::SpawnItem(Vec2 Pos)
{

}

void GameMgr::SpawnEnemy()
{
	SpawnDelay += dt;
	//ObjMgr->AddObject(new Boss(), "Boss");
	//ObjMgr->AddObject(new MiniBoss(Vec2(1920/2+500,1080/2)), "Boss");
	if (EnemyCount <= 0 && GameInfo->m_Score >= 3000 && !isMiniBossSpawn && isOneBoss && isOneMiniBoss || CK_MiniBossSpawn) {
		ObjMgr->AddObject(new MiniBoss(Vec2(Camera::GetInst()->m_Position.x + 1920 + 500, rand() % 450 + 100)), "MiniBoss");
		isOneMiniBoss = false;
		CK_MiniBossSpawn = false;
	}
	if (EnemyCount <= 0 && GameInfo->m_Score >= 7000 && !isBossSpawn && isOneBoss && !isOneMiniBoss || CK_BossSpawn) {
		ObjMgr->AddObject(new Boss(Vec2(Camera::GetInst()->m_Position.x + 1920 + 500, 310)), "Boss");
		isOneBoss = false;
		CK_BossSpawn = false;
	}
	if ((EnemyCount <= 0 && EnemyCount <= 7 || SpawnDelay > 30.f) && !isMiniBossSpawn && !isBossSpawn && isOneBoss) {
		if (isOneMiniBoss) {
			ObjMgr->AddObject(new Enemy1(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 430 + 100)), "Enemy1");
			ObjMgr->AddObject(new Enemy2(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 430 + 100)), "Enemy2");
			if (rand() % 2 == 0)
				ObjMgr->AddObject(new Enemy3(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 430 + 100)), "Enemy2");
			if (rand() % 2 == 0)
				ObjMgr->AddObject(new EliteEnemy1(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 430 + 100)), "EliteEnemy1");
			else
				ObjMgr->AddObject(new EliteEnemy2(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 430 + 100)), "EliteEnemy2");

			SpawnDelay = 5.f;
		}
		else {
			ObjMgr->AddObject(new Enemy1(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 430 + 100)), "Enemy1");
			ObjMgr->AddObject(new Enemy2(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 430 + 100)), "Enemy2");
			ObjMgr->AddObject(new Enemy3(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 430 + 100)), "Enemy2");
			ObjMgr->AddObject(new EliteEnemy1(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 430 + 100)), "EliteEnemy1");
			ObjMgr->AddObject(new EliteEnemy2(Vec2(Camera::GetInst()->m_Position.x + 1920 + rand() % 480 + 100, rand() % 430 + 100)), "EliteEnemy2");

			SpawnDelay = 0.f;
		}
	}

}
