#include "stdafx.h"
#include "Boss.h"

Boss::Boss()
{
	m_Boss = Sprite::Create(L"Painting/Boss/AllDestroy2.png", COLORKEY_GREEN);
	m_Boss->SetParent(this);
	SetScale(2, 2);
	SetPosition(2300, 480 / 2);
}

Boss::~Boss()
{
}

void Boss::Update(float deltaTime, float Time)
{
}

void Boss::Render()
{
	m_Boss->Render();
}

void Boss::OnCollision(Object* obj)
{
}
