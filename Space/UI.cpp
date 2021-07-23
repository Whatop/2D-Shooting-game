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
	Renderer::GetInst()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	m_Test->print(std::to_string(GetPlayer->m_Position.x) + " / " + std::to_string(GetPlayer->m_Position.y), 100, 50);
	m_Test->print(std::to_string(gt), 1600, 50);
	Renderer::GetInst()->GetSprite()->End();
}
