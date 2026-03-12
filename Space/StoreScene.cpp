#include "stdafx.h"
#include "StoreScene.h"
#include "Stage2.h"
#include "Pet.h" 

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
	GameInfo->m_Scene = StageScene::STORE;

	Camera::GetInst()->m_Position = Vec2(0, 0);

	m_BackGround = Sprite::Create(L"Painting/Store/BG.png");
	m_BackGround->SetPosition(1920 / 2, 1080 / 2);

	m_Button = Sprite::Create(L"Painting/Scene/BackButton.png");
	m_Button->SetPosition(100, 900);
	m_Button->SetScale(0.75f, 0.75f);

	CardFrame[0] = Sprite::Create(L"Painting/Store/Frame.png");
	CardFrame[1] = Sprite::Create(L"Painting/Store/Frame.png");
	CardFrame[2] = Sprite::Create(L"Painting/Store/Frame.png");
	CardFrame[3] = Sprite::Create(L"Painting/Store/Frame.png");
	CardFrame[4] = Sprite::Create(L"Painting/Store/Frame.png");
	
	srand(time(NULL));
	GameInfo->CreateUI();
	for (int i = 0; i < 6; i++) {
		RCrad[i] = rand() % 6;
		PCrad[i] = rand() % 6;
	}

	CardPack[0] = Sprite::Create(L"Painting/Store/Pack/" + std::to_wstring(RCrad[0]) + L".png");
	CardPack[1] = Sprite::Create(L"Painting/Store/Pack/" + std::to_wstring(RCrad[1]) + L".png");
	CardPack[2] = Sprite::Create(L"Painting/Store/Pack/" + std::to_wstring(RCrad[2]) + L".png");
	CardPack[3] = Sprite::Create(L"Painting/Store/Pack/" + std::to_wstring(RCrad[3]) + L".png");
	CardPack[4] = Sprite::Create(L"Painting/Store/Pack/" + std::to_wstring(RCrad[4]) + L".png");

	PetPack[0] = Sprite::Create(L"Painting/Store/Pack/"+std::to_wstring(PCrad[0]) + L".png");
	PetPack[1] = Sprite::Create(L"Painting/Store/Pack/"+std::to_wstring(PCrad[1]) + L".png");
	PetPack[2] = Sprite::Create(L"Painting/Store/Pack/"+std::to_wstring(PCrad[2]) + L".png");
	PetPack[3] = Sprite::Create(L"Painting/Store/Pack/"+std::to_wstring(PCrad[3]) + L".png");
	PetPack[4] = Sprite::Create(L"Painting/Store/Pack/"+std::to_wstring(PCrad[4]) + L".png");
	PetPack[5] = Sprite::Create(L"Painting/Store/Pack/"+std::to_wstring(PCrad[5]) + L".png");

	
	for (int i = 0; i < 5; i++) {
		CardFrame[i]->SetScale(0.55f, 0.55f);
		CardPack[i]->SetScale(0.55f, 0.55f);
	}
	for (int i = 0; i < 6; i++) {
		PetPack[i]->SetScale(0.25f, 0.25f);
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

	for (int i = 0; i < 6;) {
		for (int y = 1; y <= 2; y++) {
			for (int x = 1; x <= 3; x++) {
				PetPack[i]->SetPosition(950 + 150 * x, 520 + 120*y);
				i++;
			}
		}
	}
	ObjMgr->AddObject(m_Button, "UI");

	for (int i = 0; i < 5; i++) {
		ObjMgr->AddObject(CardPack[i], "UI");
		ObjMgr->AddObject(CardFrame[i], "UI");
	}
	for (int i = 0; i < 6; i++) {
		ObjMgr->AddObject(PetPack[i], "UI");
	}
	// [추가] 가격 롤링
	for (int i = 0; i < 5; ++i)  m_CardCost[i] = ((rand() % 6) + 1)* pow(1.5f, GameInfo->Stage - 1);;     // 1~6
	for (int i = 0; i < 6; ++i)  m_PetCost[i] = (rand() % 11) + 10 * pow(1.5f, GameInfo->Stage - 1);;   // 10~20

	   // 가격 텍스트 객체 생성(폰트/색상은 UI와 톤 맞춤)
		for (int i = 0; i < 5; ++i) {
			m_CardCostTxt[i] = new TextMgr();
			m_CardCostTxt[i]->Init(28, true, false, "굴림");
			m_CardCostTxt[i]->SetColor(255, 255, 230, 80); // A,R,G,B
		}
	for (int i = 0; i < 6; ++i) {
		m_PetCostTxt[i] = new TextMgr();
		m_PetCostTxt[i]->Init(26, true, false, "굴림");
		m_PetCostTxt[i]->SetColor(255, 255, 255, 255);
	}

	SoundMgr::GetInst()->StopAll();
	m_Bgm = new SoundMgr("Sound/Store.wav", false);
	m_Bgm->volumeSetting(0.1f);
	m_Bgm->play();

	m_Clickeffect = new SoundMgr("Sound/Click.wav", false);
	m_Clickeffect->volumeSetting(0.1f);
	
	m_BuyEffect = new SoundMgr("Sound/snd_buyitem.wav", false);
	m_BuyEffect->volumeSetting(0.5f);
}

