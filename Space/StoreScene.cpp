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
	m_BackGround = Sprite::Create(L"Painting/Store/BG.png");
	m_BackGround->SetPosition(1920/2,1080/2);

	ObjMgr->AddObject(m_BackGround, "UI");
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
