#include "stdafx.h"
#include "Stage2.h"

Stage2::Stage2()
{
}

Stage2::~Stage2()
{
}

void Stage2::Init()
{
	ObjMgr->Release();
	GameInfo->Init();

	BGInit();

	Camera::GetInst()->m_Position = Vec2(0, -180);

	UpWall = Sprite::Create(L"Painting/Wall.png");
	UpWall->SetPosition(1920 / 2, -50);
	UpWall->SetScale(19.2f, 1);

	DownWall = Sprite::Create(L"Painting/Wall.png");
	DownWall->SetPosition(1920 / 2, 650);
	DownWall->SetScale(19.2f, 1);


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

	std::cout << "STAGE 2 이동" << std::endl;
	GameInfo->isSpawnEnemy = true;
	GameInfo->m_Scene = StageScene::STAGE2;
	if(!GameInfo->m_isCreatePlayer)
	GameMgr::GetInst()->CreatePlayer();
}

void Stage2::Release()
{
}

void Stage2::Update(float deltaTime, float time)
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
			for (int i = 0; i < 4; i++) {
				m_BackGround[1][i]->m_Position.x += 10 * dt;
				m_BackGround[2][i]->m_Position.x += 20 * dt;
				m_BackGround[3][i]->m_Position.x += 30 * dt;
				m_BackGround[4][i]->m_Position.x += 40 * dt;
				m_BackGround[5][i]->m_Position.x += 50 * dt;
				m_BackGround[6][i]->m_Position.x += 60 * dt;
				m_BackGround[7][i]->m_Position.x += 70 * dt;
				m_BackGround[8][i]->m_Position.x += 80 * dt;
				m_BackGround[9][i]->m_Position.x += 90 * dt;
			}
		}
		GameInfo->SpawnEnemy();
		GameInfo->CheatKey();
		if(GameInfo->m_Scene == StageScene::STAGE2)
		for (int i = 0; i < 10; i++) {
			if (m_BackGround[i][0]->m_Position.x + 2400 - 280 <= Camera::GetInst()->m_Position.x) {
				m_BackGround[i][0]->m_Position.x += m_BackGround[0][0]->m_Size.x * 4;
				m_BackGround[i][1]->m_Position.x += -1+ m_BackGround[0][1]->m_Size.x * 4;
			}
			if (m_BackGround[i][2]->m_Position.x + 2400 - 280 <= Camera::GetInst()->m_Position.x) {
				m_BackGround[i][2]->m_Position.x += m_BackGround[0][2]->m_Size.x * 4;
				m_BackGround[i][3]->m_Position.x += -1 +m_BackGround[0][3]->m_Size.x * 4;
			}
		}

	}
}

void Stage2::Render()
{
	for (int i = 9; i > 0; i--) {
		for (int j = 0; j < 4; j++) {
			m_BackGround[i][j]->Render();
		}
	}
}

void Stage2::BGInit()
{
	// j = 파일 123456789
	// i = 그거
	int a = 1;
	for (int i = 0; i < 10; i++) {
		m_BackGround[i][0] = Sprite::Create(L"Painting/GameScreen/Stage2/" + std::to_wstring(a) + L".png");
		m_BackGround[i][0]->SetPosition(1200 / 2, 600 / 2);
		m_BackGround[i][1] = Sprite::Create(L"Painting/GameScreen/Stage2/" + std::to_wstring(a) + L".png");
		m_BackGround[i][1]->SetPosition(m_BackGround[i][0]->m_Position.x  +m_BackGround[i][0]->m_Size.x, m_BackGround[i][0]->m_Position.y);
		m_BackGround[i][2] = Sprite::Create(L"Painting/GameScreen/Stage2/" + std::to_wstring(a) + L".png");
		m_BackGround[i][2]->SetPosition(m_BackGround[i][1]->m_Position.x  + m_BackGround[i][1]->m_Size.x, m_BackGround[i][1]->m_Position.y);
		m_BackGround[i][3] = Sprite::Create(L"Painting/GameScreen/Stage2/" + std::to_wstring(a) + L".png");
		m_BackGround[i][3]->SetPosition(m_BackGround[i][2]->m_Position.x + m_BackGround[i][2]->m_Size.x, m_BackGround[i][2]->m_Position.y);
		a++;
	}
}
