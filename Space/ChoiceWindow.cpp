#include "stdafx.h"
#include "ChoiceWindow.h"

ChoiceWindow::ChoiceWindow()
{

	srand(time(NULL));
	for (int i = 0; i < 5; i++) {
		RCrad[i] = rand() % 6 + 1;
		std::cout << RCrad[i] << std::endl;
	}

	m_Choice = Sprite::Create(L"Painting/GameScreen/Choice.png");

	ChoicePack[0] = Sprite::Create(L"Painting/Store/Pack/" + std::to_wstring(RCrad[0]) + L".png");
	ChoicePack[1] = Sprite::Create(L"Painting/Store/Pack/" + std::to_wstring(RCrad[1]) + L".png");
	ChoicePack[2] = Sprite::Create(L"Painting/Store/Pack/" + std::to_wstring(RCrad[2]) + L".png");

	m_Choice->SetPosition(1920/2, 1080/2);
	ChoicePack[0]->SetPosition(1920/2 +300,1080/3);
	ChoicePack[1]->SetPosition(1920/2,1080/3);
	ChoicePack[2]->SetPosition(1920/2 - 300,1080/3);

	ObjMgr->AddObject(m_Choice, "UI");
	ObjMgr->AddObject(ChoicePack[0], "UI");
	ObjMgr->AddObject(ChoicePack[1], "UI");
	ObjMgr->AddObject(ChoicePack[2], "UI");

	ChoicePack[0]->SetScale(0.55f, 0.55f);
	ChoicePack[1]->SetScale(0.55f, 0.55f);
	ChoicePack[2]->SetScale(0.55f, 0.55f);
}

ChoiceWindow::~ChoiceWindow()
{
}

void ChoiceWindow::Update(float deltaTime, float Time)
{
}

void ChoiceWindow::Render()
{
}

void ChoiceWindow::OnCollisionCard()
{
}
