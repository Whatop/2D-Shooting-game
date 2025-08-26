#include "stdafx.h"
#include "Enemy1.h"
#include "EnemyDirBullet.h"
#include "Item.h"

Enemy1::Enemy1(Vec2 Pos)
{
	m_Enemy1 = Sprite::Create(L"Painting/Enemy/Enemy1.png");
	m_Enemy1->SetParent(this);

	SetPosition(Pos);
	m_Hp = 100 * pow(1.5f, GameInfo->Stage - 1);
	m_Rotation = D3DXToRadian(180);
	m_Speed = 450.f;
	m_LastMoveTime = 2.f;
	SpawnMove = 0.f;
	m_Layer = 2;
	std::cout << "Enemy1 »ý¼º" << std::endl;
	GameInfo->EnemyCount++;
	ones = true;
	SetScale(0.85f, 0.85f);
	OneDamege = false;
}

Enemy1::~Enemy1()
{
}

void Enemy1::Update(float deltaTime, float Time)
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
				//73 433
				m_RandomPosition = Vec2((rand() % 100 + 400) + m_Position.x, (rand() % 360 + 73));
				ones = false;
			}
			ObjMgr->CollisionCheak(this, "Bullet");
			ObjMgr->CollisionCheak(this, "ChargeBullet");
			m_LastMoveTime += dt;
			if (m_LastMoveTime >= 4/ pow(1.5f, GameInfo->Stage - 1))
				Move();
			if (m_Hp <= 0)
			{
				if ((rand() % 30) == 0)
					ObjMgr->AddObject(new Item(m_Position), "Heal");
				ObjMgr->RemoveObject(this);
				ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Big/", 1, 9, 0.1f, m_Position), "Effect");
				GameInfo->EnemyCount--;
				GameInfo->MaxScore += 100;
				GameInfo->KillScore += 100;
				GameInfo->SpawnCoin(m_Position);

				SoundMgr* effect = new SoundMgr("Sound/snd_bomb.wav", false);
				effect->play();
				effect->volumeSetting(0.02f);
			}
			if (GameInfo->AutoCamera && !GameInfo->CameraStop) {
				m_Position.x += 100 * dt;
			}
		}
	}
}

void Enemy1::Render()
{
	m_Enemy1->Render();
}

void Enemy1::OnCollision(Object* obj)
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

void Enemy1::Move()
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

		Vec2 C, D, Dir;
		C = m_Position;
		D = GameInfo->GetPlayerInfo()->m_Position;

		Dir = D - C;

		D3DXVec2Normalize(&Dir, &Dir);

		ObjMgr->AddObject(new EnemyDirBullet(Vec2(m_Position.x - 10, m_Position.y), Dir), "EnemyBullet");
	}
}
