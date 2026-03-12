#include "stdafx.h"
#include "EffectMgr.h"

EffectMgr::EffectMgr(std::wstring filename, int start, int end, float speed, Vec2 Pos, float scalex, float scaley)
{
	// 애니메이션 생성
	Effect = new Animation();

	// 지정한 프레임 구간을 연속 프레임으로 등록
	Effect->AddContinueFrame(filename, start, end, D3DCOLOR_XRGB(0, 248, 0));

	// 애니메이션 초기화
	Effect->Init(speed, true);

	// 부모 오브젝트 설정
	Effect->SetParent(this);

	// 이펙트 위치/크기 설정
	Effect->m_Position = Pos;
	Effect->SetScale(scalex, scaley);

	Start = start;
	End = end;

	// 렌더 레이어 설정
	m_Layer = 3;
}

EffectMgr::~EffectMgr()
{
}

void EffectMgr::Update(float deltaTime, float time)
{
	// 점수씬에서는 반투명하게 표시
	if (GameInfo->isScoreScene) {
		Effect->A = 105;
	}

	// 마지막 프레임에 도달하면 오브젝트 제거
	if (Effect->m_CurrentFrame >= End - 1)
	{
		ObjMgr->RemoveObject(this);
	}

	// 일시정지 중이 아닐 때만 애니메이션 갱신
	if (!GameInfo->isPause) {
		Effect->Update(deltaTime, time);
	}
}

void EffectMgr::Render()
{
	// 이펙트 렌더링
	Effect->Render();
}

void EffectMgr::OnCollision(Object* other)
{
	// 충돌 처리 없음
}