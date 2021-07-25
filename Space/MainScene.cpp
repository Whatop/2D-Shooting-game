#include "stdafx.h"
#include "MainScene.h"
#include "Boss.h"

MainScene::MainScene()
{
}

MainScene::~MainScene()
{
}

void MainScene::Init()
{
	m_Map = Sprite::Create(L"Painting/Misson1.png");
	m_Map->SetPosition(m_Map->m_Size.x / 2, m_Map->m_Size.y / 2);

	UpWall = Sprite::Create(L"Painting/Wall.png");
	UpWall->SetPosition(5000, -50);
	UpWall->SetScale(100, 1);

	DownWall = Sprite::Create(L"Painting/Wall.png");
	DownWall->SetPosition(5000, 700);
	DownWall->SetScale(100, 1);

	LeftWall = Sprite::Create(L"Painting/Wall.png");
	LeftWall->SetPosition(-50, 325);
	LeftWall->SetScale(1, 6.5f);

	RightWall = Sprite::Create(L"Painting/Wall.png");
	RightWall->SetPosition(9300, 325);
	RightWall->SetScale(1, 6.5f);

	Left_Limit = Sprite::Create(L"Painting/Wall.png");
	Left_Limit->SetPosition(-50, 325);
	Left_Limit->SetScale(1, 6.5f);

	Right_Limit = Sprite::Create(L"Painting/Wall.png");
	Right_Limit->SetPosition(1970, 325);
	Right_Limit->SetScale(1, 6.5f);
	ObjMgr->AddObject(m_Map, "Map");
	ObjMgr->AddObject(UpWall, "Wall");
	ObjMgr->AddObject(DownWall, "Wall");
	ObjMgr->AddObject(LeftWall, "Wall");
	ObjMgr->AddObject(RightWall, "Wall");
	ObjMgr->AddObject(Left_Limit, "Wall");
	ObjMgr->AddObject(Right_Limit, "Wall");
	ObjMgr->AddObject(new Boss(), "Boss");

	UpWall->m_Visible = false;
	DownWall->m_Visible = false;
	LeftWall->m_Visible = false;
	RightWall->m_Visible = false;

	GameMgr::GetInst()->CreatePlayer();
	std::cout << "메인 장소로 이동" << std::endl;
}

void MainScene::Release()
{
}

void MainScene::Update(float deltaTime, float time)
{
	if (!GameInfo->m_DebugMode) {
		UpWall->m_Visible = false;
		DownWall->m_Visible = false;
		LeftWall->m_Visible = false;
		RightWall->m_Visible = false;
		Left_Limit->m_Visible = false;
		Right_Limit->m_Visible = false;
	}
	else {
		UpWall->m_Visible = true;
		DownWall->m_Visible = true;
		LeftWall->m_Visible = true;
		RightWall->m_Visible = true;
		Left_Limit->m_Visible = true;
		Right_Limit->m_Visible = true;
	}
	if (GameInfo->AutoCamera) {
		Left_Limit->m_Position.x += 100 * dt;
		Right_Limit->m_Position.x += 100 * dt;
	}
}

void MainScene::Render()
{
}
