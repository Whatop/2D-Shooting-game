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
	m_HpBar = Sprite::Create(L"Painting/UI/Hp.png");
	m_HpBar->SetPosition(390, 900);

	m_BossBar = Sprite::Create(L"Painting/UI/BossHp.png");
	m_BossBar->SetPosition(1920/2, 100);


	ObjMgr->AddObject(m_HpBar, "UI");
	ObjMgr->AddObject(m_BossBar, "UI");


	m_Test = new TextMgr();
	m_Test->Init(50, true, false, "굴림");
	m_Test->SetColor(255, 255, 255, 255);

	std::cout << "UI 생성" << std::endl;
}

void UI::Release()
{
}

void UI::Update()
{

}

void UI::Render()
{
	float m_HpGage = m_HpBar->m_Size.x / GameInfo->GetPlayerMaxHp();
	int Hp = GameInfo->GetPlayerMaxHp() - GameInfo->GetPlayerHp();
	if (GameInfo->GameInfo->GetPlayerHp() >= 0)
	{
		SetRect(&m_HpBar->m_Collision, m_HpBar->m_Position.x - m_HpBar->m_Size.x / 2, m_HpBar->m_Position.y - m_HpBar->m_Size.y / 2,
			m_HpBar->m_Position.x + m_HpBar->m_Size.x / 2, m_HpBar->m_Position.y + m_HpBar->m_Size.y / 2);

		m_HpBar->m_Rect.right = m_HpBar->m_Size.x - (Hp * m_HpGage);
	}
	Renderer::GetInst()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	m_Test->print(std::to_string(GetPlayer->m_Position.x) + " / " + std::to_string(GetPlayer->m_Position.y), 100, 50);
	m_Test->print(std::to_string(gt), 1600, 50);
	Renderer::GetInst()->GetSprite()->End();
}