void StoreScene::Update(float deltaTime, float Time)
{
	//GameInfo->CheatKey();

	if (GameInfo->m_Scene == StageScene::STORE)
		OnCollisionCard();
}

void StoreScene::Render()
{ 
	// 가격 텍스트 출력 (팩 바로 아래)
	Renderer::GetInst()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	for (int i = 0; i < 5; ++i) {
		if (m_CardCost[i] >= 0 && CardPack[i]) {
			Vec2 p = CardPack[i]->m_Position;
			m_CardCostTxt[i]->print("Gold: " + std::to_string(m_CardCost[i]),
				int(p.x - 50), int(p.y + 230));
		}
	}
	for (int i = 0; i < 6; ++i) {
		if (m_PetCost[i] >= 0 && PetPack[i]) {
			Vec2 p = PetPack[i]->m_Position;
			m_PetCostTxt[i]->print("Gold: " + std::to_string(m_PetCost[i]),
				int(p.x - 40), int(p.y + 50));
		}
	}

	Renderer::GetInst()->GetSprite()->End();
}

void StoreScene::Release()
{
}

void StoreScene::OnCollisionCard()
{
	// ===== 카드팩(5개): 하이라이트/클릭 처리 =====
	bool anyCardHover = false;
	for (int i = 0; i < 5; ++i)
	{
		if (!CardFrame[i] || !CardPack[i] || m_CardCost[i] < 0) continue;

		if (CollisionMgr::GetInst()->MouseWithBoxSize(CardFrame[i]))
		{
			anyCardHover = true;

			// 하이라이트 + 확대
			CardFrame[i]->R = 255; CardFrame[i]->G = 255; CardFrame[i]->B = 51;
			CardFrame[i]->SetScale(0.6f, 0.6f);
			CardPack[i]->SetScale(0.6f, 0.6f);

			if (INPUT->GetButtonDown()) {
				if (GameInfo->m_Money >= m_CardCost[i]) {
					GameInfo->RemoveMoney(m_CardCost[i]);
					// 무기 타입 데미지 증가 (해당 카드 타입)
					GameInfo->AddCard(RCrad[i]); // 내부에서 HV_ShotType[type] 증가 가정

					// 슬롯 제거 표시
					CardFrame[i]->SetDestroy(true);
					CardPack[i]->SetDestroy(true);
					m_CardCost[i] = -1; // 렌더에서 가격표시 중단
					m_BuyEffect->play();

				}
				else {
					m_BuyEffect->play();
					INPUT->ButtonDown(false);
				}
			}
			break; // 한 슬롯만 활성 처리
		}
	}
	if (!anyCardHover) {
		for (int i = 0; i < 5; i++) {
			if (!CardFrame[i] || !CardPack[i]) continue;
			CardFrame[i]->SetScale(0.55f, 0.55f);
			CardPack[i]->SetScale(0.55f, 0.55f);
			CardFrame[i]->R = 255; CardFrame[i]->G = 255; CardFrame[i]->B = 255;
		}
	}

	// ===== 펫팩(6개): 하이라이트/클릭 처리 =====
	bool anyPetHover = false;
	for (int i = 0; i < 6; ++i)
	{
		if (!PetPack[i] || m_PetCost[i] < 0) continue;

		if (CollisionMgr::GetInst()->MouseWithBoxSize(PetPack[i]))
		{
			anyPetHover = true;

			PetPack[i]->SetScale(0.28f, 0.28f);

			if (INPUT->GetButtonDown()) {
				if (GameInfo->MaxMoney >= m_PetCost[i]) {
					GameInfo->RemoveMoney(m_PetCost[i]);
					GameInfo->AddOwnedPetType(PCrad[i]);  // 이 줄이 핵심
					PetPack[i]->SetDestroy(true);
					m_PetCost[i] = -1;

					m_BuyEffect->play();

				}
				else {
					m_Clickeffect->play();
					INPUT->ButtonDown(false);
				}
			}
			break;
		}
	}
	if (!anyPetHover) {
		for (int i = 0; i < 6; i++) {
			if (!PetPack[i]) continue;
			PetPack[i]->SetScale(0.25f, 0.25f);
		}
	}

	// 뒤로가기
	if (CollisionMgr::GetInst()->MouseWithBoxSize(m_Button) && INPUT->GetButtonDown())
	{
		SceneDirector::GetInst()->ChangeScene(new Stage2());
	}


}
