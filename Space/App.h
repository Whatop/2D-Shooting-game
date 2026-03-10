#pragma once


class App : public Singleton<App>
{
private:
	HWND m_Hwnd; // 생성된 윈도우의 핸들

public:
	App();
	~App();

public:
	int m_Width;       // 창 가로 크기
	int m_Height;      // 창 세로 크기
	bool m_WindowMode; // true면 창모드, false면 전체화면 비슷한 팝업 모드

private:
	// 내부적으로 윈도우 생성
	bool _CreateWindow();

	// 내부적으로 렌더러 초기화
	bool _CreateRenderer();

public:
	// 앱 초기화
	// 해상도와 창모드 설정 후 윈도우/렌더러 생성
	bool Init(int width, int height, bool windowMode);

	// 메인 루프 실행
	void Run();

	// 자원 해제
	void Release();

public:
	float
		DeltaTime, // 프레임 간 시간 차이
		Time;      // 누적 시간

public:
	// 외부에서 현재 윈도우 핸들 접근용
	HWND GetHwnd() {
		return m_Hwnd;
	}

private:
	// 윈도우 메시지 처리 함수
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
};

