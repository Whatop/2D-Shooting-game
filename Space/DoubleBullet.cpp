#include "stdafx.h"
#include "DoubleBullet.h"

DoubleBullet::DoubleBullet(Vec2 Pos)
{
	m_Spark = new Animation();
	m_Spark->Init(0.1f, true);
	m_Spark->AddContinueFrame(L"Painting/Bullet/Spark/", 1, 5);
	m_Spark->SetParent(this);

	m_DoubleBullet = Sprite::Create(L"Painting/Bullet/Spread.png");
	m_DoubleBullet->SetParent(this);
	m_DoubleBullet->m_Visible = false;
	SetPosition(Pos);
	m_Speed = 600.f;
	DelayTime = 1.f;
	DestroyTime = 0.f;
	m_Layer = 2;
	m_Atk = 5.f * GameInfo->HV_ShotType[5] * 0.5f *GameInfo->Player_Coefficient;
	SetScale(0.75f, 0.75f);

	m_Spark->R = 255;
	m_Spark->G = 100;
	m_Spark->B = 255;

}

DoubleBullet::~DoubleBullet()
{
}

void DoubleBullet::Update(float deltaTime, float Time)
{
	if (!GameInfo->isPause) {
		DestroyTime += dt;
		if (DelayTime < 5)
			DelayTime += dt * 1.1f;
		Move();

		if (DestroyTime > 6)
			ObjMgr->RemoveObject(this);
		m_Spark->Update(deltaTime, Time);
	}
}

void DoubleBullet::Render()
{
	m_Spark->Render();
	m_DoubleBullet->Render();
}

void DoubleBullet::OnCollision(Object* obj)
{
}

void DoubleBullet::Move()
{
	m_Rotation = GetPlayer->m_Rotation;
	Dire.y = cos(m_Rotation);
	Dire.x = sin(m_Rotation);
	Translate(Dire.x * m_Speed * DelayTime * dt, Dire.y * m_Speed * DelayTime * dt);
}
