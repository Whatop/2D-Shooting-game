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
#include "Pet.h" 
#include "UI.h" 


Player::Player(float hp)
{
	m_MaxHp = 100;
	m_Hp = hp;
	Init();
	SetPosition(300, 180);
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
	m_Player->m_Layer = 3;
	SetScale(0.65f, 0.65f);

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
	two = false;

}

void Player::Update(float deltaTime, float Time)
{
	
	if (GameInfo->isScoreScene) {
		m_Player->A = 105;
	}
	if (INPUT->GetKey('B') == KeyState::DOWN) {
		ObjMgr->AddObject(new Pet(rand()% 6), "Pet");
	}
	if (INPUT->GetKey('J') == KeyState::DOWN) {
		ObjMgr->AddObject(new Item(Vec2(m_Position.x + 1000,400/2)), "Pet");
	}
	if (!GameInfo->isSpawnEnemy) {
		m_Player->m_Visible = false;
		GameInfo->isGunType = true;
	}
	else {
		m_Player->m_Visible = true;
		if (GameInfo->isGunType == true) {
			m_GunType = GameInfo->HV_TYPE;
			GameInfo->isGunType = false;
		}
	}

	if (!GameInfo->isPause) {
		
		GameInfo->HV_TYPE = m_GunType;
		RpmDelayTime += dt;
		isLeft = false;
		isRight = false;
		isUp = false;
		isDown = false;
		isHit = false;

		ObjMgr->CollisionCheck(this, "Wall");
		ObjMgr->CollisionCheck(this, "EnemyBullet");
		ObjMgr->CollisionCheck(this, "BossBullet");
		ObjMgr->CollisionCheck(this, "Missile");

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
				if (!two  && Damage_Received > m_MaxHp) {
					two = true;
					m_Hp = 1;
				}
				else {
					m_Hp -= Damage_Received;
				}
				float randx = (rand() % (int)m_Size.x * m_Scale.x) + m_Position.x - m_Size.x / 2 * m_Scale.x;
				float randy = (rand() % (int)m_Size.y * m_Scale.y) + m_Position.y - m_Size.y / 2 * m_Scale.y;

				ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Explosion/", 1, 9, 0.1f, Vec2(randx, randy)), "Effect");
				Camera::GetInst()->isVibration = true;
				Camera::GetInst()->ShakeTimeX = 0;

				ones = false;
				SoundMgr* effect = new SoundMgr("Sound/snd_damage.wav", false);
				effect->play();
				effect->volumeSetting(0.12f);
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
	if (INPUT->GetKey('I') == KeyState::DOWN) {
		m_Hp = 0;
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
			UI::GetInst()->PushMessage(L"체력을 회복했다.",false);
		}
		else {
			GameInfo->MaxScore += 500;
			GameInfo->ItemScore += 500;
			UI::GetInst()->PushMessage(L"점수를 획득했다.",false);
		}

		SoundMgr* effect = new SoundMgr("Sound/snd_heal_c.wav", false);
		effect->play();
		effect->volumeSetting(0.12f);
		obj->SetDestroy(true);
	}
	if (obj->m_Tag == "AtkUp") {
		isBuff = true;
		BuffTime = 0.f;
		SoundMgr* effect = new SoundMgr("Sound/snd_heal_c.wav", false);
		effect->play();
		effect->volumeSetting(0.12f);
		UI::GetInst()->PushMessage(L"공격력이 증가했다.", false);
		obj->SetDestroy(true);
	}
	if (obj->m_Tag == "None") {
		if (GameInfo->HV_Boom < 3) {
			GameInfo->HV_Boom++;
			UI::GetInst()->PushMessage(L"폭탄을 먹었다.", false);
		}
		else {
			GameInfo->MaxScore += 550;
			GameInfo->ItemScore += 550;
			UI::GetInst()->PushMessage(L"점수를 획득했다.", false);
		}
		SoundMgr* effect = new SoundMgr("Sound/buffe.wav", false);
		effect->play();
		effect->volumeSetting(0.12f);
		obj->SetDestroy(true);
	}	
	if (obj->m_Tag == "Bonus") {
		GameInfo->MaxScore += 50;
		GameInfo->BonusScore += 50;
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
	ColBox[RIGHT]->SetPosition(m_Position.x + m_Size.x / 2 , m_Position.y);
	ColBox[UP]->SetPosition(m_Position.x, m_Position.y - m_Size.y / 2 + 35);
	ColBox[DOWN]->SetPosition(m_Position.x, m_Position.y + m_Size.y / 2 - 35);
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
				ObjMgr->AddObject(new ShotGun(0,m_Position, false), "Bullet");
				ObjMgr->AddObject(new ShotGun(5, m_Position, false), "Bullet");
				ObjMgr->AddObject(new ShotGun(-5, m_Position, false), "Bullet");
			RpmDelayTime = 0;
			m_Rpm = 0.2f;
		}
	}
	else if (m_GunType == charge) {
			if ((INPUT->GetKey('Z') == KeyState::PRESS) && RpmDelayTime > m_Rpm && GameInfo->ChargeCount < 20) {
				ObjMgr->AddObject(new ChargeBullet, "ChargeBullet");
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
