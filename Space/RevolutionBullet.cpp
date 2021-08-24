#include "stdafx.h"
#include "RevolutionBullet.h"

RevolutionBullet::RevolutionBullet(float r)
{
	m_RBullet = Sprite::Create(L"Painting/Bullet/EnemyBullet.png");
	m_RBullet->SetParent(this);
	for (auto iter : ObjMgr->m_Objects) {
		if (iter->m_Tag == "ChargeBullet")
			m_Position = iter->m_Position;
	}

	KeepRotation = r;
	m_Speed = 500.f;
	m_Layer = 3;
	m_Atk = 3.f;
}

RevolutionBullet::~RevolutionBullet()
{
}

void RevolutionBullet::Update(float deltaTime, float Time)
{
	//for (auto iter : ObjMgr->m_Objects) {
	//if(iter->m_Tag == "ChargeBullet")
		//m_Position = iter->m_Position;
	//}
	m_Rotation += D3DXToRadian(KeepRotation);
	Move();
}

void RevolutionBullet::Render()
{
	m_RBullet->Render();
}

void RevolutionBullet::OnCollision(Object* obj)
{
}

void RevolutionBullet::Move()
{
	m_Dire.y = sin(m_Rotation);
	m_Dire.x = cos(m_Rotation);
	D3DXVec2Normalize(&m_Dire, &m_Dire);
	Translate(m_Dire.x * m_Speed * dt, m_Dire.y * m_Speed * dt);
}
