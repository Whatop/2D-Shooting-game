#include "stdafx.h"
#include "EnemyRotationBullet.h"

EnemyRotationBullet::EnemyRotationBullet(Vec2 Pos, float r)
{

	m_Bolt = new Animation();
	m_Bolt->Init(0.1f, true);
	m_Bolt->AddContinueFrame(L"Painting/Bullet/Bolt/enemybolt", 1, 4);
	m_Bolt->SetParent(this);

	m_Bullet = Sprite::Create(L"Painting/Bullet/EnemyBullet.png");
	m_Bullet->SetParent(this);
	SetPosition(Pos);

	m_Rotation = D3DXToRadian(r);
	m_Speed = 500.f;
	m_Atk = 15.f;
	if (GameInfo->m_Scene == StageScene::STAGE1)
	{
		m_Bolt->R = 51;
		m_Bolt->G = 51;
		m_Bolt->B = 51;
	}
	else {
		m_Bolt->R = 255;
		m_Bolt->G = 255;
		m_Bolt->B = 255;
	}
}

EnemyRotationBullet::~EnemyRotationBullet()
{
}

void EnemyRotationBullet::Update(float deltaTime, float Time)
{
	if (GameInfo->isScoreScene) {
		m_Bolt->A = 105;
		m_Bullet->A = 105;
	}
	if (!GameInfo->isPause) {
		Move();
		DelayDestroy(this, 4);
		m_Bolt->Update(deltaTime, Time);
	}

}

void EnemyRotationBullet::Render()
{
	m_Bullet->Render();
	m_Bolt->Render();
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
