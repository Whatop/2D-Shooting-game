#include "stdafx.h"
#include "ChargeBullet.h"
#include "RevolutionBullet.h"
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
	m_Rotation = GetPlayer->m_Rotation;
	if (GameInfo->ChargeCount < 0)
		GameInfo->ChargeCount = 0;

	//추가 능력 
	if (GameInfo->UP_Charge) {
		ObjMgr->AddObject(new RevolutionBullet(40), "Bullet");
		ObjMgr->AddObject(new RevolutionBullet(20), "Bullet");
	}
}

ChargeBullet::~ChargeBullet()
{
}

void ChargeBullet::Update(float deltaTime, float Time)
{
	if (GameInfo->isScoreScene) {
		m_ChargeBullet->A = 105;
	}
	if (!GameInfo->isPause) {

		if (!(INPUT->GetKey('Z') == KeyState::PRESS) || OneCharge)
			Move();
		else {
			if (!OneCharge) {
				if (DelayTime < 4)
					DelayTime += dt * 1.5f;
				else {
					DelayTime = 4;
				}
				Spawnpoint = Vec2(GetPlayer->m_Position.x + (GetPlayer->m_Size.x) / 2, GetPlayer->m_Position.y - 2);
				SetPosition(Spawnpoint);
			}
			
		}
		SetScale(1.f * DelayTime, 1.f * DelayTime);
		m_Atk = 1.5f * GameInfo->HV_ShotType[2] * DelayTime * GameInfo->Player_Coefficient;
	
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
