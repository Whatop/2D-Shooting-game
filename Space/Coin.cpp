#include "stdafx.h"
#include "Coin.h"

Coin::Coin(Vec2 Pos)
{
	LoopCoin = new Animation();
	LoopCoin->Init(0.15f, true);
	LoopCoin->SetParent(this);
	LoopCoin->AddContinueFrame(L"Painting/Coin/", 0, 7);
	m_Coin = Sprite::Create(L"Painting/Coin/0.png");
	m_Coin->SetParent(this);
	m_Coin->m_Visible = false;
	SetPosition(Pos);
	m_Speed = 250.f;
	AccTime = 0.f;

	if (rand() % 6 == 0) {
		SetScale(1.5f, 1.5f);
		bigger = true;
	}
	else {
		bigger = false;
		SetScale(1.f, 1.f);
	}
}

Coin::~Coin()
{
}

void Coin::Update(float deltaTime, float Time)
{
	if (!GameInfo->isPause) {
		AccTime += dt;
		ObjMgr->CollisionCheak(this, "MoneyPoket");
	
		Vec2 PoketDir = GameInfo->MoneyPokeyPos  - m_Position;
		D3DXVec2Normalize(&Dire, &PoketDir);
		
		LoopCoin->Update(deltaTime, Time);

		if (AccTime > 3.f) {
			Move();
		}
		m_Position.x += 100 * dt;
	}
	
}

void Coin::Render()
{
	LoopCoin->Render();
}

void Coin::OnCollision(Object* obj)
{
	if (obj->m_Tag == "MoneyPoket") {
		ObjMgr->RemoveObject(this);
		if (bigger) {
			GameInfo->MaxMoney += 5 * pow(1.5f, GameInfo->Stage - 1);
		}
		else 
			GameInfo->MaxMoney += 1 * pow(1.5f, GameInfo->Stage - 1);
	}
}

void Coin::Move()
{
	m_Position.x += 100 * dt;
	Translate(Dire.x * m_Speed * AccTime * dt, Dire.y * m_Speed * AccTime * dt);
}
