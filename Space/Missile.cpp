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
	m_ColBox->SetPosition(Pos);
	m_ColBox->m_Visible = false;
	//Rad = GameMgr::GetInst()->GrenDir;
	turnRadian = std::atan2(Rad.y, Rad.x);
	vrad = 0.019f;
	Delay = 0.f;
	Homing = false;
	DelayTime = 0.f;
}

Missile::~Missile()
{
}

void Missile::Update(float deltaTime, float Time)
{
	
	DelayTime += dt;
	m_Missile->Update(deltaTime, Time);
	Move();
	m_ColBox->SetPosition(m_Position);
	if (DelayTime > 15) {
		ObjMgr->RemoveObject(this);
	}
}

void Missile::Render()
{
	m_Missile->Render();
	m_ColBox->Render();
}

void Missile::OnCollision(Object* other)
{
}

void Missile::Move()
{
	if (!Homing) {
		for (auto iter : ObjMgr->m_Objects) {
			if(iter->m_Tag == "Player")
				Enemy = iter->m_Position - m_Position;
		}
		D3DXVec2Normalize(&Dire, &Enemy);
		Delay += dt;
		if (Delay > 1) {
			vrad += dt * 0.01;
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
		Translate(Dire.x * 700 * dt, Dire.y * 700 * dt);
	}
}
