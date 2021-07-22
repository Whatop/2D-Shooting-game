#include "stdafx.h"
#include "MainScene.h"
#include "Player.h"

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
	UpWall->SetPosition(50, -50);
	UpWall->SetScale(100, 1);

	DownWall = Sprite::Create(L"Painting/Wall.png");
	DownWall->SetPosition(50, 700);
	DownWall->SetScale(100, 1);

	ObjMgr->AddObject(m_Map, "Map");
	ObjMgr->AddObject(UpWall, "Wall");
	ObjMgr->AddObject(DownWall, "Wall");

	GameMgr::GetInst()->CreatePlayer();
	std::cout << "메인 장소로 이동" << std::endl;
}

void MainScene::Release()
{
}

void MainScene::Update(float deltaTime, float time)
{
	
}

void MainScene::Render()
{
}
