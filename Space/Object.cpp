#include "stdafx.h"
#include "Object.h"

Object::Object() :
	m_Position(0.f, 0.f)           // 기본 위치는 원점
	, m_Rotation(0.f)              // 기본 회전 0
	, m_Scale(1.f, 1.f)            // 기본 스케일 1배
	, m_Destroy(false)             // 시작 시 파괴 상태 아님
	, m_Visible(true)              // 기본적으로 보임
	, m_Layer(0)                   // 기본 레이어 0
	, m_Tag("UnNamed")             // 기본 태그
	, m_RotationCenter(0.f, 0.f)   // 회전 중심 기본값
	, m_ScaleCenter(0.f, 0.f)      // 스케일 중심 기본값
	, m_Parent(nullptr)            // 부모 없음
	, m_Radius(0.f)                // 반지름 0
	, DestroyTime(0.f)             // 지연 파괴 누적 시간 0
	, m_Atk(0.f)                   // 공격력 기본값 0
{
	// 월드 행렬을 단위행렬로 초기화
	D3DXMatrixIdentity(&m_wMat);

	// 충돌 박스 초기화
	SetRect(&m_Collision, 0, 0, 0, 0);

	// 크기 초기화
	m_Size = Vec2(0.f, 0.f);
}

Object::~Object()
{
}

Matrix Object::GetMatrix()
{
	Vec2 RotCenter = Vec2(0.f, 0.f);
	Vec2 ScaleCenter = Vec2(0.f, 0.f);

	// UI가 아닌 일반 오브젝트는 카메라 위치를 고려해서 중심점 보정
	if (m_Tag != "UI")
	{
		RotCenter.x = -Camera::GetInst()->m_Position.x + m_RotationCenter.x;
		RotCenter.y = -Camera::GetInst()->m_Position.y + m_RotationCenter.y;

		ScaleCenter.x = -Camera::GetInst()->m_Position.x + m_ScaleCenter.x;
		ScaleCenter.y = -Camera::GetInst()->m_Position.y + m_ScaleCenter.y;
	}

	// 2D 변환 행렬 생성
	// 스케일 중심, 회전 중심, 스케일, 회전, 위치를 모두 반영
	D3DXMatrixTransformation2D(&m_wMat, &ScaleCenter, 0, &m_Scale, &RotCenter, m_Rotation, &m_Position);

	// 부모가 있으면 부모 행렬까지 곱해서 최종 월드 행렬 구성
	if (m_Parent)
		m_wMat *= m_Parent->GetMatrix();

	return m_wMat;
}

// 현재 위치에 상대적으로 이동
void Object::Translate(float x, float y)
{
	m_Position.x += x;
	m_Position.y += y;
}

// 스케일 설정
void Object::SetScale(float x, float y)
{
	m_Scale.x = x;
	m_Scale.y = y;
}

// Vec2로 위치 설정
void Object::SetPosition(Vec2 pos)
{
	m_Position = pos;
}

// x, y로 위치 설정
void Object::SetPosition(float x, float y)
{
	m_Position.x = x;
	m_Position.y = y;
}

// 회전값 누적
void Object::Rotate(float r)
{
	m_Rotation += r;
}

// 태그 설정
void Object::SetTag(const std::string tag)
{
	m_Tag = tag;
}

// 부모 오브젝트 설정
void Object::SetParent(Object* obj)
{
	m_Parent = obj;
}

// 일정 시간이 지나면 파괴 처리
void Object::DelayDestroy(Object* obj, float destroyTime)
{
	DestroyTime += dt;

	if (DestroyTime >= destroyTime)
	{
		SetDestroy(true);
	}
	else if (destroyTime <= 0)
	{
		SetDestroy(true);
	}
}

// 기본 Update는 비워 둠
// 자식 클래스에서 override해서 사용
void Object::Update(float deltaTime, float time)
{
}

// 기본 Render도 비워 둠
void Object::Render()
{
}

// 충돌 처리용 기본 가상 함수
void Object::OnCollision(Object* other)
{
}