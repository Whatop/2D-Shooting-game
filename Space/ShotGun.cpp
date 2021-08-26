#include "stdafx.h"
#include "ShotGun.h"

ShotGun::ShotGun(float r)
{
	m_ShotGun = new Animation();
	m_ShotGun->Init(0.1f, true);
	m_ShotGun->AddContinueFrame(L"Painting/Bullet/Bolt/enemybolt", 1, 4);
	m_ShotGun->SetParent(this);

	m_ColBox = Sprite::Create(L"Painting/Bullet/Spread.png");
	m_ColBox->SetParent(this);
	m_ColBox->m_Visible = false;
	Spawnpoint = Vec2(GetPlayer->m_Position.x + (GetPlayer->m_Size.x * m_Scale.x) / 2, GetPlayer->m_Position.y - 2);
	SetPosition(Spawnpoint);

	m_Speed = 1250.f;
	DelayTime = 0.f;
	DestroyTime = 0.f;
	m_Rotation = D3DXToRadian(r);
	m_Atk = 7.5f * GameInfo->HV_ShotType[1] * 0.5f * GameInfo->Player_Coefficient;
	m_ShotGun->R = 255;
	m_ShotGun->G = 10;
	m_ShotGun->B = 10;
}

ShotGun::~ShotGun()
{
}

void ShotGun::Update(float deltaTime, float Time)
{
	if (!GameInfo->isPause) {
		DelayDestroy(this, 0.6f);
		Move();
		m_ShotGun->Update(deltaTime, Time);
	}
}
void ShotGun::Render()
{
	m_ShotGun->Render();
	m_ColBox->Render();
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
