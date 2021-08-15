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

	m_Rank = Sprite::Create(L"Painting/Scene/Rank.png");
	m_Rank->SetPosition(1920 / 2, 100);

	m_Back = Sprite::Create(L"Painting/Button/ReturnTitle.png");
	m_Back->SetPosition(1700, 900);

	m_First = new TextMgr();
	m_Secend = new TextMgr();
	m_Third = new TextMgr();

	m_First->Init(72, true, false, "Arial");
	m_Secend->Init(72, true, false, "Arial");
	m_Third->Init(72, true, false, "Arial");
	GameInfo->m_Scene = StageScene::NONE;
}

void RankScene::Release()
{
}

void RankScene::Update(float deltaTime, float Time)
{

	GameMgr::GetInst()->SortRanking();
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
	std::vector<RankingPlayer*> s = GameInfo->Ranks;
	m_First->print("(1) NAME : " + GameInfo->Ranks.at(0)->name + " / SCORE : " + std::to_string(GameInfo->Ranks.at(0)->score), 400, 300);
	m_Secend->print("(2) NAME : " + GameInfo->Ranks.at(1)->name + " / SCORE : " + std::to_string(GameInfo->Ranks.at(1)->score), 400, 500);
	m_Third->print("(3) NAME : " + GameInfo->Ranks.at(2)->name + " / SCORE : " + std::to_string(GameInfo->Ranks.at(2)->score), 400, 700);
	// if문으로 Player 스코어 제어하기
	if(GameInfo->Ranks.at(2)->score > GameInfo->m_Rank->score)
	m_Third->print("(?) NAME : " + GameInfo->m_Rank->name + " / SCORE : " + std::to_string(GameInfo->m_Rank->score), 400, 900);

	Renderer::GetInst()->GetSprite()->End();
}
