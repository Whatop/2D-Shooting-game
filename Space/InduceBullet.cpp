#include "stdafx.h"
#include "InduceBullet.h"
#include "stdafx.h"
#include "Bullet.h"

InduceBullet::InduceBullet()
{
	m_Induce = new Animation();
	m_Induce->Init(0.1f, true);
	m_Induce->AddContinueFrame(L"Painting/Bullet/Bolt/enemybolt", 1, 4);
	m_Induce->SetParent(this);

	m_InduceBullet = Sprite::Create(L"Painting/Bullet/Spread.png");
	m_InduceBullet->SetParent(this);
	m_InduceBullet->m_Visible = false;
	Spawnpoint = Vec2(GetPlayer->m_Position.x + (GetPlayer->m_Size.x * m_Scale.x) / 2, GetPlayer->m_Position.y - 2);
	SetPosition(Spawnpoint);
	m_Speed = 600.f;
	DelayTime = 1.f;
	DestroyTime = 0.f;
	m_Layer = 2;
	m_Atk = 7.5f * GameInfo->Player_Coefficient;
	SetScale(1.f, 1.f);

	m_Induce->R = 50;
	m_Induce->G = 255;
	m_Induce->B = 50;

	turnRadian = m_Rotation;
	vrad = 0.029f;
	Delay = 0.f;
	isHoming = true;
	HomingTime = 1.f;
	impellent = 1.f;
}

InduceBullet::~InduceBullet()
{
}

void InduceBullet::Update(float deltaTime, float Time)
{
	if (!GameInfo->isPause) {
		Iduce();
		DestroyTime += dt;
		if (DelayTime < 5)
			DelayTime += dt * 1.1f;

		if (DestroyTime > 6)
			ObjMgr->RemoveObject(this);

		m_Induce->Update(deltaTime, Time);
	}
}

void InduceBullet::Render()
{
	m_InduceBullet->Render();
	m_Induce->Render();
}

void InduceBullet::OnCollision(Object* obj)
{
}

void InduceBullet::Iduce()
{
	if (!isHoming) {
		HomingTime += dt;
		m_Position.y += 100 * HomingTime * dt;
		if (HomingTime > 1.7f) {
			isHoming = true;
		}
	}
	else {
		if (impellent < 2) {
			impellent += dt;
		}
		if (!GameInfo->isBossSpawn) {
			for (auto iter : ObjMgr->m_Objects) {
				if (iter->m_Tag == "Enemy")
					Enemy = iter->m_Position - m_Position;
			}
		}
		else {
			Enemy = GameInfo->BossPosition - m_Position;
		}
		D3DXVec2Normalize(&Dire, &Enemy);
		Delay += dt;
		if (Delay > 1) {
			vrad += dt * 0.01;
			Delay = 0;
		}
		float pi2 = D3DX_PI * 2;
		float diff = std::atan2f(Dire.y, Dire.x) - turnRadian;
		while (diff < -D3DX_PI) diff += pi2;
		while (diff >= D3DX_PI) diff -= pi2;

		if (abs(diff) < vrad)
			turnRadian += diff;
		else {
			turnRadian += (diff < 0 ? -vrad : vrad);
		}

		Dire.y = sin(turnRadian);
		Dire.x = cos(turnRadian);
		m_Rotation = std::atan2f(Dire.y, Dire.x);
		Translate(Dire.x * m_Speed * impellent * dt, Dire.y * m_Speed * impellent * dt);

	}
}

void InduceBullet::Move()
{
}
