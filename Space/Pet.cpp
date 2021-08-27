#include "stdafx.h"
#include "Pet.h"

Pet::Pet()
{
	m_Pet = Sprite::Create(L"Painting/Player/Player0.png");
	m_Pet->SetParent(this);
	
	SetPosition(GetPlayer->m_Position.x, GetPlayer->m_Position.x + 200);
	SetScale(0.4f, 0.4f);

	m_Speed = 300.f;
	Limit = 1.f;
	GameInfo->PetCount++;
	std::cout << "펫 생성" << std::endl;
	m_Rotation = D3DXToRadian(90);
}

Pet::~Pet()
{
}

void Pet::Update(float deltaTime, float Time)
{
	if(!GameInfo->isPause)
		Move();
}

void Pet::Render()
{
	m_Pet->Render();
}

void Pet::OnCollision(Object* obj)
{
}

void Pet::Move()
{
	// 근접할수록 천천히 다가가기
	
	//
	Vec2 A, B;
	float S;
	A = m_Position;

	if (GetPlayer->m_Position.y > 360) // 플레이어의 위로
		S = -100.f * GameInfo->PetCount;
	else 
		S = 100.f;

	B = Vec2(GetPlayer->m_Position.x, GetPlayer->m_Position.y + S);

	Dire = B - A;
	Limit = (sqrt(pow(A.x - B.x, 2) + pow(A.y - B.y, 2))/m_Speed*2);

	D3DXVec2Normalize(&Dire, &Dire);
	Translate(Dire.x * m_Speed * Limit * dt, Dire.y * m_Speed * Limit * dt);
}
