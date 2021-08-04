#include "stdafx.h"
#include "Item.h"

Item::Item(Vec2 Pos)
{
	m_Item = Sprite::Create(L"Painting/Item/Item.png");
	m_Item->SetParent(this);
	SetPosition(Pos);
	m_Speed = 300.f;
	m_Rotation = D3DXToRadian(90);
}

Item::~Item()
{
}

void Item::Update(float deltaTime, float Time)
{
	Move();
	if (m_Position.y > 600)
		m_Rotation = m_Rotation * -1;
	if (m_Position.y < 40)
		m_Rotation = m_Rotation * -1 + D3DXToRadian(20);
	if (m_Position.x > Camera::GetInst()->m_Position.x + App::GetInst()->m_Width-100)
		m_Rotation += D3DXToRadian(90);
	if (m_Position.x < Camera::GetInst()->m_Position.x + 40)
		m_Rotation += D3DXToRadian(90);


}

void Item::Render()
{
	m_Item->Render();
}

void Item::OnCollision(Object obj)
{
}

void Item::Move()
{
		m_Dire.x = cos(m_Rotation);
		m_Dire.y = sin(m_Rotation);

	m_Rotation = std::atan2f(m_Dire.y, m_Dire.x);
	Translate(m_Dire.x * m_Speed * dt, m_Dire.y * m_Speed * dt);
}
