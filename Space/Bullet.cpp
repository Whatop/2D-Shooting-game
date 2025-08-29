#include "stdafx.h"
#include "Bullet.h"

Bullet::Bullet(Vec2 spawnpoint, bool minibullet)
{
	m_Bolt = new Animation();
	m_Bolt->Init(0.1f, true);
	m_Bolt->AddContinueFrame(L"Painting/Bullet/Bolt/bolt", 1, 4);
	m_Bolt->SetParent(this);

	m_Bullet = Sprite::Create(L"Painting/Bullet/Spread.png");
	m_Bullet->SetParent(this);
	m_Bullet->m_Visible = false;
	Spawnpoint = Vec2(spawnpoint.x + (GetPlayer->m_Size.x * 0.4f) / 2, spawnpoint.y - 2);

	SetPosition(Spawnpoint);
	m_Speed = 600.f;
	DelayTime = 1.f;
	DestroyTime = 0.f;
	m_Layer = 2;
	m_Atk = 45.f * GameInfo->HV_ShotType[0] * GameInfo->Player_Coefficient;
	if (!minibullet)
		SetScale(1.5f, 1.5f);
	else {
		SetScale(0.75f, 0.75f);
		m_Bolt->A = 105;
	}
}

Bullet::~Bullet()
{
}

void Bullet::Update(float deltaTime, float Time)
{
	if (GameInfo->isScoreScene) {
		m_Bolt->A = 105;
	}
	if (!GameInfo->isPause) {
		DestroyTime += dt;
		if (DelayTime < 5)
			DelayTime += dt * 1.1f;
		Move();

		if (DestroyTime > 6)
			ObjMgr->RemoveObject(this);

		m_Bolt->Update(deltaTime, Time);
	}
}

void Bullet::Render()
{
	m_Bullet->Render();
	m_Bolt->Render();
}

void Bullet::OnCollision(Object* obj)
{

}

void Bullet::Move()
{
	m_Rotation = GetPlayer->m_Rotation;
	Dire.y = cos(m_Rotation);
	Dire.x = sin(m_Rotation);

	Translate(Dire.x * m_Speed*DelayTime * dt, Dire.y * m_Speed* DelayTime * dt);
}
