#include "stdafx.h"
#include "App.h"

App::App()
{
}


App::~App()
{
}

// 앱 초기화
// 1. 해상도/창모드 저장
// 2. 윈도우 생성
// 3. 렌더러 생성
// 4. 누적 시간 초기화
bool App::Init(int width, int height, bool windowMode)
{
	m_Width = width;
	m_Height = height;
	m_WindowMode = windowMode;


	if (!_CreateWindow())
		return false;

	if (!_CreateRenderer())
		return false;

	Time = 0.0f;
	return true;
}

// 메인 루프
// 윈도우 메시지를 처리하고,
// 메시지가 없으면 게임 로직 업데이트와 렌더링을 수행
void App::Run()
{
	srand(time(NULL)); // 난수 시드 초기화

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (msg.message != WM_QUIT)
	{
		static DWORD lastTime = timeGetTime(); // 이전 프레임 시간
		static float fps = 0.f;

		DWORD curTime = timeGetTime(); // 현재 시간
		float timeDelta = float(curTime - lastTime) * 0.001f; // 초 단위 변환
		fps = 1000.f / float(curTime - lastTime); // 현재 FPS 계산

		// 메시지가 있으면 메시지 처리
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// 메시지가 없으면 게임 프레임 진행
		else
		{
			Renderer::GetInst()->Begin();                    // 렌더 시작
			SceneDirector::GetInst()->Update(timeDelta, Time); // 현재 씬 업데이트
			DeltaTime = timeDelta;                           // 델타타임 저장
			Time += timeDelta;                               // 누적 시간 증가
			SceneDirector::GetInst()->Render();              // 현재 씬 렌더링
			Renderer::GetInst()->End();                      // 렌더 종료 및 출력

			lastTime = curTime; // 프레임 시간 갱신
		}
	}
}

// 종료 시 자원 정리
void App::Release()
{
	FreeConsole();               // 콘솔 창 해제
	Renderer::GetInst()->Release(); // 렌더러 해제
	ObjMgr->Release();           // 오브젝트 매니저 해제
}

// 윈도우 메시지 처리
LRESULT App::WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0); // 종료 메시지
		break;

	case WM_LBUTTONDOWN:
		INPUT->ButtonDown(true); // 마우스 왼쪽 클릭 눌림
		break;

	case WM_LBUTTONUP:
		INPUT->ButtonDown(false); // 마우스 왼쪽 클릭 해제
		break;

	case WM_RBUTTONDOWN:
		INPUT->RightButtonDown(true); // 마우스 오른쪽 클릭 눌림
		break;

	case WM_RBUTTONUP:
		INPUT->RightButtonDown(false); // 마우스 오른쪽 클릭 해제
		break;
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

// 실제 윈도우 생성
bool App::_CreateWindow()
{
	WNDCLASS wc = {};
	wc.lpszClassName = L"Space";         // 윈도우 클래스 이름
	wc.hCursor = LoadCursor(0, IDC_ARROW); // 기본 커서
	wc.lpfnWndProc = WndProc;            // 메시지 처리 함수 연결

	RegisterClass(&wc);

	DWORD Style = 0;

	// 창모드 / 전체화면 비슷한 팝업모드 분기
	if (m_WindowMode)
		Style = WS_OVERLAPPEDWINDOW;
	else
		Style = WS_POPUP | WS_EX_TOPMOST;

	HWND hWnd = CreateWindow(
		wc.lpszClassName, wc.lpszClassName,
		Style,
		0, 0,
		m_Width, m_Height,
		0, 0, 0, 0
	);

	if (hWnd)
		m_Hwnd = hWnd;
	else
		return false;

	ShowWindow(m_Hwnd, SW_SHOWDEFAULT);

	return true;
}

// 렌더러 생성 및 초기화
bool App::_CreateRenderer()
{
	if (!(Renderer::GetInst()->Init(m_Width, m_Height, m_WindowMode)))
		return false;

	return true;
}