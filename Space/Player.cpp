#include "stdafx.h"
#include "Player.h"
#include "Bullet.h"
#include "Item.h"
#include "Boomerang.h"
#include "ShotGun.h"
#include "DoubleBullet.h"
#include "ChargeBullet.h"
#include "InputScoreScene.h"
#include "InduceBullet.h"
#include "RevolutionBullet.h" 
#include "Boom.h" 
#include "Pet.h" 


Player::Player(float hp)
{
	m_MaxHp = 100;
	m_Hp = hp;
	Init();
	SetPosition(360, 360);
	GameInfo->PlayerUpdate(this);
	CollisionBox();
	std::cout << "플레이어 생성" << std::endl;
	m_Rotation = D3DXToRadian(90);
	ones = true;
}

Player::~Player()
{
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
	m_Rpm = 0.1f;
	RpmDelayTime = 0.f;
	isLeft = false;
	isRight = false;
	isUp = false;
	isDown = false;
	isHit = false;
	GameInfo->PlayerHpUpdate(m_MaxHp, m_Hp);

	ColBox[LEFT] = Sprite::Create(L"Painting/Player/Height.png");
	ColBox[RIGHT] = Sprite::Create(L"Painting/Player/Height.png");
	ColBox[UP] = Sprite::Create(L"Painting/Player/Width.png");
	ColBox[DOWN] = Sprite::Create(L"Painting/Player/Width.png");
	ColBox[HIT] = Sprite::Create(L"Painting/Player/HitBox.png");
	Defense = Sprite::Create(L"Painting/Skill/Shield.png");

	ColBox[LEFT]->m_Visible = false;
	ColBox[RIGHT]->m_Visible = false;
	ColBox[UP]->m_Visible = false;
	ColBox[DOWN]->m_Visible = false;
	ColBox[HIT]->m_Visible = false;
	Defense->m_Visible = false;
	GameInfo->CreateUI();
	m_GunType = shot;

	defenseTime = 0.f;
	m_Layer = 2;
	shot = 0, shotgun = 1, charge = 2, induce = 3, boomerang = 4, doubleshot = 5;
	BuffTime = 0.f;
	isBuff = false;
	isBuffOnes = true;
}

void Player::Update(float deltaTime, float Time)
{
	GameInfo->HV_TYPE = m_GunType;
	if (INPUT->GetKey('C') == KeyState::DOWN) {
		ObjMgr->AddObject(new Boom, "Boom");
	}
	if (INPUT->GetKey('B') == KeyState::DOWN) {
		ObjMgr->AddObject(new Pet, "Pet");
	}
	if (GameInfo->isSpawnEnemy) {
		m_Player->m_Visible = true;
	}
	else {
		m_Player->m_Visible = false;
	}
	if (!GameInfo->isPause) {
		RpmDelayTime += dt;
		isLeft = false;
		isRight = false;
		isUp = false;
		isDown = false;
		isHit = false;

		ObjMgr->CollisionCheak(this, "Wall");
		ObjMgr->CollisionCheak(this, "EnemyBullet");
		ObjMgr->CollisionCheak(this, "BossBullet");
		ObjMgr->CollisionCheak(this, "Missile");

		GameInfo->PlayerHpUpdate(m_MaxHp, m_Hp);

		Move();
		CollisionBox();
		GameInfo->PlayerUpdate(this);
		Buff();

		if (m_Player->m_Visible == true)
			GunType();
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

		Camera::GetInst()->Side_Scroll(this, 360, GameInfo->AutoCamera);

		if (isHit) {
			if (ones) {
				m_Hp -= Damage_Received;
				float randx = (rand() % (int)m_Size.x * m_Scale.x) + m_Position.x - m_Size.x / 2 * m_Scale.x;
				float randy = (rand() % (int)m_Size.y * m_Scale.y) + m_Position.y - m_Size.y / 2 * m_Scale.y;

				ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Explosion/", 1, 9, 0.1f, Vec2(randx, randy)), "Effect");
				Camera::GetInst()->isVibration = true;
				Camera::GetInst()->ShakeTimeX = 0;

				ones = false;
			}
		}
		if (ones) {
			Defense->m_Visible = false;
			m_Player->R = 255;
			m_Player->G = 255;
			m_Player->B = 255;
		}
		if (!ones) {
			Defense->m_Visible = true;
			defenseTime += dt;
			if (defenseTime > 3.f) {

				ones = true;
				isHit = false;
				defenseTime = 0.f;
			}
			m_Player->R = 255;
			m_Player->G = 30;
			m_Player->B = 30;
		}
		if (m_Hp <= 0) {
			SceneDirector::GetInst()->ChangeScene(new InputScoreScene());
		}
	}
}

