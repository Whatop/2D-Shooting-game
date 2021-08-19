#include "stdafx.h"
#include "Pet.h"

Pet::Pet()
{
	m_Pet = Sprite::Create(L"Painting/.png");
	m_Pet->SetParent(this);
	
}

Pet::~Pet()
{
}

void Pet::Update(float deltaTime, float Time)
{
}

void Pet::Render()
{
}

void Pet::OnCollision(Object* obj)
{
}
