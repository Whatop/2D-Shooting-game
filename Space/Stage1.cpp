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
	ObjMgr->Release();
	GameInfo->Init();
	GameInfo->m_Score = 0;
	GameInfo->MaxScore = 0;

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

	std::cout << "STAGE 1 이동" << std::endl;
	GameInfo->m_Scene = StageScene::STAGE1;

	if (!GameInfo->m_isCreatePlayer)
		GameMgr::GetInst()->CreatePlayer();

	GameInfo->SpawnChoice();

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
			for (int i = 0; i < 4; i++) {
				m_BackGround[1][i]->m_Position.x -= 10 * dt;
				m_BackGround[2][i]->m_Position.x -= 25 * dt;
				m_BackGround[3][i]->m_Position.x -= 50 * dt;
				m_BackGround[4][i]->m_Position.x -= 75 * dt;
				m_BackGround[5][i]->m_Position.x -= 100 * dt;
			}
		}
		GameInfo->SpawnEnemy();
		GameInfo->CheatKey();

		if (GameInfo->m_Scene == StageScene::STAGE1) {
			for (int i = 0; i < 6; i++) {
				if (m_BackGround[i][0]->m_Position.x + 2400 - 280 <= Camera::GetInst()->m_Position.x) {
					m_BackGround[i][0]->m_Position.x += m_BackGround[0][0]->m_Size.x * 4;
					m_BackGround[i][1]->m_Position.x += m_BackGround[0][1]->m_Size.x * 4;
				}
				if (m_BackGround[i][2]->m_Position.x + 2400 - 280 <= Camera::GetInst()->m_Position.x) {
					m_BackGround[i][2]->m_Position.x += m_BackGround[0][2]->m_Size.x * 4;
					m_BackGround[i][3]->m_Position.x += m_BackGround[0][3]->m_Size.x * 4;
				}
			}
		}
	}
}

void Stage1::Render()
{
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 4; j++) {
			m_BackGround[i][j]->Render();
		}
	}
}

void Stage1::BGInit()
{
	// j = 파일 123456 
	// i = 그거
	int a = 1;
	for (int i = 0; i < 6; i++) {
		m_BackGround[i][0] = Sprite::Create(L"Painting/GameScreen/Stage1/" + std::to_wstring(a) + L".png");
		m_BackGround[i][0]->SetPosition(1200 / 2, 600 / 2);
		m_BackGround[i][1] = Sprite::Create(L"Painting/GameScreen/Stage1/" + std::to_wstring(a) + L".png");
		m_BackGround[i][1]->SetPosition(m_BackGround[i][0]->m_Position.x + m_BackGround[i][0]->m_Size.x, m_BackGround[i][0]->m_Position.y);
		m_BackGround[i][2] = Sprite::Create(L"Painting/GameScreen/Stage1/" + std::to_wstring(a) + L".png");
		m_BackGround[i][2]->SetPosition(m_BackGround[i][1]->m_Position.x + m_BackGround[i][1]->m_Size.x, m_BackGround[i][1]->m_Position.y);
		m_BackGround[i][3] = Sprite::Create(L"Painting/GameScreen/Stage1/" + std::to_wstring(a) + L".png");
		m_BackGround[i][3]->SetPosition(m_BackGround[i][2]->m_Position.x + m_BackGround[i][2]->m_Size.x, m_BackGround[i][2]->m_Position.y);
		a++;
	}
}
