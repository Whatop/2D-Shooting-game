#include "stdafx.h"
#include "Boss.h"
#include "BossBullet.h"
#include "Missile.h"

Boss::Boss()
{
	Propeller = new Animation();
	Propeller->Init(0.1f, true);
	Propeller->SetParent(this);
	Propeller->AddContinueFrame(L"Painting/Boss/Propeller/Propeller", 0, 5, COLORKEY_GREEN);
	
	PilotAttack = new Animation();
	PilotAttack->Init(1, false);
	PilotAttack->SetParent(this);
	PilotAttack->AddContinueFrame(L"Painting/Boss/Attack/Attack", 0, 3, COLORKEY_GREEN);

	m_Boss = Sprite::Create(L"Painting/Boss/All.png", COLORKEY_GREEN); // 296 x 186
	m_Boss->SetParent(this);

	BossBody = Sprite::Create(L"Painting/Boss/Body.png", COLORKEY_GREEN); // 222 x 138
	BossTail = Sprite::Create(L"Painting/Boss/Tail.png", COLORKEY_GREEN); // 74 x 92
	BossBehind = Sprite::Create(L"Painting/Boss/Exit.png", COLORKEY_GREEN); // 66 x 83

	DestroyBody = Sprite::Create(L"Painting/Boss/DestroyBody.png", COLORKEY_GREEN);
	DestroyTail = Sprite::Create(L"Painting/Boss/DestroyTail.png", COLORKEY_GREEN);
	DestroyTop = Sprite::Create(L"Painting/Boss/DestroyTop.png", COLORKEY_GREEN);

	SetScale(2, 2);
	SetPosition(1000, 480 / 2);
	BossBody->SetScale(2, 2);
	BossTail->SetScale(2, 2);
	BossBehind->SetScale(2, 2);

	BossBody->SetPosition(m_Position.x+74, m_Position.y+20);
	BossTail->SetPosition(BossBody->m_Position.x-222-74, BossBody->m_Position.y-138/2-92/2);
	BossBehind->SetPosition(BossBody->m_Position.x-210, BossBody->m_Position.y+ 138 / 2 - 16);

	DelayTime = 1.f;
	ShootTime = 0.f;
	isShoot = false;
	//BossTail->m_Visible = false;
	//BossBehind->m_Visible = false;
	DestroyBody->m_Visible = false;
	DestroyTail->m_Visible = false;
	DestroyTop->m_Visible = false;
}			 

Boss::~Boss()
{
}

void Boss::Update(float deltaTime, float Time)
{
	DelayTime += dt;
	Propeller->Update(deltaTime, Time);
	Move();
	Fire();
	State();
}

void Boss::Render()
{
	m_Boss->Render();
	BossBody->Render();
	BossTail->Render();
	BossBehind->Render();

	Propeller->Render();
	PilotAttack->Render();

	DestroyBody->Render();
	DestroyTail-> Render();
	DestroyTop->Render();
}

void Boss::OnCollision(Object* obj)
{
}

void Boss::Move()
{
	if (GameInfo->AutoCamera)
		m_Position.x += 100 *dt;
}

void Boss::Fire()
{

	Vec2 TempPos;
	float TempRotation;
	if (PilotAttack->m_CurrentFrame == 0) {
	//	TempPos = Vec2()
		TempRotation = D3DXToRadian(0);
		TempPos = Vec2(m_Position.x + 210, m_Position.y + 115);
	}
	else if (PilotAttack->m_CurrentFrame == 1) {
		TempRotation = D3DXToRadian(20);
		TempPos = Vec2(m_Position.x + 235, m_Position.y + 105);
	}
	else if (PilotAttack->m_CurrentFrame == 2) {
		TempRotation = D3DXToRadian(45);
		TempPos = Vec2(m_Position.x + 250, m_Position.y + 95);
	}
	else {
		TempRotation = D3DXToRadian(75);
		TempPos = Vec2(m_Position.x + 260, m_Position.y + 85);
	}


	Dire.y = cos(TempRotation);
	Dire.x = sin(TempRotation);
	if (DelayTime > 0.5f) {
		ShootTime += dt;
		if(!isShoot)
		ObjMgr->AddObject(new BossBullet(TempPos, Dire), "EnemyBullet");

		isShoot = true;
		if (ShootTime > 0.5f) {

			PilotAttack->NextFrame();
			isShoot = false;
			ShootTime = 0;
			DelayTime = 0;
		}
	}
}

void Boss::State()
{
	
}

void Boss::SpawnObstacle()
{
}

void Boss::SpawnMissile()
{
}
