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
	m_Layer = 2;
	m_Rotation = (std::atan2(dir.y, dir.x));
}

BossBullet::~BossBullet()
{
}

void BossBullet::Update(float deltaTime, float Time)
{
	SetScale(2 + DestroyTime, 2+DestroyTime);

	DestroyTime += dt;
	Move();

	if (DestroyTime > 5) {
		ObjMgr->RemoveObject(this);
		ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Explosion/", 1, 9, 0.1f,m_Position,1*DestroyTime, 1 * DestroyTime), "Effect");
	}
	if (m_Position.y > 600)
		m_Rotation = m_Rotation * -1;
	if (m_Position.y < 40)
		m_Rotation = m_Rotation * -1;
	if (m_Position.x > Camera::GetInst()->m_Position.x + App::GetInst()->m_Width - 100)
		m_Rotation = m_Rotation * -1.5f;
	if (m_Position.x < Camera::GetInst()->m_Position.x + 40)
		m_Rotation = m_Rotation * -0.5f;

	m_BossBullet->Update(deltaTime, Time);
}

void BossBullet::Render()
{
	m_BossBullet->Render();
}

void BossBullet::OnCollision(Object* obj)
{
	if (obj->m_Tag == "Player") {
		ObjMgr->RemoveObject(this);
		ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Explosion/", 1, 9, 0.1f, m_Position, 1 * DestroyTime, 1 * DestroyTime), "Effect");
	}
}

void BossBullet::Move()
{
	Dire.x = cos(m_Rotation);
	Dire.y = sin(m_Rotation);

	m_Rotation = std::atan2f(Dire.y, Dire.x);
	Translate(Dire.x * m_Speed * dt, Dire.y * m_Speed * dt);
}
