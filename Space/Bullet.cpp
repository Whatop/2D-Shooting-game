#include "stdafx.h"
#include "Bullet.h"

Bullet::Bullet()
{
	m_Bullet = Sprite::Create(L"Painting/Bullet/Spread.png");
	m_Bullet->SetParent(this);
	Spawnpoint = Vec2(GetPlayer->m_Position.x + (GetPlayer->m_Size.x * m_Scale.x) / 2, GetPlayer->m_Position.y-2);
	SetPosition(Spawnpoint);
	m_Speed = 1000.f;
	DelayTime = 1.f;
	DestroyTime = 0.f;
	m_Layer = 2;
	m_Atk = 35.f;
}

Bullet::~Bullet()
{
	std::cout << "ÃÑ¾Ë Áö¿öÁü" << std::endl;
}

void Bullet::Update(float deltaTime, float Time)
{
	DestroyTime += dt;
	if (DelayTime < 5)
	DelayTime += dt*0.4f;
	Move();

	if (DestroyTime > 4)
		ObjMgr->RemoveObject(this);
}

void Bullet::Render()
{
	m_Bullet->Render();
}

void Bullet::OnCollision(Object* obj)
{

}

void Bullet::Move()
{
	m_Rotation = GetPlayer->m_Rotation;
	Dire.y = cos(m_Rotation);
	Dire.x = sin(m_Rotation);
	Translate(Dire.x * m_Speed*DelayTime * dt, Dire.y * m_Speed* DelayTime * dt);
}
