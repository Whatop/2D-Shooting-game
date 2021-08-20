#include "stdafx.h"
#include "StoreScene.h"

StoreScene::StoreScene()
{
}

StoreScene::~StoreScene()
{
}

void StoreScene::Init()
{
	ObjMgr->Release();
	GameInfo->ReleaseUI();
	Camera::GetInst()->m_Position = Vec2(0, 0);
	m_BackGround = Sprite::Create(L"Painting/Store/Store.png");
	m_BackGround->SetPosition(1920/2,1080/2);

	NormalCard[0] = Sprite::Create(L"Painting/Store/Card.png");
	NormalCard[0]->SetPosition(412.5f,325);
	NormalCard[0]->SetScale(0.55f, 0.55f);

	ObjMgr->AddObject(m_BackGround, "UI");
	ObjMgr->AddObject(NormalCard[0], "UI");
	GameInfo->m_Scene = StageScene::NONE;
}

void StoreScene::Update(float deltaTime, float Time)
{
	GameInfo->CheatKey();
}

void StoreScene::Render()
{
}

void StoreScene::Release()
{
}