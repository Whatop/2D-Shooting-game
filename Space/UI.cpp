#include "stdafx.h"
#include "UI.h"

UI::UI()
{
}

UI::~UI()
{
}

void UI::Init()
{
	PlayerBar = Sprite::Create(L"Painting/UI/Hp.png");
	PlayerBar->SetPosition(390, 900);

	BossBar = Sprite::Create(L"Painting/UI/BossHp.png");
	BossBar->SetPosition(1920 / 2, 100);

	MiniBossBar = Sprite::Create(L"Painting/UI/BossHp.png");
	MiniBossBar->SetPosition(1920 / 2, 100);

	UIScoreFrame = Sprite::Create(L"Painting/UI/ScoreUI2.png");
	UIScoreFrame->SetPosition(1920 / 2, 1080/2);
	
	StateWindow = Sprite::Create(L"Painting/UI/StateWindow.png");
	StateWindow->SetPosition(1920 / 2, 72.f/2.f);

	UIScoreFrame->m_Layer = 2;

	for (int i = 0; i < 6; i++) {
		Pack[i] = Sprite::Create(L"Painting/UI/Pack/" + std::to_wstring(i) + L".png");
		Pack[i]->SetScale(0.35f, 0.35f);
		Pack[i]->SetPosition(790 + i * Pack[i]->m_Size.x * Pack[i]->m_Scale.x, 900);
		ObjMgr->AddObject(Pack[i], "UI");
		Pack[i]->A = 205;
	}

	if (GameInfo->m_Scene != StageScene::STORE) {

		ObjMgr->AddObject(PlayerBar, "UI");
		ObjMgr->AddObject(BossBar, "UI");
		ObjMgr->AddObject(MiniBossBar, "UI");
		ObjMgr->AddObject(UIScoreFrame, "UI");
	}
	ObjMgr->AddObject(StateWindow, "UI");

	for (int i = 0; i < 3; i++) {                 
		Boom[i] = Sprite::Create(L"Painting/UI/Boom.png");
		Boom[i]->SetPosition(100 + 100 * i, 72.f / 2.f);
		ObjMgr->AddObject(Boom[i], "UI");
	}

	m_Test = new TextMgr();
	m_Test->Init(72, true, false, "굴림");
	m_Test->SetColor(255, 255, 50, 50);

	Money = new TextMgr();
	Money->Init(72, true, false, "굴림");
	Money->SetColor(255, 255, 255, 0);

	ScoreText = new TextMgr();
	ScoreText->Init(80, true, false, "굴림");
	ScoreText->SetColor(255, 255, 255, 255);


	StageText = new TextMgr();
	StageText->Init(80, true, false, "굴림");
	StageText->SetColor(255, 255, 155, 255);

	SoundText = new TextMgr();
	SoundText->Init(80, true, false, "굴림");
	SoundText->SetColor(145, 145, 255, 255);
	TextMgr* msgFont = new TextMgr();
	msgFont->Init(48, true, false, "굴림");     // 프로젝트 폰트 스타일 맞춤
	msgFont->SetColor(255, 255, 255, 255);

	// 텍스트바 생성: 화면 중앙 상단 근처(원하는 좌표로 조정)
	MessageBar = new TextBar(1920 / 2 - 180, 700); // x,y 위치
	MessageBar->SetFont(msgFont);
	for (int i = 0; i < 6; ++i) {
		m_CardLvTxt[i] = new TextMgr();
		m_CardLvTxt[i]->Init(24, true, false, "굴림"); // 폰트/크기 프로젝트에 맞춰 조정
		m_CardLvTxt[i]->SetColor(255, 255, 255, 255); // A,R,G,B
	}

	PlayerBar->m_Visible = false;
	BossBar->m_Visible = false;
	MiniBossBar->m_Visible = false;
	UIScoreFrame->m_Visible = false;
	memset(limit, 0, sizeof(limit));
	ScoredaleyTime = 0.f;
}

void UI::Release()
{
}

void UI::Update()
{
	if (MessageBar) MessageBar->Update(dt);
	for (int i = 0; i < 6; i++) {
		Pack[i]->A = 105;
	}
	PlayerBar->A = 105;
	if (GameInfo->isSpawnEnemy && !GameInfo->isScoreScene) {
		Pack[GameInfo->HV_TYPE]->A = 255;
		PlayerBar->A = 255;
	}

	if (GameInfo->isScoreScene) {
		if (ScoredaleyTime < 4)
			ScoredaleyTime += dt;
		else
			ScoreUI();
	}
	else {
		UIScoreFrame->m_Visible = false;
		ScoreText->SetColor(0, 255, 255, 255);
	}
	if (GameInfo->HV_Boom == 1) {
		Boom[0]->m_Visible = true;
		Boom[1]->m_Visible = false;
		Boom[2]->m_Visible = false;
	}
	else if(GameInfo->HV_Boom == 2)
	{
		Boom[0]->m_Visible = true;
		Boom[1]->m_Visible = true;
		Boom[2]->m_Visible = false;
	}
	else if (GameInfo->HV_Boom == 3) {
		for (int i = 0; i < 3; i++) {
			Boom[i]->m_Visible = true;
		}
	}
	else if(GameInfo->HV_Boom <= 0) {
		for (int i = 0; i < 3; i++) {
			Boom[i]->m_Visible = false;
		}
	}
}


