#include "stdafx.h"
#include "Boss.h"
#include "BossBullet.h"
#include "Missile.h"
#include "Box.h"
#include "Missile.h"

Boss::Boss()
{
	Propeller = new Animation();
	Propeller->Init(0.1f, true);
	Propeller->SetParent(this);
	Propeller->AddContinueFrame(L"Painting/Boss/Propeller/Propeller", 0, 5);

	PilotAttack = new Animation();
	PilotAttack->Init(1, false);
	PilotAttack->SetParent(this);
	PilotAttack->AddContinueFrame(L"Painting/Boss/Attack/Attack", 0, 4);

	m_Boss = Sprite::Create(L"Painting/Boss/All.png"); // 296 x 186
	m_Boss->SetParent(this);

	m_ColBox = Sprite::Create(L"Painting/Boss/ColBox/ColBox.png"); // 296 x 186
	m_ColBox->SetParent(this);

	BossBody = Sprite::Create(L"Painting/Boss/Body.png"); // 222 x 138
	BossTail = Sprite::Create(L"Painting/Boss/Tail.png"); // 74 x 92
	BossBehind = Sprite::Create(L"Painting/Boss/Exit.png"); // 66 x 83

	DestroyBody = Sprite::Create(L"Painting/Boss/DestroyBody.png");// 213 x 137
	DestroyTail = Sprite::Create(L"Painting/Boss/DestroyTail.png");
	ColBoxTop = Sprite::Create(L"Painting/Boss/ColBox/HitBox.png");

	SetScale(2, 2);
	SetPosition(1400, 480 / 2);

	BossBody->SetScale(2, 2);
	BossTail->SetScale(2, 2);
	BossBehind->SetScale(2, 2);
	DestroyBody->SetScale(2, 2);
	DestroyTail->SetScale(2, 2);
	ColBoxTop->SetScale(2, 2);

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
	isMove = true;
	//BossTail->m_Visible = false;
	//BossBehind->m_Visible = false;
	DestroyBody->m_Visible = false;
	DestroyTail->m_Visible = false;
	ColBoxTop->m_Visible = false;
	m_MaxHp = 2500.f;
	m_Hp = m_MaxHp;
	m_Speed = 200.f;
	m_RandomPosition = Vec2((rand() % 100 + 400) + m_Position.x, (rand() % 1080));
	m_MoveWaitingTime = 3.f;
	m_LastMoveTime = 3.f;
	MS_Num = 0;
	MoveNum = 0;

	TailHp = 450.f;
	BodyHp = 500.f;
	TopHp = 200.f;
	DestroyTime = 1.f;
	isDestroyTop = false;
	isDestroyTail = false;
	isBoom = false;
}

Boss::~Boss()
{
}

