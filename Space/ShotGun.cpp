#include "stdafx.h"
#include "ShotGun.h"

ShotGun::ShotGun(float r)
{
	m_ShotGun = Sprite::Create(L"Painting/Bullet/Spread.png");
	m_ShotGun->SetParent(this);
	Spawnpoint = Vec2(GetPlayer->m_Position.x + (GetPlayer->m_Size.x * m_Scale.x) / 2, GetPlayer->m_Position.y - 2);
	SetPosition(Spawnpoint);

	m_Speed = 1250.f;
	DelayTime = 0.f;
	DestroyTime = 0.f;
	m_Rotation = D3DXToRadian(r);
	m_Atk = 7.5f * GameInfo->Player_Coefficient;
}

ShotGun::~ShotGun()
{
}

void ShotGun::Update(float deltaTime, float Time)
{
	if (!GameInfo->isPause) {
		DelayDestroy(this, 0.6f);
		Move();
	}
}
void ShotGun::Render()
{
	m_ShotGun->Render();
}

void ShotGun::OnCollision(Object* obj)
{
}

void ShotGun::Move()
{
	Vec2 m_Dire;
	m_Dire.y = sin(m_Rotation);
	m_Dire.x = cos(m_Rotation);
	D3DXVec2Normalize(&m_Dire, &m_Dire);
	Translate(m_Dire.x * m_Speed * dt, m_Dire.y * m_Speed * dt);
}