void UI::Render()
{
	if (GameInfo->m_isCreatePlayer) {
		float m_PlayerHpGage = PlayerBar->m_Size.x / GameInfo->GetPlayerMaxHp();
		int PlayerHp = GameInfo->GetPlayerMaxHp() - GameInfo->GetPlayerHp();
		if (GameInfo->GetPlayerHp() >= 0)
		{
			SetRect(&PlayerBar->m_Collision, PlayerBar->m_Position.x - PlayerBar->m_Size.x / 2, PlayerBar->m_Position.y - PlayerBar->m_Size.y / 2,
				PlayerBar->m_Position.x + PlayerBar->m_Size.x / 2, PlayerBar->m_Position.y + PlayerBar->m_Size.y / 2);

			PlayerBar->m_Rect.right = PlayerBar->m_Size.x - (PlayerHp * m_PlayerHpGage);
			PlayerBar->m_Visible = true;
		}
	}
	if (GameInfo->isBossSpawn) {
		float m_BossHpGage = BossBar->m_Size.x / GameInfo->GetBossMaxHp();
		int BossHp = GameInfo->GetBossMaxHp() - GameInfo->GetBossHp();
		if (GameInfo->GetBossHp() > 0)
		{
			SetRect(&BossBar->m_Collision, BossBar->m_Position.x - BossBar->m_Size.x / 2, BossBar->m_Position.y - BossBar->m_Size.y / 2,
				BossBar->m_Position.x + BossBar->m_Size.x / 2, BossBar->m_Position.y + BossBar->m_Size.y / 2);

			BossBar->m_Rect.right = BossBar->m_Size.x - (BossHp * m_BossHpGage);
			BossBar->m_Visible = true;
		}
		else {
			BossBar->m_Visible = false;
		}
	}
	else {
		BossBar->m_Visible = false;
	}
	if (GameInfo->isMiniBossSpawn) {
		float m_MiniBossHpGage = MiniBossBar->m_Size.x / GameInfo->GetMiniBossMaxHp();
		int MiniBossHp = GameInfo->GetMiniBossMaxHp() - GameInfo->GetMiniBossHp();
		if (GameInfo->GetMiniBossHp() > 0)
		{
			SetRect(&MiniBossBar->m_Collision, MiniBossBar->m_Position.x - MiniBossBar->m_Size.x / 2, MiniBossBar->m_Position.y - MiniBossBar->m_Size.y / 2,
				MiniBossBar->m_Position.x + MiniBossBar->m_Size.x / 2, MiniBossBar->m_Position.y + MiniBossBar->m_Size.y / 2);

			MiniBossBar->m_Rect.right = MiniBossBar->m_Size.x - (MiniBossHp * m_MiniBossHpGage);
			MiniBossBar->m_Visible = true;
		}
		else {
			MiniBossBar->m_Visible = false;
		}
	}
	else {
		MiniBossBar->m_Visible = false;
	}

	Renderer::GetInst()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	m_Test->print(std::to_string(int(GameInfo->m_Score)), 1920 / 2 - 470, 0);
	Money->print(std::to_string(int(GameInfo->m_Money)), 1920 / 2, 0);
	StageText->print("STAGE " + std::to_string(int(GameInfo->Stage)), 1920 - 360, 0);

	char buf[32];
	std::snprintf(buf, sizeof(buf), "%.f%%", SoundMgr::GetMasterVolume() * 100.f);
	SoundText->print(std::string("소리 : ") + buf, 1920 -800, 0);//m_Test->print("Enemy : " + std::to_string(GameInfo->EnemyCount), 1650, 50);
	if (MessageBar) MessageBar->Render();  // << 여기
	ScoreTextUI();

	for (int i = 0; i < 6; ++i) {

		int lv = GameInfo->HV_ShotType[i];               // 핵심: 해당 타입 레벨

		Vec2 p = Pack[i]->m_Position;
		// Pack 이미지 바로 아래 Y + 80~100 정도로 맞추면 보기 좋음
		m_CardLvTxt[i]->print("LV: " + std::to_string(lv), int(p.x - 35), int(p.y + 95));
	}

	Renderer::GetInst()->GetSprite()->End();
}
void UI::PushMessage(const std::wstring& msg, bool stick) {
	if (MessageBar) MessageBar->Push(msg, stick);
}
void UI::ScoreUI()
{
	//UIScoreFrame->m_Visible = true;
	if (limit[0] <= 1)
		limit[0] += dt;

	if (limit[0] >= 1 && limit[1] <= 1)
		limit[1] += dt;

	if (limit[1] >= 1 && limit[2] <= 1)
		limit[2] += dt;

	if (limit[2] >= 1 && limit[3] <= 1)
		limit[3] += dt;

	for (int i = 0; i < 4; i++) {
		if (limit[i] > 1)
			limit[i] = 1;
	}
}

void UI::ScoreTextUI()
{
	ScoreText->SetColor(255, 255, 255, 255);
	if (limit[0] > 0) {
		ScoreText->print("Kill", 450, 350);
		ScoreText->print("+ " + std::to_string(int(GameInfo->KillScore * limit[0])), 700, 350);
	}
	if (limit[1] > 0) {
		ScoreText->print("Item", 450, 450);
		ScoreText->print("+ " + std::to_string(int(GameInfo->ItemScore * limit[1])), 700, 450);
	}
	if (limit[2] > 0) {
		ScoreText->print("Bonus", 450, 550);
		ScoreText->print("+ " + std::to_string(int(GameInfo->BonusScore * limit[2])), 700, 550);
	}
	if (limit[3] > 0) {
		ScoreText->print("All", 450, 650);
		ScoreText->print("= " + std::to_string(int(GameInfo->m_Score * limit[3])), 700, 650);
	}
}
