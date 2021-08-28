#include "stdafx.h"
#include "EffectMgr.h"

EffectMgr::EffectMgr(std::wstring filename, int start, int end, float speed, Vec2 Pos,float scalex,float scaley)
{
	Effect = new Animation();
	Effect->AddContinueFrame(filename, start, end, D3DCOLOR_XRGB(0,248,0));
	Effect->Init(speed, true);
	Effect->SetParent(this);
	Effect->m_Position = Pos;
	Effect->SetScale(scalex, scaley);

	Start = start;
	End = end;
	m_Layer = 3;
}

EffectMgr::~EffectMgr()
{
}

void EffectMgr::Update(float deltaTime, float time)
{
	if (GameInfo->isScoreScene) {
		Effect->A = 105;
	}
	if (Effect->m_CurrentFrame >= End - 1)
	{
		ObjMgr->RemoveObject(this);
	}
	if (!GameInfo->isPause) {
		Effect->Update(deltaTime, time);
	}
}

void EffectMgr::Render()
{
	Effect->Render();
}

void EffectMgr::OnCollision(Object* other)
{
}
