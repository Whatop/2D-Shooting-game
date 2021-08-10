#include "stdafx.h"
#include "MiniMissile.h"

MiniMissile::MiniMissile(Vec2 Pos)
{
	m_Missile = new Animation();
	m_Missile->Init(0.1f, true);
	m_Missile->SetParent(this);
	m_Missile->AddContinueFrame(L"Painting/Boss/Missile/missile", 0, 1);

	m_ColBox = Sprite::Create(L"Painting/Boss/Missile/ColBox.png");
	SetPosition(Pos);
	m_ColBox->SetPosition(Pos);
	m_ColBox->m_Visible = true;

	m_Rotation = D3DXToRadian(190);;
	turnRadian = m_Rotation;
	vrad = 0.009f;
	Delay = 0.f;
	isHoming = false;
	HomingTime = 1.f;
	impellent = 1.f;
	m_Speed = 400.f;
	SetScale(1.f, 1.f);
	m_ColBox->SetScale(0.5f, 0.5f);
	m_ColBox->m_Visible = false;
	DestroyTime = 0.f;
	m_Atk = 10.f;
}
MiniMissile::~MiniMissile()
{
}

void MiniMissile::Update(float deltaTime, float Time)
{
	DestroyTime += dt;
	m_Missile->Update(deltaTime, Time);
	Move();
	m_ColBox->SetPosition(m_Position);
	if (DestroyTime > 2.5f) {
		ObjMgr->RemoveObject(this);
		ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Big/", 1, 9, 0.1f, m_Position), "Effect");
	}

}

void MiniMissile::Render()
{
	m_Missile->Render();
	m_ColBox->Render();
}

void MiniMissile::OnCollision(Object* obj)
{
}

void MiniMissile::Move()
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
		for (auto iter : ObjMgr->m_Objects) {
			if (iter->m_Tag == "Player")
				Enemy = iter->m_Position - m_Position;
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
		DelayDestroy(this, 3);
	}
}