void Player::Render()
{
	m_Player->Render();
	ColBox[LEFT]->Render();
	ColBox[RIGHT]->Render();
	ColBox[UP]->Render();
	ColBox[DOWN]->Render();
	ColBox[HIT]->Render();
	Defense->Render();
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
		if (IntersectRect(&rc, &ColBox[4]->m_Collision, &obj->m_Collision)) {
			isHit = true;

			Damage_Received = obj->m_Atk;
			obj->SetDestroy(true);
		}
	}
	if (obj->m_Tag == "Missile") {
		RECT rc;
		if (IntersectRect(&rc, &ColBox[4]->m_Collision, &obj->m_Collision)) {
			isHit = true;
			float randx = (rand() % (int)m_Size.x) + m_Position.x - m_Size.x / 2;
			float randy = (rand() % (int)m_Size.y) + m_Position.y - m_Size.y / 2;
			ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Big/", 1, 9, 0.1f, Vec2(randx, randy)), "Effect");

			Damage_Received = obj->m_Atk;
			obj->SetDestroy(true);
		}
	}
	if (obj->m_Tag == "BossBullet") {
		RECT rc;
		if (IntersectRect(&rc, &ColBox[4]->m_Collision, &obj->m_Collision)) {
			ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Explosion/", 1, 9, 0.1f, m_Position, obj->m_Scale.x, obj->m_Scale.y), "Effect");
			isHit = true;

			Damage_Received = obj->m_Atk;
			obj->SetDestroy(true);
		}
	}
	if (obj->m_Tag == "Heal") {
		if (m_Hp < m_MaxHp) {
			if (m_Hp + 20 < m_MaxHp)
				m_Hp += 20;
			else
				m_Hp = m_MaxHp;
		}
		else {
			GameInfo->MaxScore += 500;
		}
		obj->SetDestroy(true);
	}
	if (obj->m_Tag == "AtkUp") {
		isBuff = true;
		BuffTime = 0.f;
		obj->SetDestroy(true);
	}
	if (obj->m_Tag == "None") {
		m_Hp -= 1;
		obj->SetDestroy(true);
	}	
	if (obj->m_Tag == "Bonus") {
		GameInfo->MaxScore += 100;
		obj->SetDestroy(true);
	}

}

