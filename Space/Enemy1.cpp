#include "stdafx.h"
#include "Enemy1.h"

Enemy1::Enemy1(Vec2 Pos)
{
	m_Enemy = Sprite::Create(L"Painting/Enemy/Enemy.png");
	m_Enemy->SetParent(this);

	m_ColBox[0] = Sprite::Create(L"Painting/Enemy/ColBox.png");
	m_ColBox[0]->SetParent(this);
	SetPosition(Pos);

	LEFT = 0;
	RIGHT = 1;
	UP = 2;
	DOWN = 3;
	MoveNum = 0;
	m_ColBox[LEFT] = Sprite::Create(L"Painting/Enemy/Height.png");
	m_ColBox[RIGHT] = Sprite::Create(L"Painting/Enemy/Height.png");
	m_ColBox[UP] = Sprite::Create(L"Painting/Enemy/ColBox.png");
	m_ColBox[DOWN] = Sprite::Create(L"Painting/Enemy/ColBox.png");
	m_ColBox[LEFT]->SetPosition(m_Position.x - m_Size.x / 2, m_Position.y);
	m_ColBox[RIGHT]->SetPosition(m_Position.x + m_Size.x / 2, m_Position.y);
	m_ColBox[UP]->SetPosition(m_Position.x, m_Position.y - m_Size.y / 2);
	m_ColBox[DOWN]->SetPosition(m_Position.x, m_Position.y + m_Size.y / 2);

	m_Hp = 500;
	m_Rotation = D3DXToRadian(270);
	MoveNum = 0;
	m_Speed = 450.f;
	m_LastMoveTime = 2.f;
}

Enemy1::~Enemy1()
{
}

void Enemy1::Update(float deltaTime, float Time)
{
	isRight = false;
	isLeft = false;
	isUp = false;
	isDown = false;
	m_LastMoveTime += dt;
	ObjMgr->CollisionCheak(this, "Wall");
	if (m_LastMoveTime >= 5)
		Move();
	

	if (!GameInfo->m_DebugMode) {
		m_ColBox[LEFT]->m_Visible = false;
		m_ColBox[RIGHT]->m_Visible = false;
		m_ColBox[UP]->m_Visible = false;
		m_ColBox[DOWN]->m_Visible = false;
	}
	else {
		m_ColBox[LEFT]->m_Visible = true;
		m_ColBox[RIGHT]->m_Visible = true;
		m_ColBox[UP]->m_Visible = true;
		m_ColBox[DOWN]->m_Visible = true;
	}
	if (m_Hp <= 0)
	{
		//if ((rand() % 50) == 0)
		//	ObjMgr->AddObject(new Item(m_Position), "ITEM");
		ObjMgr->AddObject(new EffectMgr(L"Painting/Object/Effect/Big/", 1, 9, 5, m_Position), "Effect");
		ObjMgr->RemoveObject(this);
	}
	m_ColBox[LEFT]->SetPosition(m_Position.x - m_Size.x / 2, m_Position.y);
	m_ColBox[RIGHT]->SetPosition(m_Position.x + m_Size.x / 2, m_Position.y);
	m_ColBox[UP]->SetPosition(m_Position.x, m_Position.y - m_Size.y / 2);
	m_ColBox[DOWN]->SetPosition(m_Position.x, m_Position.y + m_Size.y / 2);

	if (GameInfo->AutoCamera && !GameInfo->CameraStop) {
		m_Position.x += 100 * dt;
	}
}

void Enemy1::Render()
{
	m_Enemy->Render();
	m_ColBox[LEFT]->Render();
	m_ColBox[RIGHT]->Render();
	m_ColBox[UP]->Render();
	m_ColBox[DOWN]->Render();
}

void Enemy1::OnCollision(Object* obj)
{
	if (obj->m_Tag == "Wall")
	{
		RECT rc;
		if (IntersectRect(&rc, &m_ColBox[LEFT]->m_Collision, &obj->m_Collision))
			isLeft = true;
		if (IntersectRect(&rc, &m_ColBox[RIGHT]->m_Collision, &obj->m_Collision))
			isRight = true;
		if (IntersectRect(&rc, &m_ColBox[UP]->m_Collision, &obj->m_Collision))
			isUp = true;
		if (IntersectRect(&rc, &m_ColBox[DOWN]->m_Collision, &obj->m_Collision))
			isDown = true;
	}
}

void Enemy1::Move()
{
	if (MoveTime > 1.4f) {
		if (MoveNum == 0) 
			m_RandomPosition = Vec2(m_Position.x + 100, 150);
		if (MoveNum == 1)
			m_RandomPosition = Vec2(m_Position.x + 600, 480);
		if (MoveNum == 2)
			m_RandomPosition = Vec2(m_Position.x + 100, 150);
		if (MoveNum == 3) {
			m_RandomPosition = Vec2(m_Position.x + 800, 360);
			MoveNum = 0;
		}
		MoveNum++;
		m_LastMoveTime = 0.f;
		MoveTime = 0;
	}
	//좌표 몇개를 만들어주자.

	Vec2 A, B, Dire;
	A = m_Position;
	B = m_RandomPosition;

	Dire = B - A;

	D3DXVec2Normalize(&Dire, &Dire);

	if (!isRight && Dire.x > 0 || !isLeft && Dire.x < 0)
		Translate(Dire.x * m_Speed * dt, 0);
	if (!isUp && Dire.y < 0 || !isDown && Dire.y > 0)
		Translate(0, Dire.y * m_Speed * dt);
	MoveTime += dt;


}
