#include "stdafx.h"
#include "BossBullet.h"

BossBullet::BossBullet(Vec2 spawnPos, Vec2 dir)
{
	m_BossBullet = new Animation();
	m_BossBullet->Init(0.2f, true);
	m_BossBullet->SetParent(this);
	m_BossBullet->AddContinueFrame(L"Painting/Boss/Bullet/Bullet", 0, 1, COLORKEY_GREEN);

	DestroyTime = 0.f;
	m_Speed = 500.f;
	SetScale(2, 2);
	SetPosition(spawnPos);
	Dire = dir;
}

BossBullet::~BossBullet()
{
}

void BossBullet::Update(float deltaTime, float Time)
{
	SetScale(2 + DestroyTime, 2+DestroyTime);

	DestroyTime += dt;
	Move();

	if (DestroyTime > 3.5f) 
		ObjMgr->RemoveObject(this);

	m_BossBullet->Update(deltaTime, Time);
}

void BossBullet::Render()
{
	m_BossBullet->Render();
}

void BossBullet::OnCollision(Object* obj)
{
	if (obj->m_Tag == "Player")
	ObjMgr->RemoveObject(this);
}

void BossBullet::Move()
{
	Translate(Dire.x * m_Speed * dt, Dire.y * m_Speed * dt);
}
