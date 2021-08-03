#include "stdafx.h"
#include "RotationBullet.h"

RotationBullet::RotationBullet(Vec2 Pos, float r)
{
	m_Bullet = Sprite::Create(L"Painting/Bullet/EnemyBullet.png");
	m_Bullet->SetParent(this);
	SetPosition(Pos);
	m_Rotation = r;
	m_Speed = 500.f;
}

RotationBullet::~RotationBullet()
{
}

void RotationBullet::Update(float deltaTime, float Time)
{
}

void RotationBullet::Render()
{
	m_Bullet->Render();
}

void RotationBullet::OnCollision(Object* obj)
{
}

void RotationBullet::Move()
{
	m_Dire.y = sin(m_Rotation);
	m_Dire.x = cos(m_Rotation);
	D3DXVec2Normalize(&m_Dire, &m_Dire);
	Translate(m_Dire.x * m_Speed * dt, m_Dire.y * m_Speed * dt);

}
