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
	m_Atk = 5.f;
}

BossBullet::~BossBullet()
{
}

void BossBullet::Update(float deltaTime, float Time)
{
	if (!GameInfo->isPause) {
		SetScale(2 + DestroyTime, 2 + DestroyTime);

		DestroyTime += dt;
		Move();

		if (DestroyTime > 5) {
			ObjMgr->RemoveObject(this);
			ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Explosion/", 1, 9, 0.1f, m_Position, 1 * DestroyTime, 1 * DestroyTime), "Effect");
		}
		if (m_Position.y > 600 - m_Size.y / 2)
			m_Rotation = m_Rotation * -1;
		if (m_Position.y < 0 + m_Size.y / 2)
			m_Rotation = m_Rotation * -1;
		if (m_Position.x > Camera::GetInst()->m_Position.x + App::GetInst()->m_Width - m_Size.x / 2)
			m_Rotation = m_Rotation * -1.5f;
		if (m_Position.x < Camera::GetInst()->m_Position.x + m_Size.x / 2)
			m_Rotation = m_Rotation * -0.5f;
		m_Atk = 5.f * m_Scale.x;
		m_BossBullet->Update(deltaTime, Time);
	}
}

void BossBullet::Render()
{
	m_BossBullet->Render();
}

void BossBullet::OnCollision(Object* obj)
{
}

void BossBullet::Move()
{
	Dire.x = cos(m_Rotation);
	Dire.y = sin(m_Rotation);

	m_Rotation = std::atan2f(Dire.y, Dire.x);
	Translate(Dire.x * m_Speed * dt, Dire.y * m_Speed * dt);
}
