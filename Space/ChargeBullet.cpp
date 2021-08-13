#include "stdafx.h"
#include "ChargeBullet.h"
ChargeBullet::ChargeBullet()
{
	m_ChargeBullet = Sprite::Create(L"Painting/Bullet/Spread.png");
	m_ChargeBullet->SetParent(this);
	Spawnpoint = Vec2(GetPlayer->m_Position.x + (GetPlayer->m_Size.x * m_Scale.x) / 2, GetPlayer->m_Position.y - 2);
	SetPosition(Spawnpoint);
	m_Speed = 1400.f;
	DelayTime = 1.f;
	DestroyTime = 0.f;
	m_Layer = 2;
	OneCharge = false;
	GameInfo->ChargeCount++;
}

ChargeBullet::~ChargeBullet()
{
}

void ChargeBullet::Update(float deltaTime, float Time)
{
	if (!GameInfo->isPause) {

		if (!(INPUT->GetKey('Z') == KeyState::PRESS) || OneCharge)
			Move();
		else {
			if (!OneCharge) {
				if (DelayTime < 4)
					DelayTime += dt * 1.5f;

				Spawnpoint = Vec2(GetPlayer->m_Position.x + (GetPlayer->m_Size.x) / 2, GetPlayer->m_Position.y - 2);
				SetPosition(Spawnpoint);
			}

		}
		SetScale(1.f * DelayTime, 1.f * DelayTime);
		m_Atk = 2.f * DelayTime * GameInfo->Player_Coefficient;

	}
}

void ChargeBullet::Render()
{
	m_ChargeBullet->Render();
}

void ChargeBullet::OnCollision(Object* obj)
{

}

void ChargeBullet::Move()
{
	DestroyTime += dt;
	m_Rotation = GetPlayer->m_Rotation;
	Dire.y = cos(m_Rotation);
	Dire.x = sin(m_Rotation);
	Translate(Dire.x * m_Speed * dt, Dire.y * m_Speed* dt);
	OneCharge = true;
	if (DestroyTime > 10 || m_Position.x > Spawnpoint.x + 1500) {
		ObjMgr->RemoveObject(this);
		GameInfo->ChargeCount--;
	}
}
