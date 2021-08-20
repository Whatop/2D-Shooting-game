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

	CardFrame[0] = Sprite::Create(L"Painting/Store/Frame.png");
	CardFrame[1] = Sprite::Create(L"Painting/Store/Frame.png");
	CardFrame[2] = Sprite::Create(L"Painting/Store/Frame.png");
	CardFrame[3] = Sprite::Create(L"Painting/Store/Frame.png");
	CardFrame[4] = Sprite::Create(L"Painting/Store/Frame.png");

	
	srand(time(NULL));
	for (int i = 0; i < 5; i++) {
		RCrad[i] = rand() % 6 + 1;
		std::cout << RCrad[i] << std::endl;
	
	}
	//음 고민중 같은것을 먹어서 업그레이드? or 그냥 사고 업그레이드 된 카드가 따로있는것.

	CardPack[0] = Sprite::Create(L"Painting/Store/Pack/"+std::to_wstring(RCrad[0])+L".png");
	CardPack[1] = Sprite::Create(L"Painting/Store/Pack/" + std::to_wstring(RCrad[1]) + L".png");
	CardPack[2] = Sprite::Create(L"Painting/Store/Pack/" + std::to_wstring(RCrad[2]) + L".png");
	CardPack[3] = Sprite::Create(L"Painting/Store/Pack/"+std::to_wstring(RCrad[3])+L".png");
	CardPack[4] = Sprite::Create(L"Painting/Store/Pack/"+std::to_wstring(RCrad[4])+L".png");
	
	for (int i = 0; i < 5; i++) {
		CardFrame[i]->SetScale(0.55f, 0.55f);
		CardPack[i]->SetScale(0.55f, 0.55f);
	}

	CardPack[0]->SetPosition(412.5f, 306);
	CardPack[1]->SetPosition(390 + 300, 306);
	CardPack[2]->SetPosition(380 + 600, 306);
	CardPack[3]->SetPosition(360 + 900, 306);
	CardPack[4]->SetPosition(350 + 1200, 306);

	//닿았을때 색깔 변함
	CardFrame[0]->R = 255;
	CardFrame[0]->G = 255;
	CardFrame[0]->B = 51;

	//닿았을때 + 크기변함
	CardFrame[0]->SetScale(0.6f, 0.6f);
	CardPack[0]->SetScale(0.6f, 0.6f);

	CardFrame[0]->SetPosition(412.5f, 325);
	CardFrame[1]->SetPosition(390 + 300, 325);
	CardFrame[2]->SetPosition(380 + 600, 325);
	CardFrame[3]->SetPosition(360 + 900, 325);
	CardFrame[4]->SetPosition(350 + 1200, 325);

	ObjMgr->AddObject(m_BackGround, "UI");

	for (int i = 0; i < 5; i++) {
		ObjMgr->AddObject(CardPack[i], "UI");
		ObjMgr->AddObject(CardFrame[i], "UI");
	}
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