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

	m_ColBox = Sprite::Create(L"Painting/Boss/ColBox/ColBox.png", COLORKEY_GREEN); // 296 x 186
	m_ColBox->SetParent(this);

	BossBody = Sprite::Create(L"Painting/Boss/Body.png", COLORKEY_GREEN); // 222 x 138
	BossTail = Sprite::Create(L"Painting/Boss/Tail.png", COLORKEY_GREEN); // 74 x 92
	BossBehind = Sprite::Create(L"Painting/Boss/Exit.png", COLORKEY_GREEN); // 66 x 83

	DestroyBody = Sprite::Create(L"Painting/Boss/DestroyBody.png", COLORKEY_GREEN);// 213 x 137
	DestroyTail = Sprite::Create(L"Painting/Boss/DestroyTail.png", COLORKEY_GREEN);
	DestroyTop = Sprite::Create(L"Painting/Boss/DestroyTop.png", COLORKEY_GREEN);

	SetScale(2, 2);
	SetPosition(1400, 480 / 2);

	BossBody->SetScale(2, 2);
	BossTail->SetScale(2, 2);
	BossBehind->SetScale(2, 2);
	DestroyBody->SetScale(2, 2);
	DestroyTail->SetScale(2, 2);
	DestroyTop->SetScale(2, 2);

	BossBody->SetPosition(m_Position.x + 74, m_Position.y + 20);
	BossTail->SetPosition(BossBody->m_Position.x - 222 - 74, BossBody->m_Position.y - 138 / 2 - 92 / 2);
	BossBehind->SetPosition(BossBody->m_Position.x - 210, BossBody->m_Position.y + 138 / 2 - 16);

	LEFT = 0;
	RIGHT = 1;
	UP = 2;
	DOWN = 3;
	HIT = 4;

	ColBox[LEFT] = Sprite::Create(L"Painting/Boss/ColBox/Height.png");
	ColBox[RIGHT] = Sprite::Create(L"Painting/Boss/ColBox/Height.png");
	ColBox[UP] = Sprite::Create(L"Painting/Boss/ColBox/Width.png");
	ColBox[DOWN] = Sprite::Create(L"Painting/Boss/ColBox/Width.png");
	//ColBox[HIT] = Sprite::Create(L"Painting/Boss/ColBox/HitBox.png");
	ColBox[LEFT]->SetScale(1.5f, 1.5f);
	ColBox[RIGHT]->SetScale(1.5f, 1.5f);
	ColBox[UP]->SetScale(1.5f, 1.5f);
	ColBox[DOWN]->SetScale(1.5f, 1.5f);

	ColBox[LEFT]->m_Visible = false;
	ColBox[RIGHT]->m_Visible = false;
	ColBox[UP]->m_Visible = false;
	ColBox[DOWN]->m_Visible = false;
	//ColBox[HIT]->m_Visible = false;

	Count = 0;
	DelayTime = 1.f;
	ShootTime = 0.f;
	isShoot = false;
	isDire = false;
	//BossTail->m_Visible = false;
	//BossBehind->m_Visible = false;
	DestroyBody->m_Visible = false;
	DestroyTail->m_Visible = false;
	DestroyTop->m_Visible = false;

	m_MaxHp = 5000.f;
	m_Hp = m_MaxHp;
}

Boss::~Boss()
{
}

void Boss::Update(float deltaTime, float Time)
{
	ObjMgr->CollisionCheak(this, "Bullet");
	DelayTime += dt;
	Propeller->Update(deltaTime, Time);

	if (!GameInfo->m_DebugMode) {
		m_ColBox->m_Visible = false;
		ColBox[LEFT]->m_Visible = false;
		ColBox[RIGHT]->m_Visible = false;
		ColBox[UP]->m_Visible = false;
		ColBox[DOWN]->m_Visible = false;
		//ColBox[HIT]->m_Visible = false;
	}
	else {
		m_ColBox->m_Visible = true;
		ColBox[LEFT]->m_Visible = true;
		ColBox[RIGHT]->m_Visible = true;
		ColBox[UP]->m_Visible = true;
		ColBox[DOWN]->m_Visible = true;
		//ColBox[HIT]->m_Visible = true;
	}
	if(isMove)
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
	DestroyTail->Render();
	DestroyTop->Render();

	m_ColBox->Render();
	ColBox[LEFT]->Render();
	ColBox[RIGHT]->Render();
	ColBox[UP]->Render();
	ColBox[DOWN]->Render();
	//ColBox[HIT]->Render();
}

