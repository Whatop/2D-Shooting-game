#include "stdafx.h"
#include "Boom.h"

Boom::Boom()
{
	m_Boom = new Animation();
	m_Boom->Init(0.1f, true);
	m_Boom->SetParent(this);
	m_Boom->AddContinueFrame(L"Painting/Effect/Big/", 1, 7, D3DCOLOR_XRGB(0, 248, 0));
	
	m_ColBox = Sprite::Create(L"Painting/Effect/Big/6.png", D3DCOLOR_XRGB(0, 248, 0));
	m_ColBox->SetParent(this);
	m_ColBox->m_Visible = false;

	SetPosition(GetPlayer->m_Position);

	ScaleUp = 0.1f;
	m_Boom->A = 100;
	m_Atk = 100.f;

	GameInfo->HV_Boom--;
	SoundMgr* effect = new SoundMgr("Sound/buffe.wav", false);
	effect->play();
	effect->volumeSetting(0.1f);

}

Boom::~Boom()
{
}

void Boom::Update(float deltaTime, float Time)
{

	if (!GameInfo->isPause) {
		DelayDestroy(this,3.f);
			if (ScaleUp < 10)
				ScaleUp += 15 * dt;

		ObjMgr->CollisionCheck(this, "EnemyBullet");
		ObjMgr->CollisionCheck(this, "Missile");
		if (m_Boom->m_CurrentFrame < 5) {
			m_Boom->Update(deltaTime, Time);
		}
		else {
			m_Boom->A -= 10;
			ScaleUp += 30 * dt;
		}

		m_Position.x += 100 * dt;
		SetScale(1.f * ScaleUp, 1.f * ScaleUp);
	}
	
}

void Boom::Render()
{
	m_ColBox->Render();
	m_Boom->Render();
}

void Boom::OnCollision(Object* obj)
{
	if (obj->m_Tag == "Missile") {
		obj->SetDestroy(true);
	}
	if (obj->m_Tag == "EnemyBullet") {
		obj->SetDestroy(true);
	}
}
