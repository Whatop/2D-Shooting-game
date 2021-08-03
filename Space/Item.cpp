#include "stdafx.h"
#include "Item.h"

Item::Item(Vec2 Pos)
{
	m_Item = Sprite::Create(L"Painting/Item/Item.png");
	m_Item->SetParent(this);
	SetPosition(Pos);
}

Item::~Item()
{
}

void Item::Update(float deltaTime, float Time)
{
}

void Item::Render()
{
	m_Item->Render();
}

void Item::OnCollision(Object obj)
{
}
