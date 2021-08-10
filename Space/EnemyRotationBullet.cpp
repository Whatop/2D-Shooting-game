#include "stdafx.h"
#include "EnemyRotationBullet.h"

EnemyRotationBullet::EnemyRotationBullet(Vec2 Pos, float r)
{
	m_Bullet = Sprite::Create(L"Painting/Bullet/EnemyBullet.png");
	m_Bullet->SetParent(this);
	SetPosition(Pos);
	m_Rotation = D3DXToRadian(r);
	m_Speed = 500.f;
	m_Atk = 10.f;
}

EnemyRotationBullet::~EnemyRotationBullet()
{
}

void EnemyRotationBullet::Update(float deltaTime, float Time)
{
	Move();
	DelayDestroy(this, 4);
}

void EnemyRotationBullet::Render()
{
	m_Bullet->Render();
}

void EnemyRotationBullet::OnCollision(Object* obj)
{
}

void EnemyRotationBullet::Move()
{
	m_Dire.y = sin(m_Rotation);
	m_Dire.x = cos(m_Rotation);
	D3DXVec2Normalize(&m_Dire, &m_Dire);
	Translate(m_Dire.x * m_Speed * dt, m_Dire.y * m_Speed * dt);

}