void Player::Move()
{
	if (m_Player->m_Visible == true) {
		if (INPUT->GetKey(VK_SHIFT) == KeyState::PRESS) {
			m_Speed = 440.f / 2;
		}
		else {
			m_Speed = 440.f;
		}
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
	}
	if (GameInfo->AutoCamera && !GameInfo->CameraStop) {
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
	Defense->SetPosition(m_Position);
}

void Player::GunType()
{
	// shot = 1, shotgun = 2, charge = 3, induce = 4, boomerang = 5;
	// IT_Shot;			GameInfo->HV_ShotType[shot]
	// IT_Shotgun;		GameInfo->HV_ShotType[shotgun]
	// IT_Charge;		GameInfo->HV_ShotType[charge]
	// IT_Induce;		GameInfo->HV_ShotType[induce]
	// IT_Boomerang;	GameInfo->HV_ShotType[boomerang]
	// IT_Doubleshot;	GameInfo->HV_ShotType[doubleshot]

	if (m_GunType == shot) {
		if ((INPUT->GetKey('Z') == KeyState::PRESS || INPUT->GetKey('Z') == KeyState::DOWN) && RpmDelayTime > m_Rpm) {
			ObjMgr->AddObject(new Bullet, "Bullet");
			RpmDelayTime = 0;
			m_Rpm = 0.45f;
		}
	}
	else if (m_GunType == shotgun) {
		if ((INPUT->GetKey('Z') == KeyState::PRESS || INPUT->GetKey('Z') == KeyState::DOWN) && RpmDelayTime > m_Rpm) {
				ObjMgr->AddObject(new ShotGun(0), "Bullet");
				ObjMgr->AddObject(new ShotGun(5), "Bullet");
				ObjMgr->AddObject(new ShotGun(-5), "Bullet");
			RpmDelayTime = 0;
			m_Rpm = 0.2f;
		}
	}
	else if (m_GunType == charge) {
			if ((INPUT->GetKey('Z') == KeyState::PRESS) && RpmDelayTime > m_Rpm && GameInfo->ChargeCount < 20) {
				ObjMgr->AddObject(new ChargeBullet, "ChargeBullet");
				//RpmDelayTime = 0;
				m_Rpm = 0.4f;
		}
	}
	else if (m_GunType == induce) {
		if ((INPUT->GetKey('Z') == KeyState::PRESS || INPUT->GetKey('Z') == KeyState::DOWN) && RpmDelayTime > m_Rpm) {
			ObjMgr->AddObject(new InduceBullet, "Bullet");

			RpmDelayTime = 0;
			m_Rpm = 0.15f;
		}
	}
	else if (m_GunType == boomerang) {
		if ((INPUT->GetKey('Z') == KeyState::PRESS || INPUT->GetKey('Z') == KeyState::DOWN) && RpmDelayTime > m_Rpm) {
				ObjMgr->AddObject(new Boomerang, "Bullet");
			RpmDelayTime = 0;
			m_Rpm = 0.35f;
		}
	}
	else if (m_GunType == doubleshot) {
		if ((INPUT->GetKey('Z') == KeyState::PRESS || INPUT->GetKey('Z') == KeyState::DOWN) && RpmDelayTime > m_Rpm) {
			ObjMgr->AddObject(new DoubleBullet(Vec2(m_Position.x + 10, m_Position.y + 20)), "Bullet");
			ObjMgr->AddObject(new DoubleBullet(Vec2(m_Position.x + 10, m_Position.y - 20)), "Bullet");
			RpmDelayTime = 0;
			m_Rpm = 0.1f;
		}
	}
	if (INPUT->GetKey('1') == KeyState::DOWN) {
		m_GunType = shot;
	}
	if (INPUT->GetKey('2') == KeyState::DOWN) {
		m_GunType = shotgun;
	}
	if (INPUT->GetKey('3') == KeyState::DOWN) {
		m_GunType = charge;
	}
	if (INPUT->GetKey('4') == KeyState::DOWN) {
		m_GunType = induce;
	}
	if (INPUT->GetKey('5') == KeyState::DOWN) {
		m_GunType = boomerang;
	}
	if (INPUT->GetKey('6') == KeyState::DOWN) {
		m_GunType = doubleshot;
	}
	if (m_GunType == 6)
		m_GunType = GameInfo->HV_TYPE;
}

void Player::Buff()
{
	if (isBuff) {
		BuffTime += dt;
		if (BuffTime > 10.f) {
			isBuff = false;
			isBuffOnes = true;
			GameInfo->Player_Coefficient -= 1.5f;
			BuffTime = 0.f;
		}
		if (isBuffOnes) {
			GameInfo->Player_Coefficient += 1.5f;
			isBuffOnes = false;
		}
	}
}
