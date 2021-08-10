#include "stdafx.h"
#include "Missile.h"

Missile::Missile(Vec2 Pos)
{
	m_Missile = new Animation();
	m_Missile->Init(0.1f, true);
	m_Missile->SetParent(this);
	m_Missile->AddContinueFrame(L"Painting/Boss/Missile/missile", 0, 1);

	m_ColBox = Sprite::Create(L"Painting/Boss/Missile/ColBox.png");
	SetPosition(Pos);
	m_ColBox->m_Visible = true;

	m_Rotation = D3DXToRadian(180);;
	turnRadian = m_Rotation;
	vrad = 0.005f;
	Delay = 0.f;
	isHoming = false;
	HomingTime = 1.f;
	impellent = 1.f;
	m_Speed = 400.f;
	SetScale(2.f, 2.f);
	DestroyTime = 0.f;
}

Missile::~Missile()
{
}

void Missile::Update(float deltaTime, float Time)
{
	DestroyTime += dt;
	m_Missile->Update(deltaTime, Time);
	Move();
	if (DestroyTime > 22.f) {
		ObjMgr->RemoveObject(this);
		ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Big/", 1, 9, 0.1f, m_Position), "Effect");
	}
}

void Missile::Render()
{
	m_ColBox->Render();
	m_Missile->Render();
}

void Missile::OnCollision(Object* obj)
{
}

void Missile::Move()
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
			if(iter->m_Tag == "Player")
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
		
	}
}