void Boss::Update(float deltaTime, float Time)
{
	m_LastMoveTime += dt;
	MS_DelayTime += dt;
	DelayTime += dt;
	isLeft = false;
	isRight = false;
	isUp = false;
	isDown = false;
	ObjMgr->CollisionCheak(this, "Wall");
	ObjMgr->CollisionCheak(this, "Bullet");
	Propeller->Update(deltaTime, Time);

	if (!GameInfo->m_DebugMode) {
		m_ColBox->m_Visible = false;
		ColBox[LEFT]->m_Visible = false;
		ColBox[RIGHT]->m_Visible = false;
		ColBox[UP]->m_Visible = false;
		ColBox[DOWN]->m_Visible = false;
		ColBoxTop->m_Visible = false;
		//ColBox[HIT]->m_Visible = false;
	}
	else {
		m_ColBox->m_Visible = true;
		ColBox[LEFT]->m_Visible = true;
		ColBox[RIGHT]->m_Visible = true;
		ColBox[UP]->m_Visible = true;
		ColBox[DOWN]->m_Visible = true;
		ColBoxTop->m_Visible = true;
		//ColBox[HIT]->m_Visible = true;
	}
	if (m_LastMoveTime >= m_MoveWaitingTime)
	{
		if (isMove)
			Move();


	}
	if (!isDestroyTop) {
		Fire();
	}

	State();

	if (MS_DelayTime > 5) {
		MS_RpmTime += dt;
		if (MS_RpmTime > 1) {
			MS_Num++;
			SpawnMissile();
			if (MS_Num >= 5) {
				MS_DelayTime = 0;
				MS_Num = 0;
			}
			MS_RpmTime = 0;
		}
	}
	if (GameInfo->AutoCamera && !GameInfo->CameraStop)
		m_Position.x += 100 * dt;
	//if ((rand() % 10) == 0) 아이템코드
	//	ObjMgr->AddObject(new Item(m_Position), "ITEM");
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
	ColBoxTop->Render();

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
		if (!isDestroyTail) {
			if (IntersectRect(&rc, &BossTail->m_Collision, &obj->m_Collision)) {
				m_Hp -= 101;
				TailHp -= 101;
				float randx = (rand() % (int)BossTail->m_Size.x * m_Scale.x) + BossTail->m_Position.x - BossTail->m_Size.x / 2 * m_Scale.x;
				float randy = (rand() % (int)BossTail->m_Size.y * m_Scale.y) + BossTail->m_Position.y - BossTail->m_Size.y / 2 * m_Scale.y;
				obj->SetDestroy(true);
				ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Explosion/", 1, 9, 0.1f, Vec2(randx, randy)), "Effect");
			}
		}
		if (!isDestroyTop) {
			if (IntersectRect(&rc, &ColBoxTop->m_Collision, &obj->m_Collision)) {
				m_Hp -= 110;
				TopHp -= 110;
				float randx = (rand() % (int)ColBoxTop->m_Size.x * m_Scale.x) + ColBoxTop->m_Position.x - ColBoxTop->m_Size.x / 2 * m_Scale.x;
				float randy = (rand() % (int)ColBoxTop->m_Size.y * m_Scale.y) + ColBoxTop->m_Position.y - ColBoxTop->m_Size.y / 2 * m_Scale.y;
				obj->SetDestroy(true);
				ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Explosion/", 1, 9, 0.1f, Vec2(randx, randy)), "Effect");
			}
		}
		if (isDestroyTop && isDestroyTail && !isDestroyBody) {
			if (IntersectRect(&rc, &BossBody->m_Collision, &obj->m_Collision)) {
				m_Hp -= 110;
				BodyHp -= 110;
				float randx = (rand() % (int)m_Size.x * m_Scale.x) + m_Position.x - m_Size.x / 2 * m_Scale.x;
				float randy = (rand() % (int)m_Size.y * m_Scale.y) + m_Position.y - m_Size.y / 2 * m_Scale.y;
				obj->SetDestroy(true);
				ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Explosion/", 1, 9, 0.1f, Vec2(randx, randy)), "Effect");
			}
		}
		if (isDestroyBody) {
			if (IntersectRect(&rc, &BossBody->m_Collision, &obj->m_Collision)) {
				m_Hp -= 110;
				float randx = (rand() % (int)m_Size.x * m_Scale.x) + m_Position.x - m_Size.x / 2 * m_Scale.x;
				float randy = (rand() % (int)m_Size.y * m_Scale.y) + m_Position.y - m_Size.y / 2 * m_Scale.y;
				obj->SetDestroy(true);
				ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Explosion/", 1, 9, 0.1f, Vec2(randx, randy)), "Effect");
			}
		}
	}
}

