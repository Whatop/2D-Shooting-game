#include "stdafx.h"
#include "Boomerang.h"

Boomerang::Boomerang()
{
	m_Boomerang = Sprite::Create(L"Painting/Bullet/Direct.png");
	m_Boomerang->SetParent(this);
	Spawnpoint = Vec2(GetPlayer->m_Position.x + (GetPlayer->m_Size.x * m_Scale.x) / 2, GetPlayer->m_Position.y - 2);
	SetPosition(Spawnpoint);

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
	}
}

void Boomerang::Render()
{
	m_Boomerang->Render();
}

void Boomerang::OnCollision(Object* obj)
{

}

void Boomerang::Move()
{
	m_Position.x += (m_Speed - 350 * DelayTime)* dt;
}
