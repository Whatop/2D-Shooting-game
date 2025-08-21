#include "stdafx.h"
#include "EliteEnemy1.h"
#include "EnemyDirBullet.h"
#include "Item.h"

EliteEnemy1::EliteEnemy1(Vec2 Pos)
{
	m_EliteEnemy1 = Sprite::Create(L"Painting/Enemy/EliteEnemy1.png");
	m_EliteEnemy1->SetParent(this);

	SetPosition(Pos);
	m_RandomPosition = Vec2((rand() % 100 + 400) + m_Position.x, (rand() % 360 + 73));
	m_Hp = 300;
	m_Rotation = D3DXToRadian(180);
	m_Speed = 450.f;
	m_LastMoveTime = 2.f;
	m_Layer = 2;
	SpawnMove = 0.f;
	ones = true;
	SetScale(0.75f, 0.75f);
	std::cout << "EliteEnemy1 »ý¼º" << std::endl;
	GameInfo->EnemyCount++;
	OneDamege = false;
	DamegeCoolTime = 0.f;
}

EliteEnemy1::~EliteEnemy1()
{
}

void EliteEnemy1::Update(float deltaTime, float Time)
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
			ObjMgr->CollisionCheak(this, "Bullet");
			ObjMgr->CollisionCheak(this, "ChargeBullet");
		}
		else {
			ObjMgr->CollisionCheak(this, "Bullet");
			ObjMgr->CollisionCheak(this, "ChargeBullet");
			if (ones) {
				m_RandomPosition = Vec2((rand() % 100 + 400) + m_Position.x, (rand() % 360 + 73));
				ones = false;
			}

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

void EliteEnemy1::Render()
{
	m_EliteEnemy1->Render();
}

void EliteEnemy1::OnCollision(Object* obj)
{
	if (obj->m_Tag == "Bullet") {
		m_Hp -= obj->m_Atk;
		float randx = (rand() % (int)m_Size.x * m_Scale.x) + m_Position.x - m_Size.x / 2 * m_Scale.x;
		float randy = (rand() % (int)m_Size.y * m_Scale.y) + m_Position.y - m_Size.y / 2 * m_Scale.y;
		obj->SetDestroy(true);
		ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Explosion/", 1, 9, 0.1f, Vec2(randx, randy)), "Effect");
		std::cout << obj->m_Atk << std::endl;

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

void EliteEnemy1::Move()
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

		Vec2 C, D,E,F,G,H,Dir1,Dir2,Dir3, Dir4, Dir5;
		C = m_Position;
		D = GameInfo->GetPlayerInfo()->m_Position;
		E = Vec2(GameInfo->GetPlayerInfo()->m_Position.x, GameInfo->GetPlayerInfo()->m_Position.y + 20);
		F = Vec2(GameInfo->GetPlayerInfo()->m_Position.x, GameInfo->GetPlayerInfo()->m_Position.y - 20);
		G = Vec2(GameInfo->GetPlayerInfo()->m_Position.x, GameInfo->GetPlayerInfo()->m_Position.y - 40);
		H = Vec2(GameInfo->GetPlayerInfo()->m_Position.x, GameInfo->GetPlayerInfo()->m_Position.y - 60);
		Dir1 = D - C;
		Dir2 = E - C;
		Dir3 = F - C;
		Dir4 = G - C;
		Dir5 = H - C;

		D3DXVec2Normalize(&Dir1, &Dir1);
		D3DXVec2Normalize(&Dir2, &Dir2);
		D3DXVec2Normalize(&Dir3, &Dir3);

		ObjMgr->AddObject(new EnemyDirBullet(Vec2(m_Position.x - 10, m_Position.y), Dir1), "EnemyBullet");
		ObjMgr->AddObject(new EnemyDirBullet(Vec2(m_Position.x - 10, m_Position.y), Dir2), "EnemyBullet");
		ObjMgr->AddObject(new EnemyDirBullet(Vec2(m_Position.x - 10, m_Position.y), Dir3), "EnemyBullet");
		ObjMgr->AddObject(new EnemyDirBullet(Vec2(m_Position.x - 10, m_Position.y), Dir4), "EnemyBullet");
		ObjMgr->AddObject(new EnemyDirBullet(Vec2(m_Position.x - 10, m_Position.y), Dir5), "EnemyBullet");
	}
}
