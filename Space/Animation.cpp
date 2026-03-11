#include "stdafx.h"
#include "Texture.h"
#include "Animation.h"

Animation::Animation()
	: m_AutoPlay(0)
	, m_CurrentFrame(0)
	, m_Delay(0.f)
	, m_FrameCount(0.f)
	, m_FirstFrame(0)
	, m_LastFrame(0)
	, A(255)
	, R(255)
	, G(255)
	, B(255)
{
}

Animation::~Animation()
{
	// 자신이 관리하는 Sprite 프레임들 정리
	for (auto sprite : m_Anims)
	{
		SafeDelete(sprite);
	}
	m_Anims.clear();
}

// firstFrame ~ lastFrame까지 연속된 이미지 파일을 Sprite로 만들어 저장
void Animation::AddContinueFrame(std::wstring fileName, int firstFrame, int lastFrame, D3DCOLOR ColorKey)
{
	m_FirstFrame = firstFrame;
	m_LastFrame = lastFrame;

	// 잘못된 범위면 생성하지 않음
	if (firstFrame > lastFrame)
		return;

	for (int i = firstFrame; i <= lastFrame; i++)
	{
		// 예: "Image/Run" + "0" + ".png"
		Sprite* sprite = Sprite::Create(fileName + std::to_wstring(i) + L".png", ColorKey);

		// 생성 실패 시 건너뜀
		if (!sprite)
			continue;

		// 각 프레임 Sprite의 부모를 Animation으로 설정
		sprite->SetParent(this);

		m_Anims.push_back(sprite);
	}
}

// 다음 프레임으로 이동
void Animation::NextFrame()
{
	if (m_Anims.empty())
		return;

	m_CurrentFrame++;
	m_FrameCount = 0.f;

	// 마지막 프레임을 넘으면 처음으로 되돌림
	if (m_CurrentFrame >= static_cast<int>(m_Anims.size()))
	{
		m_CurrentFrame = 0;
	}
}

// 이전 프레임으로 이동
void Animation::BackFrame()
{
	if (m_Anims.empty())
		return;

	m_CurrentFrame--;
	m_FrameCount = 0.f;

	// 첫 프레임보다 작아지면 마지막 프레임으로 이동
	if (m_CurrentFrame < 0)
	{
		m_CurrentFrame = static_cast<int>(m_Anims.size()) - 1;
	}
}

void Animation::Init(float delay, bool play)
{
	m_Delay = delay;     // 프레임 전환 시간
	m_AutoPlay = play;   // 자동 재생 여부
}

void Animation::Update(float deltaTime, float time)
{
	// 프레임이 하나도 없으면 업데이트 불가
	if (m_Anims.empty())
		return;

	// 프레임 누적 시간 증가
	m_FrameCount += dt;

	// Animation 자체가 파괴 상태면 현재 프레임 Sprite도 파괴 상태로 설정
	if (m_Destroy)
	{
		m_Anims.at(m_CurrentFrame)->SetDestroy(true);
	}

	// 자동 재생이면 delay를 넘었을 때 다음 프레임으로 이동
	if (m_AutoPlay == true)
	{
		if (m_FrameCount > m_Delay)
		{
			m_CurrentFrame++;
			m_FrameCount = 0.f;
		}
	}

	// 마지막 프레임을 넘으면 처음 프레임으로 순환
	if (m_CurrentFrame >= static_cast<int>(m_Anims.size()))
	{
		m_CurrentFrame = 0;
	}

	// Animation의 색상값을 현재 프레임 Sprite에 전달
	m_Anims.at(m_CurrentFrame)->A = A;
	m_Anims.at(m_CurrentFrame)->R = R;
	m_Anims.at(m_CurrentFrame)->G = G;
	m_Anims.at(m_CurrentFrame)->B = B;

	// 부모 자식 구조를 쓰므로 위치/회전/스케일은 Animation의 Object 상태를 따라간다.
	// 현재 프레임 Sprite 업데이트
	m_Anims.at(m_CurrentFrame)->Update(deltaTime, time);

	// 현재 프레임 크기를 Animation 자신도 따라가게 맞춤
	m_Size = m_Anims.at(m_CurrentFrame)->m_Size;
}

void Animation::Render()
{
	// 프레임이 하나도 없으면 렌더 불가
	if (m_Anims.empty())
		return;

	// 부모가 있으면 부모 기준 충돌 범위 갱신
	if (m_Parent)
	{
		SetRect(&m_Parent->m_Collision,
			static_cast<int>(m_Parent->m_Position.x - (m_Size.x * m_Parent->m_Scale.x) / 2),
			static_cast<int>(m_Parent->m_Position.y - (m_Size.y * m_Parent->m_Scale.y) / 2),
			static_cast<int>(m_Parent->m_Position.x + (m_Size.x * m_Parent->m_Scale.x) / 2),
			static_cast<int>(m_Parent->m_Position.y + (m_Size.y * m_Parent->m_Scale.y) / 2));

		m_Parent->m_Size = m_Size;
	}
	else
	{
		// 부모가 없으면 자기 자신 충돌 범위 설정
		SetRect(&m_Collision,
			static_cast<int>(m_Position.x - m_Size.x / 2),
			static_cast<int>(m_Position.y - m_Size.y / 2),
			static_cast<int>(m_Position.x + m_Size.x / 2),
			static_cast<int>(m_Position.y + m_Size.y / 2));
	}

	// 현재 프레임 Sprite 렌더링
	m_Anims.at(m_CurrentFrame)->Render();
}