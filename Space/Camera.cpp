#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
{
	// 생성 시 기본 상태 초기화
	Init();
}

Camera::~Camera()
{
}

void Camera::Init()
{
	m_Rotation = 0.f;              // 시작 회전값
	m_Position = Vec2(0.f, -180.f); // 시작 위치
	m_Scale = Vec2(1.f, 1.f);      // 기본 배율 1
	Follow(nullptr);               // 현재는 아무 오브젝트도 추적하지 않음

	m_MinMapSize = Vec2(960.f, 0.f);      // Stage1 기준 최소 맵 범위
	m_MaxMapSize = Vec2(300000.f, 0.f);   // Stage1 기준 최대 맵 범위

	isVibration = false; // 시작 시 흔들림 없음
	ShakeTimeX = 4.f;    // 수정: int처럼 쓰던 값을 float로 명확히 표기
	ShakeTimeY = 4.f;    // 수정: 시간 변수이므로 float 표기가 더 자연스러움
	XShakePosition = 0.f;
}

void Camera::Translate()
{
	// 현재 비어 있음
	// 나중에 카메라 수동 이동 기능이 필요하면 여기서 처리 가능
}

void Camera::Follow(Object* obj)
{
	// 추적 대상이 있을 때만 카메라 위치를 대상 중심으로 이동
	if (obj != nullptr)
	{
		m_Position.x = obj->m_Position.x - App::GetInst()->m_Width / 2;
		m_Position.y = obj->m_Position.y - App::GetInst()->m_Height / 2;
	}
}

void Camera::Side_Scroll(Object* obj, float fixed_value, bool Auto)
{
	if (obj != nullptr)
	{
		if (!GameInfo->CameraStop)
		{
			// 수동 추적 모드
			if (!Auto)
			{
				// 수정 설명:
				// 현재 오브젝트가 맵 범위 안에 있을 때만 카메라 X를 따라감
				if (m_MinMapSize.x <= obj->m_Position.x && m_MaxMapSize.x >= obj->m_Position.x)
					m_Position.x = obj->m_Position.x - App::GetInst()->m_Width / 2;
			}
			// 자동 스크롤 모드
			else
			{
				// 수정 설명:
				// 현재 카메라 위치가 최대 맵 범위를 넘지 않을 때만 자동 이동
				if (m_MaxMapSize.x >= m_Position.x)
				{
					m_Position.x += 100 * dt;
				}
			}

			// y는 고정값을 기준으로 스크롤
			m_Position.y = fixed_value - App::GetInst()->m_Height / 2;
		}
	}
}

void Camera::Update(float deltaTime, float time)
{
	// 회전값이 너무 커지면 다시 0으로 순환
	if (m_Rotation >= 360.f)
		m_Rotation = 0.f;

	// 흔들림 시간 누적
	ShakeTimeX += dt;
	ShakeTimeY += dt;

	// Y축 흔들림
	if (isVibration && ShakeTimeY < 6.f)
	{
		m_Position.y +=
			(sin(2.0f * 3.14159f * ShakeTimeY * 4) * 3.5f +
				sin(2.0f * 3.14159f * ShakeTimeY * 8 + 0.2f) * 3.4f +
				sin(2.0f * 3.14159f * ShakeTimeY * 16 + 0.5f) * 3.3f) * (5 - ShakeTimeY) / 5;
	}

	// X축 흔들림
	if (isVibration && ShakeTimeX < 0.5f)
	{
		m_Position.x +=
			(sin(2.0f * 3.14159f * ShakeTimeX * 4) * 2.3f +
				sin(2.0f * 3.14159f * ShakeTimeX * 8 + 0.2f) * 2.2f +
				sin(2.0f * 3.14159f * ShakeTimeX * 16 + 0.5f) * 2.1f) * (5 - ShakeTimeX) / 5;
	}

	/*
	수정 전 메모:
	폭탄 터질 때 X축 흔들림을 추가로 넣을까 고민한 흔적처럼 보임.
	현재는 위 로직에 이미 X축 흔들림이 있으므로 따로 쓰지 않아도 됨.
	*/
}

void Camera::Render()
{
	// 카메라 회전 행렬 생성
	D3DXMatrixRotationZ(&mRot, D3DXToRadian(m_Rotation));

	// 카메라 위치만큼 월드를 반대로 이동
	// 즉, 카메라가 오른쪽으로 가면 월드는 왼쪽으로 움직이는 효과
	D3DXMatrixTranslation(&mTrans, -m_Position.x, -m_Position.y, 1.f);

	// 카메라 확대/축소
	D3DXMatrixScaling(&mScale, m_Scale.x, m_Scale.y, 1.f);

	// 최종 카메라 월드 행렬
	mWorld = mScale * mRot * mTrans;
}