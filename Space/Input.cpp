#include "stdafx.h"
#include "Input.h"

Input::Input()
	: m_MousePosition(0.f, 0.f)
	, m_ButtonDown(false)
	, m_RightButtonDown(false)
{
	// 키 상태 배열 초기화
	ZeroMemory(&m_CurrentState, sizeof(m_CurrentState));
	ZeroMemory(&m_PrevState, sizeof(m_PrevState));
}

Input::~Input()
{
}

// 매 프레임 입력 갱신
void Input::Update()
{
	KeyBoardUpdate();
	MouseUpdate();
}

// 왼쪽 마우스 버튼 상태 저장
void Input::ButtonDown(bool down)
{
	m_ButtonDown = down;
}

// 오른쪽 마우스 버튼 상태 저장
void Input::RightButtonDown(bool down)
{
	m_RightButtonDown = down;
}

// 특정 키의 이전/현재 상태를 비교해서 입력 상태 반환
KeyState Input::GetKey(int key)
{
	if (key < 0 || key >= 256)
		return KeyState::NONE;

	const bool prev = m_PrevState[key];
	const bool curr = m_CurrentState[key];

	if (prev && curr)
		return KeyState::PRESS;
	else if (prev && !curr)
		return KeyState::UP;
	else if (!prev && curr)
		return KeyState::DOWN;

	return KeyState::NONE;
}

// 키보드 상태 갱신
void Input::KeyBoardUpdate()
{
	for (int i = 0; i < 256; i++)
	{
		m_PrevState[i] = m_CurrentState[i]; // 이전 상태 저장
		m_CurrentState[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
	}
}

// 마우스 위치 갱신
void Input::MouseUpdate()
{
	POINT position;
	GetCursorPos(&position); // 화면 전체 기준 마우스 위치

	ScreenToClient(App::GetInst()->GetHwnd(), &position); // 클라이언트 좌표로 변환

	// 카메라 위치를 더해서 월드 좌표처럼 사용
	m_MousePosition.x = static_cast<float>(position.x) + Camera::GetInst()->m_Position.x;
	m_MousePosition.y = static_cast<float>(position.y) + Camera::GetInst()->m_Position.y;
}