void Boss::Move()
{
	if (MoveTime > 1.4f) {
		if (MoveNum == 0) {
			m_RandomPosition = Vec2(m_Position.x + 100, 150);
		}
		if (MoveNum == 1)
			m_RandomPosition = Vec2(m_Position.x + 600, 480);
		if (MoveNum == 2)
			m_RandomPosition = Vec2(m_Position.x + 100, 150);
		if (MoveNum == 3) {
			m_RandomPosition = Vec2(m_Position.x + 800, 360);
			MoveNum = 0;
		}
		std::cout << MoveNum << std::endl;
		MoveNum++;
		m_LastMoveTime = 0.f;
		MoveTime = 0;
	}
	//좌표 몇개를 만들어주자.
	Vec2 A, B, Dire;

	A = m_Position;
	B = m_RandomPosition;

	Dire = B - A;

	D3DXVec2Normalize(&Dire, &Dire);

	if (!isRight && Dire.x > 0 || !isLeft && Dire.x < 0)
		Translate(Dire.x * m_Speed * dt, 0);
	if (!isUp && Dire.y < 0 || !isDown && Dire.y > 0)
		Translate(0, Dire.y * m_Speed * dt);
	MoveTime += dt;

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
	BossBody->SetPosition(m_Position.x + 75, m_Position.y + 21);
	BossTail->SetPosition(BossBody->m_Position.x - 222 - 75, BossBody->m_Position.y - 138 / 2 - 92 / 2);
	BossBehind->SetPosition(BossBody->m_Position.x - 210, BossBody->m_Position.y + 138 / 2 - 16);

	DestroyBody->SetPosition(m_Position.x + 74 - 9, m_Position.y + 20 - 1);
	DestroyTail->SetPosition(BossBody->m_Position.x - 222 - 74, BossBody->m_Position.y - 138 / 2 - 92 / 2);

	ColBox[LEFT]->SetPosition(m_Position.x - m_Size.x / 2 * m_Scale.x, m_Position.y);
	ColBox[RIGHT]->SetPosition(m_Position.x + m_Size.x / 2 * m_Scale.x, m_Position.y);
	ColBox[UP]->SetPosition(m_Position.x, m_Position.y - m_Size.y / 2 * m_Scale.y);
	ColBox[DOWN]->SetPosition(m_Position.x, m_Position.y + m_Size.y / 2 * m_Scale.y);
	ColBoxTop->SetPosition(BossBody->m_Position.x + 120, BossBody->m_Position.y + 138 / 2 - 26);

	if (TailHp <= 0) {
		m_Speed = m_Speed / 1.5f;
		m_Hp -= 300.f;
		isDestroyTail = true;
		DestroyTail->m_Visible = true;
		TailHp = 99999.f;
		for (int i = 0; i < 10; i++) {
			float randx = (rand() % (int)BossTail->m_Size.x * m_Scale.x) + BossTail->m_Position.x - BossTail->m_Size.x / 2 * m_Scale.x;
			float randy = (rand() % (int)BossTail->m_Size.y * m_Scale.y) + BossTail->m_Position.y - BossTail->m_Size.y / 2 * m_Scale.y;
			ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Big/", 1, 9, 0.1f, Vec2(randx, randy)), "Effect");
		}
	}

	if (TopHp <= 0) {
		m_Hp -= 300.f;
		isDestroyTop= true;
		TopHp = 99999.f;
		for (int i = 0; i < 10; i++) {
			float randx = (rand() % (int)ColBoxTop->m_Size.x * m_Scale.x) + ColBoxTop->m_Position.x - ColBoxTop->m_Size.x / 2 * m_Scale.x;
			float randy = (rand() % (int)ColBoxTop->m_Size.y * m_Scale.y) + ColBoxTop->m_Position.y - ColBoxTop->m_Size.y / 2 * m_Scale.y;
			ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Big/", 1, 9, 0.1f, Vec2(randx, randy)), "Effect");
		}
		PilotAttack->m_CurrentFrame = 4;
	}
	if (isDestroyTop && isDestroyTail && !isDestroyBody) {
		if (BodyHp <= 0) {
			m_Hp -= 500.f;
			for (int i = 0; i < 10; i++) {
				float randx = (rand() % (int)BossBody->m_Size.x*m_Scale.x) + BossBody->m_Position.x - BossBody->m_Size.x / 2 * m_Scale.x;
				float randy = (rand() % (int)BossBody->m_Size.y*m_Scale.y) + BossBody->m_Position.y - BossBody->m_Size.y / 2 * m_Scale.y;
				ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Big/", 1, 9, 0.1f, Vec2(randx, randy)), "Effect");
			}
			DestroyBody->m_Visible = true;
			isDestroyBody = true;
			isMove = false;
		}
	}
	if (m_Hp < 0) {
		DestroyTime += dt;
		if (!isDown)
			m_Position.y += 100 * DestroyTime * dt;

		if (isDown && !isBoom) {
			ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Explosion2/", 1, 9, 0.2f, Vec2(m_Position.x-100, m_Position.y - m_Size.y / 2),2,2), "Effect");
			ObjMgr->AddObject(new EffectMgr(L"Painting/Effect/Explosion2/", 1, 9, 0.2f, Vec2(m_Position.x+100, m_Position.y - m_Size.y / 2),2,2), "Effect");
			isBoom = true;
			GameInfo->CameraStop = true;
			//DelayDestroy(this, 2);
		}
	}
}

void Boss::SpawnObstacle()
{
	ObjMgr->AddObject(new Box, "Box");
}

void Boss::SpawnMissile()
{
	ObjMgr->AddObject(new Missile(Vec2(m_Position.x, m_Position.y + 180)), "Missile");
}

void Boss::DestroyEffect()
{
	
}
