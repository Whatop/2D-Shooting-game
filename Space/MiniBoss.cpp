#include "stdafx.h"
#include "MiniBoss.h"
#include "MiniMissile.h"
#include "EnemyRotationBullet.h"
#include "EnemyDirBullet.h"
#include "Item.h"

//미니보스로! 업그레이드
MiniBoss::MiniBoss(Vec2 Pos)
{
	m_Enemy = Sprite::Create(L"Painting/Enemy/Enemy.png");
	m_Enemy->SetParent(this);

	SetPosition(Pos);
	m_RandomPosition = Vec2((rand() % 100 + 400) + m_Position.x, (rand() % 1080));
	m_MaxHp = 1300;
	m_Hp = m_MaxHp;
	m_Rotation = D3DXToRadian(270);
	m_Speed = 450.f;
	m_LastMoveTime = 2.f;
	isMissile = false;
	isBullet = true;
	m_Layer = 2;
	AttackTime = 0.f;
	ones = true;
	SpawnMove = 0.f;
	GameInfo->isMiniBossSpawn = true;
	std::cout << "미니보스 생성" << std::endl;
	GameInfo->EnemyCount++;
	std::cout << m_Hp << std::endl;
	pattern = 0;
	OneDamege = false;
	isPattern = false;
	Acc = 1.f;
}

MiniBoss::~MiniBoss()
{
}

void MiniBoss::Update(float deltaTime, float Time)
{
	if (m_Hp < m_MaxHp / 2 && !isPattern) {
		isPattern = true;
	}
	if (!GameInfo->isPause) {
		if (!OneDamege)
			ObjMgr->CollisionCheak(this, "Boom");
		else {
			DamegeCoolTime += dt;
			if (DamegeCoolTime > 4) {
				DamegeCoolTime = 0.f;
				OneDamege = false;
			}
		}
		if (!isPattern) {
			SpawnMove += dt;
			if (SpawnMove < 2) {
				m_Position.x -= (300 + rand() % 100) * dt;
			}
			else {
				if (ones) {
					m_RandomPosition = Vec2((rand() % 100 + 400) + m_Position.x, (rand() % 580));
					ones = false;
				}
				ObjMgr->CollisionCheak(this, "Bullet");
				ObjMgr->CollisionCheak(this, "ChargeBullet");
				m_LastMoveTime += dt;
				if (m_LastMoveTime >= 5)
					Move();

				if (m_Hp <= 0)
				{
					ObjMgr->AddObject(new Item(m_Position), "ITEM");
					ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Big/", 1, 9, 0.1f, m_Position), "Effect");
					GameInfo->EnemyCount--;
					GameInfo->MaxScore += 1000;
					GameInfo->ChocieScene();

					ObjMgr->RemoveObject(this);
				}

				if (GameInfo->AutoCamera && !GameInfo->CameraStop) {
					m_Position.x += 100 * dt;
				}
				Attack();
				GameInfo->MiniBossHpUpdate(m_MaxHp, m_Hp);
			}
		}
		else {
			ObjMgr->CollisionCheak(this, "Bullet");
			ObjMgr->CollisionCheak(this, "ChargeBullet");
			if (Acc < 7.5f)
				Acc += 3 * dt;
			m_Position.x -= 100 * Acc * dt;

			if (m_Position.x < Camera::GetInst()->m_Position.x - 100 && !One) {
				SetPosition(Camera::GetInst()->m_Position.x + 1920 + 300, -100);
				SetScale(4.f, 4.f);
				m_Enemy->A = 255;
				One = true;
			}
			PatternTime += dt;
			if (PatternTime > 6) {
				//여기서 빈칸 두개 Or 하나
				ObjMgr->AddObject(new EnemyRotationBullet(Vec2(CamPos.x + 1920 + 100, 0), 180), "EnemyBullet");
				//ObjMgr->AddObject(new EnemyRotationBullet(Vec2(CamPos.x + 1920 + 100, 50), 180), "EnemyBullet");
				ObjMgr->AddObject(new EnemyRotationBullet(Vec2(CamPos.x + 1920 + 100, 100), 180), "EnemyBullet");
				ObjMgr->AddObject(new EnemyRotationBullet(Vec2(CamPos.x + 1920 + 100, 150), 180), "EnemyBullet");
				ObjMgr->AddObject(new EnemyRotationBullet(Vec2(CamPos.x + 1920 + 100, 200), 180), "EnemyBullet");
				ObjMgr->AddObject(new EnemyRotationBullet(Vec2(CamPos.x + 1920 + 100, 250), 180), "EnemyBullet");
				ObjMgr->AddObject(new EnemyRotationBullet(Vec2(CamPos.x + 1920 + 100, 300), 180), "EnemyBullet");
				ObjMgr->AddObject(new EnemyRotationBullet(Vec2(CamPos.x + 1920 + 100, 350), 180), "EnemyBullet");
				ObjMgr->AddObject(new EnemyRotationBullet(Vec2(CamPos.x + 1920 + 100, 400), 180), "EnemyBullet");
				ObjMgr->AddObject(new EnemyRotationBullet(Vec2(CamPos.x + 1920 + 100, 450), 180), "EnemyBullet");
				ObjMgr->AddObject(new EnemyRotationBullet(Vec2(CamPos.x + 1920 + 100, 500), 180), "EnemyBullet");
				ObjMgr->AddObject(new EnemyRotationBullet(Vec2(CamPos.x + 1920 + 100, 550), 180), "EnemyBullet");
				ObjMgr->AddObject(new EnemyRotationBullet(Vec2(CamPos.x + 1920 + 100, 600), 180), "EnemyBullet");
				PatternCount++;
				PatternTime = 4;
			}
		}
	}
}

