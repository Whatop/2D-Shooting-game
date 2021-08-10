#include "stdafx.h"
#include "EnemyDirBullet.h"

EnemyDirBullet::EnemyDirBullet(Vec2 Pos,Vec2 Dir)
{
	m_Bullet = Sprite::Create(L"Painting/Bullet/EnemyBullet.png");
	m_Bullet->SetParent(this);
	SetPosition(Pos);
	m_Dire = Dir;
	m_Speed = 500.f;
	m_Rotation = (std::atan2(m_Dire.y, m_Dire.x));  
	m_Atk = 10.f;
}

EnemyDirBullet::~EnemyDirBullet()
{
}

void EnemyDirBullet::Update(float deltaTime, float Time)
{
	Move();
	DelayDestroy(this, 4);
}

void EnemyDirBullet::Render()
{
	m_Bullet->Render();
}

void EnemyDirBullet::OnCollision(Object* obj)
{
}

void EnemyDirBullet::Move()
{
	D3DXVec2Normalize(&m_Dire, &m_Dire);
	Translate(m_Dire.x * m_Speed * dt, m_Dire.y * m_Speed * dt);
}

