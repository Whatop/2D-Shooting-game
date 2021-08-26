#include "stdafx.h"
#include "Enemy2.h"
#include "EnemyRotationBullet.h"
#include "Item.h"

Enemy2::Enemy2(Vec2 Pos)
{
	m_Enemy2 = Sprite::Create(L"Painting/Enemy/Enemy2.png");
	m_Enemy2->SetParent(this);

	SetPosition(Pos);
	m_RandomPosition = Vec2((rand() % 100 + 400) + m_Position.x, (rand() % 580));
	m_Hp = 100;
	m_Rotation = D3DXToRadian(180);
	m_Speed = 450.f;
	m_LastMoveTime = 2.f;
	m_Layer = 2;
	SpawnMove = 0.f;
	ones = true;
	std::cout << "Enemy2 »ý¼º" << std::endl;

	GameInfo->EnemyCount++;
	SetScale(0.85f, 0.85f);
	OneDamege = false;
}

Enemy2::~Enemy2()
{
}

void Enemy2::Update(float deltaTime, float Time)
{
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
			if (m_LastMoveTime >= 4)
				Move();
			if (m_Hp <= 0)
			{
				if ((rand() % 30) == 0)
					ObjMgr->AddObject(new Item(m_Position), "Heal");
				ObjMgr->RemoveObject(this);
				ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Big/", 1, 9, 0.1f, m_Position), "Effect");
				GameInfo->EnemyCount--;
				GameInfo->MaxScore += 100;
			}
			if (GameInfo->AutoCamera && !GameInfo->CameraStop) {
				m_Position.x += 100 * dt;
			}
		}
	}
}

void Enemy2::Render()
{
	m_Enemy2->Render();
}

void Enemy2::OnCollision(Object* obj)
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
	}
	if (obj->m_Tag == "Boom") {
		m_Hp -= obj->m_Atk;
		OneDamege = true;
	}
}

void Enemy2::Move()
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

		ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180), "EnemyBullet");
		ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 210), "EnemyBullet");
		ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), -210), "EnemyBullet");
	}
}
