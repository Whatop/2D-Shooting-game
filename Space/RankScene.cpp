#include "stdafx.h"
#include "RankScene.h"
#include "MainScene.h"

RankScene::RankScene()
{
}

RankScene::~RankScene()
{
}

void RankScene::Init()
{

	m_BG = Sprite::Create(L"Painting/Scene/Black.png");
	m_BG->SetPosition(1920 / 2, 1080 / 2);
	m_BG->SetScale(1.5f, 1.5f);

	m_Rank = Sprite::Create(L"Painting/Scene/Rank.png");
	m_Rank->SetPosition(1920 / 2, 100);

	m_Back = Sprite::Create(L"Painting/Scene/BackButton.png");
	m_Back->SetPosition(1700, 400);

	m_First = new TextMgr();
	m_Secend = new TextMgr();
	m_Third = new TextMgr();

	m_First->Init(72, true, false, "Arial");
	m_Secend->Init(72, true, false, "Arial");
	m_Third->Init(72, true, false, "Arial");
	GameInfo->m_Scene = StageScene::NONE; 
	GameInfo->SortRanking();
}

void RankScene::Release()
{
	SafeDelete(m_BG);
	SafeDelete(m_Rank);
	SafeDelete(m_Back);

	SafeDelete(m_First);
	SafeDelete(m_Secend);
	SafeDelete(m_Third);
}

void RankScene::Update(float deltaTime, float Time)
{

	if (CollisionMgr::GetInst()->MouseWithBoxSize(m_Back) && INPUT->GetButtonDown() == true)
	{
		SceneDirector::GetInst()->ChangeScene(new MainScene());
	}
}

void RankScene::Render()
{
	m_BG->Render();
	m_Back->Render();
	m_Rank->Render();

	Renderer::GetInst()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	if (GameInfo->Ranks.size() > 0)
		m_First->print("(1) NAME : " + GameInfo->Ranks.at(0)->name + " / SCORE : " + std::to_string(GameInfo->Ranks.at(0)->score), 400, 300);

	if (GameInfo->Ranks.size() > 1)
		m_Secend->print("(2) NAME : " + GameInfo->Ranks.at(1)->name + " / SCORE : " + std::to_string(GameInfo->Ranks.at(1)->score), 400, 500);

	if (GameInfo->Ranks.size() > 2)
		m_Third->print("(3) NAME : " + GameInfo->Ranks.at(2)->name + " / SCORE : " + std::to_string(GameInfo->Ranks.at(2)->score), 400, 700);

	if (GameInfo->Ranks.size() > 2 && GameInfo->m_Rank != nullptr)
	{
		if (GameInfo->Ranks.at(2)->score > GameInfo->m_Rank->score)
			m_Third->print("(?) NAME : " + GameInfo->m_Rank->name + " / SCORE : " + std::to_string(GameInfo->m_Rank->score), 400, 900);
	}

	Renderer::GetInst()->GetSprite()->End();
}