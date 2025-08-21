#include "stdafx.h"
#include "Enemy3.h"
#include "EnemyDirBullet.h"
#include "EnemyRotationBullet.h"
#include "Item.h"

Enemy3::Enemy3(Vec2 Pos)
{
	m_Enemy3 = Sprite::Create(L"Painting/Enemy/Enemy2.png");
	m_Enemy3->SetParent(this);

	SetPosition(Pos);
	m_RandomPosition = Vec2((rand() % 100 + 400) + m_Position.x, (rand() % 580));
	m_Hp = 100;
	m_Rotation = D3DXToRadian(180);
	m_Speed = 350.f;
	m_LastMoveTime = 1.f;
	m_Layer = 2;
	SpawnMove = 0.f;
	ones = true;
	std::cout << "Enemy3 »ý¼º" << std::endl;

	isBoomMode = false;
	GameInfo->EnemyCount++;
	turnRadian = m_Rotation;	
	vrad = 0.009f;
	Delay = 0.f;
	isHoming = false;
	HomingTime = 1.f;
	impellent = 1.f;
	SetScale(0.5f, 0.5f);
	DestroyTime = 0.f;
	Changecount = 1;
	OneDamege = false;
}

Enemy3::~Enemy3()
{
}

void Enemy3::Update(float deltaTime, float Time)
{
	if (!GameInfo->isPause) {
		SpawnMove += dt;
		if (!OneDamege)
			ObjMgr->CollisionCheak(this, "Boom");
		else {
			DamegeCoolTime += dt;
			if (DamegeCoolTime > 4) {
				DamegeCoolTime = 0.f;
				OneDamege = false;
			}
		}
		if (SpawnMove < 2) {
			m_Position.x -= (300 + rand() % 100) * dt;
			ObjMgr->CollisionCheak(this, "Bullet");
			ObjMgr->CollisionCheak(this, "ChargeBullet");
		}
		else {
			if (ones) {
				m_RandomPosition = Vec2((rand() % 100 + 400) + m_Position.x, (rand() % 360 + 73));
				ones = false;
			}
			ObjMgr->CollisionCheak(this, "Bullet");
			ObjMgr->CollisionCheak(this, "ChargeBullet");
			m_LastMoveTime += dt;
			if (m_LastMoveTime >= 2 || isBoomMode)
				Move();
			if (m_Hp <= 0)
			{
				if ((rand() % 20) == 0)
					ObjMgr->AddObject(new Item(m_Position), "Heal");
				ObjMgr->RemoveObject(this);
				ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Big/", 1, 9, 0.1f, m_Position), "Effect");
				GameInfo->EnemyCount--;
				GameInfo->MaxScore += 100;
				GameInfo->KillScore += 100;
				GameInfo->SpawnCoin(m_Position);
			}
			if (GameInfo->AutoCamera && !GameInfo->CameraStop) {
				m_Position.x += 100 * dt;
			}
		}
		if (Changecount > 3)
			isBoomMode = true;
	}
}

void Enemy3::Render()
{
	m_Enemy3->Render();
}

void Enemy3::OnCollision(Object* obj)
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


void Enemy3::Move()
{
	if (!isBoomMode) {
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
			Changecount++;

			const float h = App::GetInst()->m_Height;
			const float scaleH = h / 1080.f;
			const float minY = -60.f * scaleH;
			const float maxY = 387.f * scaleH;

			m_RandomPosition.y = RandRange(minY, maxY);

			if (m_Position.x > Camera::GetInst()->m_Position.x + App::GetInst()->m_Width - 600)
				m_RandomPosition.x = (rand() % -500 - 100) + (int)m_Position.x;
			else
				m_RandomPosition.x = (rand() % 500 + 300) + (int)m_Position.x;

			m_LastMoveTime = 2.f;

			Vec2 C, D, Dir;
			C = m_Position;
			D = GameInfo->GetPlayerInfo()->m_Position;

			Dir = D - C;

			D3DXVec2Normalize(&Dir, &Dir);

			ObjMgr->AddObject(new EnemyDirBullet(Vec2(m_Position.x - 10, m_Position.y), Dir), "EnemyBullet");
		}
	}
	else {
		if (impellent < 2) {
			impellent += dt;
		}
		for (auto iter : ObjMgr->m_Objects) {
			if (iter->m_Tag == "Player")
				Enemy = iter->m_Position - m_Position;
		}
		D3DXVec2Normalize(&Dire, &Enemy);
		Delay += dt;
		if (Delay > 0.5f) {
			vrad += dt * 0.01f;
			Delay = 0.f;
			ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x + 15, m_Position.y - 20), m_Rotation - 240), "EnemyBullet");
			ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x + 15, m_Position.y + 20), m_Rotation - 120), "EnemyBullet");
		}
		float pi2 = D3DX_PI * 2;
		float diff = std::atan2f(Dire.y, Dire.x) - turnRadian;
		while (diff < -D3DX_PI) diff += pi2;
		while (diff >= D3DX_PI) diff -= pi2;

		if (abs(diff) < vrad)
			turnRadian += diff;
		else {
			turnRadian += (diff < 0 ? -vrad : vrad);
		}

		Dire.y = sin(turnRadian);
		Dire.x = cos(turnRadian);
		m_Rotation = std::atan2f(Dire.y, Dire.x);
		Translate(Dire.x * m_Speed * impellent * dt, Dire.y * m_Speed * impellent * dt);
	}
}
