#include "stdafx.h"
#include "Enemy1.h"
#include "MiniMissile.h"

Enemy1::Enemy1(Vec2 Pos)
{
	m_Enemy = Sprite::Create(L"Painting/Enemy/Enemy.png");
	m_Enemy->SetParent(this);

	SetPosition(Pos);
	m_RandomPosition = Vec2((rand() % 100 + 400) + m_Position.x, (rand() % 1080));
	m_Hp = 300;
	m_Rotation = D3DXToRadian(270);
	m_Speed = 450.f;
	m_LastMoveTime = 2.f;

	m_Layer = 2;
}

Enemy1::~Enemy1()
{
}

void Enemy1::Update(float deltaTime, float Time)
{
	m_LastMoveTime += dt;
	if (m_LastMoveTime >= 5)
		Move();

	if (m_Hp <= 0)
	{
		//if ((rand() % 50) == 0)
		//	ObjMgr->AddObject(new Item(m_Position), "ITEM");
		ObjMgr->AddObject(new EffectMgr(L"Painting/Object/Effect/Big/", 1, 9, 5, m_Position), "Effect");
		ObjMgr->RemoveObject(this);
	}
	
	if (GameInfo->AutoCamera && !GameInfo->CameraStop) {
		m_Position.x += 100 * dt;
	}
	ObjMgr->AddObject(new MiniMissile(Vec2(m_Position.x+15,m_Position.y+22)), "Missile");
	ObjMgr->AddObject(new MiniMissile(Vec2(m_Position.x+15,m_Position.y-22)), "Missile");

}

void Enemy1::Render()
{
	m_Enemy->Render();
}

void Enemy1::OnCollision(Object* obj)
{
	
}

void Enemy1::Move()
{
	Vec2 A, B, Dire;
	const int EPSILON = 3;

	A = m_Position;
	B = m_RandomPosition;

	Dire = B - A;

	D3DXVec2Normalize(&Dire, &Dire);

	if (abs(m_Position.x - m_RandomPosition.x) > EPSILON && abs(m_Position.y - m_RandomPosition.y) > EPSILON)
	{
		if(m_Position.x > Camera::GetInst()->m_Position.x + App::GetInst()->m_Width/2 &&
			m_Position.x < Camera::GetInst()->m_Position.x + App::GetInst()->m_Width-100)

		m_Position.x += Dire.x * m_Speed * dt;

		m_Position.y += Dire.y * m_Speed * dt;
	}
	else
	{
	
		if(m_Position.y < 360)
			m_RandomPosition.y = (rand() % 400 + 180);
		else
			m_RandomPosition.y = (rand() % 250 + 63);


		if (m_Position.x > Camera::GetInst()->m_Position.x + App::GetInst()->m_Width - 600)
			m_RandomPosition.x = (rand() %  -500 - 100) + (int)m_Position.x;
		else
			m_RandomPosition.x = (rand() % 500 + 300) + (int)m_Position.x;

		m_LastMoveTime = 2.f;
	}
}
