#include "stdafx.h"
#include "Boomerang.h"

Boomerang::Boomerang()
{/*
	m_Crossed = new Animation();
	m_Crossed->Init(0.3f, true);
	m_Crossed->AddContinueFrame(L"Painting/Bullet/Crossed/crossed", 1, 3);
	m_Crossed->SetParent(this);*/

	m_Boomerang = Sprite::Create(L"Painting/Bullet/Direct.png");
	m_Boomerang->SetParent(this);
	Spawnpoint = Vec2(GetPlayer->m_Position.x + (GetPlayer->m_Size.x * m_Scale.x) / 2, GetPlayer->m_Position.y - 2);
	SetPosition(Spawnpoint);
	//m_Boomerang->m_Visible = false;
	m_Boomerang->R = 51;
	m_Boomerang->G = 51;
	m_Boomerang->B = 251;
	m_Speed = 950.f;
	DelayTime = 0.f;
	DestroyTime = 0.f;
	m_Atk = 30.f * GameInfo->Player_Coefficient;
}

Boomerang::~Boomerang()
{
}

void Boomerang::Update(float deltaTime, float Time)
{
	if (!GameInfo->isPause) {
		DelayTime += dt;
		DestroyTime += dt;
		if (DestroyTime > 10.5f) {
			ObjMgr->RemoveObject(this);
		}

		Move();
	//	m_Crossed->Update(deltaTime, Time);
	}
}

void Boomerang::Render()
{
	m_Boomerang->Render();
//	m_Crossed->Render();
}

void Boomerang::OnCollision(Object* obj)
{

}

void Boomerang::Move()
{
	m_Position.x += (m_Speed - 350 * DelayTime)* dt;
}
