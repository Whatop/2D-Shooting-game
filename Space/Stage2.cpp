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
	GameInfo->isSpawnEnemy = false;
	GameInfo->m_Scene = StageScene::STAGE2;
	if(!GameInfo->m_isCreatePlayer)
	GameMgr::GetInst()->CreatePlayer();

	srand(time(NULL));
	for (int i = 0; i < 3; i++) {
		RCrad[i] = rand() % 6;
	}

	m_Choice = Sprite::Create(L"Painting/GameScreen/Choice.png");

	ChoicePack[0] = Sprite::Create(L"Painting/Store/Pack/" + std::to_wstring(RCrad[0]) + L".png");
	ChoicePack[1] = Sprite::Create(L"Painting/Store/Pack/" + std::to_wstring(RCrad[1]) + L".png");
	ChoicePack[2] = Sprite::Create(L"Painting/Store/Pack/" + std::to_wstring(RCrad[2]) + L".png");

	m_Choice->SetPosition(1920 / 2, 1080 / 2);
	ChoicePack[0]->SetPosition(1920 / 2 + 300, 1080 / 3);
	ChoicePack[1]->SetPosition(1920 / 2, 1080 / 3);
	ChoicePack[2]->SetPosition(1920 / 2 - 300, 1080 / 3);

	ChoicePack[0]->SetScale(0.55f, 0.55f);
	ChoicePack[1]->SetScale(0.55f, 0.55f);
	ChoicePack[2]->SetScale(0.55f, 0.55f);
	GameInfo->BossReset();
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
		
		GameInfo->SpawnEnemy();
		MoveBG();
		if(GameInfo->m_Scene == StageScene::STAGE2)
			RestBG();

		if (GameInfo->isSpawnEnemy) {
			m_Choice->m_Visible = false;
			for (int i = 0; i < 3; i++) {
				ChoicePack[i]->m_Visible = false;
			}
		}
		else {
			m_Choice->m_Visible = true;
			for (int i = 0; i < 3; i++) {
				ChoicePack[i]->m_Visible = true;
			}
			OnCollisionCard();
		}
	}
	GameInfo->CheatKey();
}

void Stage2::Render()
{
	for (int i = 9; i > 0; i--) {
		for (int j = 0; j < 4; j++) {
			m_BackGround[i][j]->Render();
		}
	}
	m_Choice->Render();
	for (int i = 0; i < 3; i++) {
		ChoicePack[i]->Render();
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

void Stage2::OnCollisionCard()
{
	if (CollisionMgr::GetInst()->MouseWithBoxSize(ChoicePack[0]))
	{

		//닿았을때 + 크기변함
		ChoicePack[0]->SetScale(0.6f, 0.6f);
		if (INPUT->GetButtonDown()) { // 눌렀을때

			//if(돈이 이카드보다 더 많을때라는 조건)
			GameInfo->AddCard(RCrad[0]);
			INPUT->ButtonDown(false);
			GameInfo->isSpawnEnemy = true;
			GameInfo->HV_TYPE = RCrad[0];
		}
	}
	else if (CollisionMgr::GetInst()->MouseWithBoxSize(ChoicePack[1]))
	{

		//닿았을때 + 크기변함
		ChoicePack[1]->SetScale(0.6f, 0.6f);

		if (INPUT->GetButtonDown()) { // 눌렀을때

			//if(돈이 이카드보다 더 많을때라는 조건)
			GameInfo->AddCard(RCrad[1]);
			INPUT->ButtonDown(false);
			GameInfo->isSpawnEnemy = true;
			GameInfo->HV_TYPE = RCrad[1];
		}
	}
	else if (CollisionMgr::GetInst()->MouseWithBoxSize(ChoicePack[2]))
	{

		//닿았을때 + 크기변함
		ChoicePack[2]->SetScale(0.6f, 0.6f);

		//if(돈이 이카드보다 더 많을때라는 조건)
		if (INPUT->GetButtonDown()) { // 눌렀을때
			GameInfo->AddCard(RCrad[2]);
			INPUT->ButtonDown(false);
			GameInfo->isSpawnEnemy = true;
			GameInfo->HV_TYPE = RCrad[2];
		}
	}
	else {
		for (int i = 0; i < 3; i++) {

			ChoicePack[i]->SetScale(0.55f, 0.55f);
		}
	}
}

void Stage2::RestBG()
{
	for (int i = 0; i < 10; i++) {
		if (m_BackGround[i][0]->m_Position.x + 2400 - 280 <= Camera::GetInst()->m_Position.x) {
			m_BackGround[i][0]->m_Position.x += m_BackGround[0][0]->m_Size.x * 4;
			m_BackGround[i][1]->m_Position.x += -1 + m_BackGround[0][1]->m_Size.x * 4;
		}
		if (m_BackGround[i][2]->m_Position.x + 2400 - 280 <= Camera::GetInst()->m_Position.x) {
			m_BackGround[i][2]->m_Position.x += m_BackGround[0][2]->m_Size.x * 4;
			m_BackGround[i][3]->m_Position.x += -1 + m_BackGround[0][3]->m_Size.x * 4;
		}
	}
}

void Stage2::MoveBG()
{
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
		m_Choice->m_Position.x += 100 * dt;
		for (int i = 0; i < 3; i++) {
			ChoicePack[i]->m_Position.x += 100 * dt;
		}
	}
}
