#include "stdafx.h"
#include "UI.h"

UI::UI()
{
}

UI::~UI()
{
	std::cout << "UI 삭제" << std::endl;
}

void UI::Init()
{
	PlayerBar = Sprite::Create(L"Painting/UI/Hp.png");
	PlayerBar->SetPosition(390, 900);

	BossBar = Sprite::Create(L"Painting/UI/BossHp.png");
	BossBar->SetPosition(1920/2, 100);

	MiniBossBar = Sprite::Create(L"Painting/UI/BossHp.png");
	MiniBossBar->SetPosition(1920/2, 100);


	ObjMgr->AddObject(PlayerBar, "UI");
	ObjMgr->AddObject(BossBar, "UI");
	ObjMgr->AddObject(MiniBossBar, "UI");


	m_Test = new TextMgr();
	m_Test->Init(50, true, false, "굴림");
	m_Test->SetColor(255, 255, 255, 255);

	std::cout << "UI 생성" << std::endl;
	PlayerBar->m_Visible = false;
	BossBar->m_Visible = false;
	MiniBossBar->m_Visible = false;
}

void UI::Release()
{
}

void UI::Update()
{

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
		if (GameInfo->GetBossHp() >= 0)
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
		if (GameInfo->GetMiniBossHp() >= 0)
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
	m_Test->print(std::to_string(gt), 1700, 50);
	Renderer::GetInst()->GetSprite()->End();
}
