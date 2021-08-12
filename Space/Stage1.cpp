#include "stdafx.h"
#include "Stage1.h"

Stage1::Stage1()
{
}

Stage1::~Stage1()
{
}

void Stage1::Init()
{
	// 임시
	GameInfo->m_Score = 0;
	GameInfo->MaxScore = 0;
	m_GameScreen1 = Sprite::Create(L"Painting/GameScreen/Stage1.png");
	m_GameScreen1->SetPosition(1920 / 2, 650 / 2);
	m_GameScreen2 = Sprite::Create(L"Painting/GameScreen/Stage1.png");
	m_GameScreen2->SetPosition(m_GameScreen1->m_Position.x + m_GameScreen1->m_Size.x, m_GameScreen1->m_Position.y);
	//가로 2188 세로 650 
	//  268 
	Camera::GetInst()->m_Position = Vec2(0, -180);

	UpWall = Sprite::Create(L"Painting/Wall.png");
	UpWall->SetPosition(5000, -50);
	UpWall->SetScale(100, 1);

	DownWall = Sprite::Create(L"Painting/Wall.png");
	DownWall->SetPosition(5000, 700);
	DownWall->SetScale(100, 1);


	Left_Limit = Sprite::Create(L"Painting/Wall.png");
	Left_Limit->SetPosition(-50, 325);
	Left_Limit->SetScale(1, 6.5f);

	Right_Limit = Sprite::Create(L"Painting/Wall.png");
	Right_Limit->SetPosition(1970, 325);
	Right_Limit->SetScale(1, 6.5f);
	ObjMgr->AddObject(UpWall, "Wall");
	ObjMgr->AddObject(DownWall, "Wall");
	ObjMgr->AddObject(Left_Limit, "Wall");
	ObjMgr->AddObject(Right_Limit, "Wall");

	//ObjMgr->AddObject(new Item(Vec2(1920/2,1080/2)), "Boss");
	UpWall->m_Visible = false;
	DownWall->m_Visible = false;

	GameMgr::GetInst()->CreatePlayer();
	std::cout << "STAGE 1 이동" << std::endl;
}

void Stage1::Release()
{
}

void Stage1::Update(float deltaTime, float time)
{
	if (!GameInfo->m_DebugMode) {
		UpWall->m_Visible = false;
		DownWall->m_Visible = false;

		Left_Limit->m_Visible = false;
		Right_Limit->m_Visible = false;
	}
	else {
		UpWall->m_Visible = true;
		DownWall->m_Visible = true;
		Left_Limit->m_Visible = true;
		Right_Limit->m_Visible = true;
	}
	if (!GameInfo->isPause) {
		if (GameInfo->AutoCamera && !GameInfo->CameraStop) {
			UpWall->m_Position.x += 100 * dt;
			DownWall->m_Position.x += 100 * dt;
			Left_Limit->m_Position.x += 100 * dt;
			Right_Limit->m_Position.x += 100 * dt;
		}
		GameInfo->SpawnEnemy();


		if (m_GameScreen1->m_Position.x + 1920 / 2 + 268 / 2 <= Camera::GetInst()->m_Position.x) {
			m_GameScreen1->m_Position.x += m_GameScreen1->m_Size.x * 2;
		}
		if (m_GameScreen2->m_Position.x + 1920 / 2 + 268 / 2 <= Camera::GetInst()->m_Position.x) {
			m_GameScreen2->m_Position.x += m_GameScreen2->m_Size.x * 2;
		}

	}
}

void Stage1::Render()
{
	m_GameScreen1->Render();
	m_GameScreen2->Render();
}
