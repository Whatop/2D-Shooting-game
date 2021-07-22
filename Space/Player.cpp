#include "stdafx.h"
#include "Player.h"

Player::Player()
{
	Init();
	SetPosition(960, 360);
	GM->PlayerPosUpdate(m_Position);
	CollisionBox();
	std::cout << "플레이어 생성" << std::endl;
}

Player::~Player()
{
	std::cout << "플레이어 삭제" << std::endl;
	GameMgr::GetInst()->ReleaseUI();
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
	m_Speed = 540.f;
	m_Hp = 5;
	isLeft = false;
	isRight = false;
	isUp = false;
	isDown = false;
	isHit = false;
	AutoCamera = false;

	ColBox[LEFT] = Sprite::Create(L"Painting/Player/Height.png");
	ColBox[RIGHT] = Sprite::Create(L"Painting/Player/Height.png");
	ColBox[UP] = Sprite::Create(L"Painting/Player/Width.png");
	ColBox[DOWN] = Sprite::Create(L"Painting/Player/Width.png");
	ColBox[HIT] = Sprite::Create(L"Painting/Player/HitBox.png");
	GameMgr::GetInst()->CreateUI();
}

void Player::Update(float deltaTime, float Time)
{
	isLeft = false;
	isRight = false;
	isUp = false;
	isDown = false;
	isHit = false;

	ObjMgr->CollisionCheak(this, "Wall");

	Move();
	CollisionBox();
	GM->PlayerPosUpdate(m_Position);
	if (INPUT->GetKey(VK_F1) == KeyState::DOWN) {
		if (!AutoCamera) {
			AutoCamera = true;
			std::cout << "AUTO 카메라 OFF" << std::endl;
		}
		else {
			AutoCamera = false;
			std::cout << "AUTO 카메라 ON" << std::endl;
		}
	}
	//Camera::GetInst()->Side_Scroll(this, 360, AutoCamera);
	Camera::GetInst()->Follow(this);
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
	if (obj->m_Tag == "eBullet") {
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
	if (!isLeft&& INPUT->GetKey(VK_LEFT) == KeyState::PRESS) {
		m_Position.x -= m_Speed * dt;
	}
	if (!isRight && INPUT->GetKey(VK_RIGHT) == KeyState::PRESS) {
		m_Position.x += m_Speed * dt;
	}
	if (AutoCamera) {
			m_Position.x += 100 * dt;
	}
}

void Player::CollisionBox()
{
	ColBox[LEFT]->SetPosition(m_Position.x - m_Size.x / 2, m_Position.y);
	ColBox[RIGHT]->SetPosition(m_Position.x + m_Size.x / 2,m_Position.y);
	ColBox[UP]->SetPosition(m_Position.x, m_Position.y - m_Size.y / 2);
	ColBox[DOWN]->SetPosition(m_Position.x, m_Position.y + m_Size.y / 2);
	ColBox[HIT]->SetPosition(m_Position);
}
