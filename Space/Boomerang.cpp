#include "stdafx.h"
#include "Boomerang.h"

Boomerang::Boomerang(Vec2 spawnpoint)
{

	m_Boomerang = Sprite::Create(L"Painting/Bullet/Direct.png");
	m_Boomerang->SetParent(this);
	Spawnpoint = Vec2(spawnpoint.x + (GetPlayer->m_Size.x * m_Scale.x) / 2, spawnpoint.y - 2);
	SetPosition(Spawnpoint);
	SetScale(0.45f, 0.45f);
	//m_Boomerang->m_Visible = false;
	m_Speed = 950.f;
	DelayTime = 0.f;
	DestroyTime = 0.f;
	m_Atk = 30.f * GameInfo->HV_ShotType[4] * GameInfo->Player_Coefficient;
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
		m_Rotation += D3DXToRadian(2);
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
