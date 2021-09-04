#include "stdafx.h"
#include "Coin.h"

Coin::Coin(Vec2 Pos)
{
	LoopCoin = new Animation();
	LoopCoin->Init(0.1f, true);
	LoopCoin->SetParent(this);
	LoopCoin->AddContinueFrame(L"Painting/Coin/", 0,7);

	m_Coin = Sprite::Create(L"Painting/Coin/0.png");
	m_Coin->SetParent(this);
	m_Coin->m_Visible = false;

	SetPosition(Pos);
	m_Speed = 300.f;
	AccTime = 1.f;
}

Coin::~Coin()
{
}

void Coin::Update(float deltaTime, float Time)
{
	if (!GameInfo->isPause) {
		m_Position.x += 100 * dt;
		LoopCoin->Update(deltaTime, Time);
	}
}

void Coin::Render()
{
	LoopCoin->Render();
	m_Coin->Render();
}

void Coin::OnCollision(Object* obj)
{
}

void Coin::Move()
{
}
