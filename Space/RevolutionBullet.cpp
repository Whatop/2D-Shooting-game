#include "stdafx.h"
#include "ChargeBullet.h"
#include "RevolutionBullet.h"
RevolutionBullet::RevolutionBullet(float r)
{
	m_RBullet = Sprite::Create(L"Painting/Bullet/EnemyBullet.png");
	m_RBullet->SetParent(this);
	Spawnpoint = Vec2(GetPlayer->m_Position.x + (GetPlayer->m_Size.x * m_Scale.x) / 2, GetPlayer->m_Position.y - 2);
	SetPosition(Spawnpoint);
	m_Speed = 1400.f;
	DelayTime = 1.f;
	DestroyTime = 0.f;
	m_Layer = 2;
	OneCharge = false;
	KeepRotation = r;
	m_Rotation = GetPlayer->m_Rotation;
}

RevolutionBullet::~RevolutionBullet()
{
}

void RevolutionBullet::Update(float deltaTime, float Time)
{
	if (!GameInfo->isPause) {

		RMove();
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
		m_Atk = 2.f * GameInfo->Player_Coefficient;

	}
}

void RevolutionBullet::Render()
{
	m_RBullet->Render();
}

void RevolutionBullet::OnCollision(Object* obj)
{

}

void RevolutionBullet::Move()
{
	DestroyTime += dt;
	m_Position.x += m_Speed * dt;
	OneCharge = true;
	if (DestroyTime > 10 || m_Position.x > Spawnpoint.x + 1500) {
		ObjMgr->RemoveObject(this);
		GameInfo->ChargeCount--;
	}
}

void RevolutionBullet::RMove()
{
	m_Rotation += D3DXToRadian(KeepRotation);
	Dire.y = cos(m_Rotation);
	Dire.x = sin(m_Rotation);
	Translate(Dire.x * 800.f * dt, Dire.y * 800.f * dt);
}