void Boss::OnCollision(Object* obj)
{
	if (obj->m_Tag == "Wall") {
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
	if (obj->m_Tag == "Bullet") {
		RECT rc;
		if (IntersectRect(&rc, &DestroyTail->m_Collision, &obj->m_Collision)) {
			m_Hp -= 10;
			obj->m_Destroy = true;
		}
	}
}

void Boss::Move()
{
	if (GameInfo->AutoCamera)
		m_Position.x += 100 * dt;

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
		if (!isShoot)
			ObjMgr->AddObject(new BossBullet(TempPos, Dire), "EnemyBullet");

		isShoot = true;

		if (ShootTime > 0.5f) {

			if (Count < 3) {
				PilotAttack->NextFrame();
				isShoot = false;
				isDire = false;
				ShootTime = 0;
				DelayTime = 0;
				Count++;
			}
			else if (Count > 2) {
				PilotAttack->BackFrame();
				isShoot = false;
				isDire = false;
				ShootTime = 0;
				DelayTime = 0;
				Count++;
			}
			if (Count > 5) {
				isShoot = false;
				isDire = false;
				ShootTime = 0;
				DelayTime = 0;
				Count = 0;
			}
		}
	}
}

void Boss::State()
{
	GameInfo->BossHpUpdate(m_MaxHp, m_Hp);
	BossBody->SetPosition(m_Position.x + 74, m_Position.y + 20);
	BossTail->SetPosition(BossBody->m_Position.x - 222 - 74, BossBody->m_Position.y - 138 / 2 - 92 / 2);
	BossBehind->SetPosition(BossBody->m_Position.x - 210, BossBody->m_Position.y + 138 / 2 - 16);

	DestroyBody->SetPosition(m_Position.x + 74 - 9, m_Position.y + 20 - 1);
	DestroyTail->SetPosition(BossBody->m_Position.x - 222 - 74, BossBody->m_Position.y - 138 / 2 - 92 / 2);
	DestroyTop->SetPosition(BossBody->m_Position.x - 210, BossBody->m_Position.y + 138 / 2 - 16);

	DestroyTop->SetPosition(BossBody->m_Position.x - 210, BossBody->m_Position.y + 138 / 2 - 16);
	DestroyTop->SetPosition(BossBody->m_Position.x - 210, BossBody->m_Position.y + 138 / 2 - 16);
	DestroyTop->SetPosition(BossBody->m_Position.x - 210, BossBody->m_Position.y + 138 / 2 - 16);
	DestroyTop->SetPosition(BossBody->m_Position.x - 210, BossBody->m_Position.y + 138 / 2 - 16);

	ColBox[LEFT]->SetPosition(m_Position.x - m_Size.x / 2 * m_Scale.x, m_Position.y);
	ColBox[RIGHT]->SetPosition(m_Position.x + m_Size.x / 2 * m_Scale.x, m_Position.y);
	ColBox[UP]->SetPosition(m_Position.x, m_Position.y - m_Size.y / 2 * m_Scale.y);
	ColBox[DOWN]->SetPosition(m_Position.x, m_Position.y + m_Size.y / 2 * m_Scale.y);

	DestroyBody->m_Visible = false;
	DestroyTail->m_Visible = true;
	DestroyTop->m_Visible = false;
}

void Boss::SpawnObstacle()
{
}

void Boss::SpawnMissile()
{
}
