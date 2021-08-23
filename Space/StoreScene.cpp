#include "stdafx.h"
#include "StoreScene.h"
#include "Stage2.h"

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

	m_Button = Sprite::Create(L"Painting/Button/exit.png");
	m_Button->SetPosition(100, 900);
	m_Button->SetScale(0.75f, 0.75f);

	CardFrame[0] = Sprite::Create(L"Painting/Store/Frame.png");
	CardFrame[1] = Sprite::Create(L"Painting/Store/Frame.png");
	CardFrame[2] = Sprite::Create(L"Painting/Store/Frame.png");
	CardFrame[3] = Sprite::Create(L"Painting/Store/Frame.png");
	CardFrame[4] = Sprite::Create(L"Painting/Store/Frame.png");

	
	srand(time(NULL));
	for (int i = 0; i < 5; i++) {
		RCrad[i] = rand() % 6;
		std::cout << RCrad[i] << std::endl;
	}

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



	CardFrame[0]->SetPosition(412.5f, 325);
	CardFrame[1]->SetPosition(390 + 300, 325);
	CardFrame[2]->SetPosition(380 + 600, 325);
	CardFrame[3]->SetPosition(360 + 900, 325);
	CardFrame[4]->SetPosition(350 + 1200, 325);

	ObjMgr->AddObject(m_BackGround, "UI");
	ObjMgr->AddObject(m_Button, "UI");

	for (int i = 0; i < 5; i++) {
		ObjMgr->AddObject(CardPack[i], "UI");
		ObjMgr->AddObject(CardFrame[i], "UI");
	}
	GameInfo->m_Scene = StageScene::NONE;
}

void StoreScene::Update(float deltaTime, float Time)
{
	GameInfo->CheatKey();


	OnCollisionCard();
}

void StoreScene::Render()
{
}

void StoreScene::Release()
{
}

void StoreScene::OnCollisionCard()
{
	if (CollisionMgr::GetInst()->MouseWithBoxSize(CardFrame[0]))
	{
		//닿았을때 색깔 변함
		CardFrame[0]->R = 255;
		CardFrame[0]->G = 255;
		CardFrame[0]->B = 51;

		//닿았을때 + 크기변함
		CardFrame[0]->SetScale(0.6f, 0.6f);
		CardPack[0]->SetScale(0.6f, 0.6f);
		if (INPUT->GetButtonDown()) { // 눌렀을때
			
			//if(돈이 이카드보다 더 많을때라는 조건)
			GameInfo->AddCard(RCrad[0]);
			CardFrame[0]->SetDestroy(true);
			CardPack[0]->SetDestroy(true);
		}
	}
	else if (CollisionMgr::GetInst()->MouseWithBoxSize(CardFrame[1]))
	{
		//닿았을때 색깔 변함
		CardFrame[1]->R = 255;
		CardFrame[1]->G = 255;
		CardFrame[1]->B = 51;

		//닿았을때 + 크기변함
		CardFrame[1]->SetScale(0.6f, 0.6f);
		CardPack[1]->SetScale(0.6f, 0.6f);

		if (INPUT->GetButtonDown()) { // 눌렀을때

			//if(돈이 이카드보다 더 많을때라는 조건)
			GameInfo->AddCard(RCrad[1]);
			CardFrame[1]->SetDestroy(true);
			CardPack[1]->SetDestroy(true);
		}
	}
	else if (CollisionMgr::GetInst()->MouseWithBoxSize(CardFrame[2]))
	{
		//닿았을때 색깔 변함
		CardFrame[2]->R = 255;
		CardFrame[2]->G = 255;
		CardFrame[2]->B = 51;

		//닿았을때 + 크기변함
		CardFrame[2]->SetScale(0.6f, 0.6f);
		CardPack[2]->SetScale(0.6f, 0.6f);

		//if(돈이 이카드보다 더 많을때라는 조건)
		if (INPUT->GetButtonDown()) { // 눌렀을때
			GameInfo->AddCard(RCrad[2]);
			CardFrame[2]->SetDestroy(true);
			CardPack[2]->SetDestroy(true);
		}
	}
	else if (CollisionMgr::GetInst()->MouseWithBoxSize(CardFrame[3]))
	{
		//닿았을때 색깔 변함
		CardFrame[3]->R = 255;
		CardFrame[3]->G = 255;
		CardFrame[3]->B = 51;

		//닿았을때 + 크기변함
		CardFrame[3]->SetScale(0.6f, 0.6f);
		CardPack[3]->SetScale(0.6f, 0.6f);

		//if(돈이 이카드보다 더 많을때라는 조건)
		if (INPUT->GetButtonDown()) { // 눌렀을때
			GameInfo->AddCard(RCrad[3]);
			CardFrame[3]->SetDestroy(true);
			CardPack[3]->SetDestroy(true);
		}
	}
	else if (CollisionMgr::GetInst()->MouseWithBoxSize(CardFrame[4]))
	{
		//닿았을때 색깔 변함
		CardFrame[4]->R = 255;
		CardFrame[4]->B = 51;
		CardFrame[4]->G = 255;

		//닿았을때 + 크기변함
		CardFrame[4]->SetScale(0.6f, 0.6f);
		CardPack[4]->SetScale(0.6f, 0.6f);

		if (INPUT->GetButtonDown()) { // 눌렀을때
		//if(돈이 이카드보다 더 많을때라는 조건)
			GameInfo->AddCard(RCrad[4]);
			CardFrame[4]->SetDestroy(true);
			CardPack[4]->SetDestroy(true);
		}
	}
	else {
		for (int i = 0; i < 5; i++) {

			CardFrame[i]->SetScale(0.55f, 0.55f);
			CardPack[i]->SetScale(0.55f, 0.55f);
			CardFrame[i]->R = 255;
			CardFrame[i]->B = 255;
			CardFrame[i]->G = 255;
		}
	}
	
	if (CollisionMgr::GetInst()->MouseWithBoxSize(m_Button)&& INPUT->GetButtonDown())
	{
		SceneDirector::GetInst()->ChangeScene(new Stage2());
	}

}
