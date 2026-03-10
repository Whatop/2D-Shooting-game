#pragma once

// 키의 상태를 프레임 기준으로 구분
enum class KeyState
{
	DOWN,   // 이번 프레임에 새로 눌림
	UP,     // 이번 프레임에 떼짐
	PRESS,  // 계속 누르는 중
	NONE    // 아무 일도 없음
};

#define INPUT Input::GetInst()

class Input : public Singleton<Input>
{
	bool m_CurrentState[256]; // 현재 프레임 키 상태
	bool m_PrevState[256];    // 이전 프레임 키 상태

	Vec2 m_MousePosition;    // 마우스 위치(카메라 보정 포함)

	bool m_ButtonDown;       // 마우스 왼쪽 버튼 상태
	bool m_RightButtonDown;  // 마우스 오른쪽 버튼 상태

public:
	Input();
	~Input();

	// 입력 전체 갱신
	void Update();

	// App의 WndProc에서 마우스 클릭 상태를 넣어줌
	void ButtonDown(bool down);
	void RightButtonDown(bool down);

	// 특정 키의 상태 반환
	KeyState GetKey(int key);

	// 현재 마우스 위치 반환
	Vec2 GetMousePos() { return m_MousePosition; }

	// 마우스 버튼 상태 반환
	bool GetButtonDown() { return m_ButtonDown; }
	bool GetRightButtonDown() { return m_RightButtonDown; }

private:
	// 키보드 상태 갱신
	void KeyBoardUpdate();

	// 마우스 위치 갱신
	void MouseUpdate();
};