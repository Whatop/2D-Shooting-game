#include "stdafx.h"
#include "Player.h"
#include "Bullet.h"

Player::Player()
{
	Init();
	SetPosition(960, 360);
	GameInfo->PlayerUpdate(this);
	CollisionBox();
	std::cout << "플레이어 생성" << std::endl;
	m_Rotation = D3DXToRadian(90);
}

Player::~Player()
{
	std::cout << "플레이어 삭제" << std::endl;
	GameInfo->ReleaseUI();
}

void Player::Init()
{
	m_Player = Sprite::Create(L"Painting/Player/Player0.png");
	m_Player->SetParent(this);
	SetScale(0.75f, 0.75f);

	LEFT = 0;
	RIGHT = 1;
	UP = 2;
	DOWN = 3;
	HIT = 4;
	m_Speed = 440.f;
	m_MaxHp = 100;
	m_Hp = m_MaxHp;
	m_Rpm = 0.2f;
	RpmDelayTime = 0.f;
	isLeft = false;
	isRight = false;
	isUp = false;
	isDown = false;
	isHit = false;
	GameInfo->PlayerHpUpdate(m_MaxHp,m_Hp);

	ColBox[LEFT] = Sprite::Create(L"Painting/Player/Height.png");
	ColBox[RIGHT] = Sprite::Create(L"Painting/Player/Height.png");
	ColBox[UP] = Sprite::Create(L"Painting/Player/Width.png");
	ColBox[DOWN] = Sprite::Create(L"Painting/Player/Width.png");
	ColBox[HIT] = Sprite::Create(L"Painting/Player/HitBox.png");

	ColBox[LEFT]->m_Visible = false;
	ColBox[RIGHT]->m_Visible = false;
	ColBox[UP]->m_Visible = false;
	ColBox[DOWN]->m_Visible = false;
	ColBox[HIT]->m_Visible = false;
	GameInfo->CreateUI();
}

void Player::Update(float deltaTime, float Time)
{
	RpmDelayTime += dt;
	isLeft = false;
	isRight = false;
	isUp = false;
	isDown = false;
	isHit = false;

	ObjMgr->CollisionCheak(this, "Wall");
	ObjMgr->CollisionCheak(this, "EnemyBullet");
	ObjMgr->CollisionCheak(this, "Missile");

	GameInfo->PlayerHpUpdate(m_MaxHp, m_Hp);

	Move();
	CollisionBox();
	GameInfo->PlayerUpdate(this);
	
	if ((INPUT->GetKey('Z') == KeyState::PRESS || INPUT->GetKey('Z') == KeyState::DOWN)&& RpmDelayTime > m_Rpm) {
		ObjMgr->AddObject(new Bullet, "Bullet");
		RpmDelayTime = 0;
	}
	if (!GameInfo->m_DebugMode) {
		ColBox[LEFT]->m_Visible = false;
		ColBox[RIGHT]->m_Visible = false;
		ColBox[UP]->m_Visible = false;
		ColBox[DOWN]->m_Visible = false;
		ColBox[HIT]->m_Visible = false;
	}
	else {
		ColBox[LEFT]->m_Visible = true;
		ColBox[RIGHT]->m_Visible = true;
		ColBox[UP]->m_Visible = true;
		ColBox[DOWN]->m_Visible = true;
		ColBox[HIT]->m_Visible = true;
	}
	if (INPUT->GetKey(VK_F2) == KeyState::DOWN) {
		m_Hp -= 10;
	}
	Camera::GetInst()->Side_Scroll(this, 360, GameInfo->AutoCamera);
}

void Player::Render()
{
	m_Player->Render();
	ColBox[LEFT]->Render();
	ColBox[RIGHT]->Render();
	ColBox[UP]->Render();
	ColBox[DOWN]->Render();
	ColBox[HIT]->Render();
}

void Player::OnCollision(Object* obj)
{
	if (obj->m_Tag == "Wall")
	{
		RECT rc;
		if (IntersectRect(&rc, &ColBox[0]->m_Collision, &obj->m_Collision))
			isLeft = true;
		if (IntersectRect(&rc, &ColBox[1]->m_Collision, &obj->m_Collision))
			isRight = true;
		if (IntersectRect(&rc, &ColBox[2]->m_Collision, &obj->m_Collision))
			isUp = true;
		if (IntersectRect(&rc, &ColBox[3]->m_Collision, &obj->m_Collision))
			isDown = true;
	}
	if (obj->m_Tag == "EnemyBullet") {
		RECT rc;
		if (IntersectRect(&rc, &ColBox[4]->m_Collision, &obj->m_Collision))
			isHit = true;
	}
}

void Player::Move()
{
	if (!isUp && INPUT->GetKey(VK_UP) == KeyState::PRESS) {
		m_Position.y -= m_Speed * dt;
	}
	if (!isDown && INPUT->GetKey(VK_DOWN) == KeyState::PRESS) {
		m_Position.y += m_Speed * dt;
	}
	if (!isLeft && INPUT->GetKey(VK_LEFT) == KeyState::PRESS) {
		m_Position.x -= m_Speed * dt;
	}
	if (!isRight && INPUT->GetKey(VK_RIGHT) == KeyState::PRESS) {
		m_Position.x += m_Speed * dt;
	}
	if (GameInfo->AutoCamera) {
		m_Position.x += 100 * dt;
	}
}

void Player::CollisionBox()
{
	ColBox[LEFT]->SetPosition(m_Position.x - m_Size.x / 2, m_Position.y);
	ColBox[RIGHT]->SetPosition(m_Position.x + m_Size.x / 2, m_Position.y);
	ColBox[UP]->SetPosition(m_Position.x, m_Position.y - m_Size.y / 2);
	ColBox[DOWN]->SetPosition(m_Position.x, m_Position.y + m_Size.y / 2);
	ColBox[HIT]->SetPosition(m_Position);
}
