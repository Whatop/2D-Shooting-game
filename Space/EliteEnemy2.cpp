#include "stdafx.h"
#include "EliteEnemy2.h"
#include "EnemyRotationBullet.h"
#include "Item.h"

EliteEnemy2::EliteEnemy2(Vec2 Pos)
{
	m_EliteEnemy2 = Sprite::Create(L"Painting/Enemy/EliteEnemy2.png");
	m_EliteEnemy2->SetParent(this);

	SetPosition(Pos);
	m_RandomPosition = Vec2((rand() % 100 + 400) + m_Position.x, (rand() % 360 + 73));
	m_Hp = 300;
	m_Rotation = D3DXToRadian(180);
	m_Speed = 450.f;
	m_LastMoveTime = 2.f;
	MoveTime = 0;
	SpawnMove = 0.f;
	ones = true;
	m_Layer = 2;
	isAttack = false;
	SetScale(0.75f, 0.75f);
	std::cout << "EliteEnemy2 생성" << std::endl;
	GameInfo->EnemyCount++;
	OneDamege = false;
}

EliteEnemy2::~EliteEnemy2()
{
}

void EliteEnemy2::Update(float deltaTime, float Time)
{
	if (!GameInfo->isPause) {
		SpawnMove += dt;
		if (!OneDamege) {
			ObjMgr->CollisionCheak(this, "Boom");
		}
		else {
			DamegeCoolTime += dt;
			std::cout << DamegeCoolTime << std::endl;

			if (DamegeCoolTime > 4.f) {

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
			if (m_LastMoveTime >= 4)
				Move();
			if (m_Hp <= 0)
			{
				if ((rand() % 10) == 0)
					ObjMgr->AddObject(new Item(m_Position), "Heal");
				ObjMgr->RemoveObject(this);
				ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Big/", 1, 9, 0.1f, m_Position), "Effect");
				GameInfo->EnemyCount--;
				GameInfo->MaxScore += 300;
				GameInfo->KillScore += 300;
				GameInfo->SpawnCoin(m_Position);
			}
			if (GameInfo->AutoCamera && !GameInfo->CameraStop) {
				m_Position.x += 100 * dt;
			}
		}
	}
}

void EliteEnemy2::Render()
{
	m_EliteEnemy2->Render();
}

void EliteEnemy2::OnCollision(Object* obj)
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
		float randx = (rand() % (int)m_Size.x * m_Scale.x) + m_Position.x - m_Size.x / 2 * m_Scale.x;
		float randy = (rand() % (int)m_Size.y * m_Scale.y) + m_Position.y - m_Size.y / 2 * m_Scale.y;
		ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Big/", 1, 9, 0.1f, Vec2(randx, randy)), "Effect");

		OneDamege = true;
	}
}

void EliteEnemy2::Move()
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
		// 생성자 등 초기화 지점
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

		Attack();

	}
	
}

void EliteEnemy2::Attack()
{
	if (isAttack) {
		ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 240), "EnemyBullet");
		ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 210), "EnemyBullet");
		ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 180), "EnemyBullet");
		ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 150), "EnemyBullet");
		ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 120), "EnemyBullet");

		isAttack = false;
	}
	else{
		ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 220), "EnemyBullet");
		ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 190), "EnemyBullet");
		ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 160), "EnemyBullet");
		ObjMgr->AddObject(new EnemyRotationBullet(Vec2(m_Position.x - 10, m_Position.y), 130), "EnemyBullet");
		isAttack = true;
	}
}
