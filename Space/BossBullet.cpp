#include "stdafx.h"
#include "BossBullet.h"

BossBullet::BossBullet(Vec2 spawnPos, Vec2 dir)
{
	m_BossBullet = new Animation();
	m_BossBullet->Init(0.2f, true);
	m_BossBullet->SetParent(this);
	m_BossBullet->AddContinueFrame(L"Painting/Boss/Bullet/Bullet", 0, 1, COLORKEY_GREEN);

	DestroyTime = 0.f;
	m_Speed = 500.f;
	SetScale(2, 2);
	SetPosition(spawnPos);
	Dire = dir;
	m_Layer = 2;
	m_Rotation = (std::atan2(dir.y, dir.x));
	m_Atk = 5.f * pow(1.5f, GameInfo->Stage - 1);;
	random = 0;
	
}

BossBullet::~BossBullet()
{
}

void BossBullet::Update(float deltaTime, float Time)
{
	if (!GameInfo->isPause) {
		SetScale(2 + DestroyTime, 2 + DestroyTime);

		DestroyTime += dt;
		Move();

		if (DestroyTime > 7) {
			ObjMgr->RemoveObject(this);
			ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Explosion/", 1, 9, 0.1f, m_Position, 1 * DestroyTime, 1 * DestroyTime), "Effect");
		}
		if (m_Position.y > 445 - m_Size.y / 2 ||
			m_Position.y < -110 + m_Size.y / 2)
		{
			// 위아래 반사
			Dire.y = -Dire.y;
			random = rand() % 4 == 0 ? 3 : 0;  // 25% 확률 플레이어 도탄
		}

		if (m_Position.x > Camera::GetInst()->m_Position.x + App::GetInst()->m_Width - m_Size.x / 2 ||
			m_Position.x < Camera::GetInst()->m_Position.x + m_Size.x / 2)
		{
			// 좌우 반사
			Dire.x = -Dire.x;
			random = rand() % 4 == 0 ? 3 : 0;  // 25% 확률 플레이어 도탄
		}


		m_Atk = 5.f * m_Scale.x * pow(1.5f, GameInfo->Stage - 1);
		m_BossBullet->Update(deltaTime, Time);
	}
}

void BossBullet::Render()
{
	m_BossBullet->Render();
}

void BossBullet::OnCollision(Object* obj)
{
}

void BossBullet::Move()
{
	if (random == 3) {
		// 플레이어 방향 재조정
		Dire = GameInfo->GetPlayerInfo()->m_Position - m_Position;
		D3DXVec2Normalize(&Dire, &Dire);
		std::cout << "도탄됨! (플레이어 향함)" << std::endl;

		random = 0; // 한 번 튄 뒤에는 다시 일반 모드
	}

	m_Rotation = atan2f(Dire.y, Dire.x);
	Translate(Dire.x * m_Speed * dt, Dire.y * m_Speed * dt);
}