void MiniBoss::Render()
{
	m_Enemy->Render();
}

void MiniBoss::OnCollision(Object* obj)
{
	if (obj->m_Tag == "Bullet") {
		m_Hp -= obj->m_Atk;
		float randx = (rand() % (int)m_Size.x * m_Scale.x) + m_Position.x - m_Size.x / 2 * m_Scale.x;
		float randy = (rand() % (int)m_Size.y * m_Scale.y) + m_Position.y - m_Size.y / 2 * m_Scale.y;
		obj->SetDestroy(true);
		ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Explosion/", 1, 9, 0.1f, Vec2(randx, randy)), "Effect");

	}
	if (obj->m_Tag == "ChargeBullet") {
		m_Hp -= obj->m_Atk;
		float randx = (rand() % (int)m_Size.x * m_Scale.x) + m_Position.x - m_Size.x / 2 * m_Scale.x;
		float randy = (rand() % (int)m_Size.y * m_Scale.y) + m_Position.y - m_Size.y / 2 * m_Scale.y;
		obj->SetDestroy(true);
		ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Explosion/", 1, 9, 0.1f, Vec2(randx, randy)), "Effect");
		GameInfo->RemoveCharge();
		std::cout << m_Hp << std::endl;
	}
	if (obj->m_Tag == "Boom") {
		m_Hp -= obj->m_Atk;
		OneDamege = true;
	}
}

void MiniBoss::Move()
{
	Vec2 A, B, Dire;
	const int EPSILON = 10;

	A = m_Position;
	B = m_RandomPosition;

	Dire = B - A;

	D3DXVec2Normalize(&Dire, &Dire);

	if (abs(m_Position.x - m_RandomPosition.x) > EPSILON && abs(m_Position.y - m_RandomPosition.y) > EPSILON)
	{
		if (m_Position.x > Camera::GetInst()->m_Position.x + App::GetInst()->m_Width / 2 &&
			m_Position.x < Camera::GetInst()->m_Position.x + App::GetInst()->m_Width - 100)

			m_Position.x += Dire.x * m_Speed * dt;

		m_Position.y += Dire.y * m_Speed * dt;
	}
	else
	{

		if (m_Position.y < 360)
			m_RandomPosition.y = (rand() % 400 + 180);
		else
			m_RandomPosition.y = (rand() % 250 + 63);


		if (m_Position.x > Camera::GetInst()->m_Position.x + App::GetInst()->m_Width - 600)
			m_RandomPosition.x = (rand() % -500 - 100) + (int)m_Position.x;
		else
			m_RandomPosition.x = (rand() % 500 + 300) + (int)m_Position.x;

		m_LastMoveTime = 2.f;
	}
}

void MiniBoss::Attack()
{
	if (isBullet) {
		AttackTime += dt;
		if (AttackTime > 0.1f && pattern == 0) {
			ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180), "EnemyBullet");
			ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180 + 50), "EnemyBullet");
			ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180 - 50), "EnemyBullet");
			pattern++;
		}
		if (AttackTime > 0.5f && pattern == 1) {
			ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180), "EnemyBullet");
			ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180 + 20), "EnemyBullet");
			ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180 - 20), "EnemyBullet");
			ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180 + 40), "EnemyBullet");
			ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180 - 40), "EnemyBullet");
			pattern++;
		}
		if (AttackTime > 0.9f && pattern == 2) {
			ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180), "EnemyBullet");
			ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180 + 50), "EnemyBullet");
			ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180 - 50), "EnemyBullet");
			pattern++;
		}
		if (AttackTime > 1.3f && pattern == 3) {
			ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180), "EnemyBullet");
			ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180 + 20), "EnemyBullet");
			ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180 - 20), "EnemyBullet");
			ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180 + 40), "EnemyBullet");
			ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180 - 40), "EnemyBullet");
			isBullet = false;
			AttackTime = 0.f;
			pattern = 0;
		}
	}
	else {
		AttackDelay += dt;
		if (AttackDelay > 2) {
			isBullet = true;
			AttackDelay = 0;


			ObjMgr->AddObject(new MiniMissile(Vec2(m_Position.x + 15, m_Position.y + 22), 190 + -20), "Missile");
			ObjMgr->AddObject(new MiniMissile(Vec2(m_Position.x + 15, m_Position.y - 22), 190 + 20), "Missile");

			ObjMgr->AddObject(new MiniMissile(Vec2(m_Position.x + 15, m_Position.y + 22), 190 + -45), "Missile");
			ObjMgr->AddObject(new MiniMissile(Vec2(m_Position.x + 15, m_Position.y - 22), 190 + 45), "Missile");
		}
	}
}
