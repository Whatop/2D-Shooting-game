#include "stdafx.h"
#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Release()
{
	// DirectX 자원 해제, 생성과 역순	
	if (m_pSprite != nullptr)
	{
		m_pSprite->Release();
		m_pSprite = nullptr;
	}

	if (m_pDevice != nullptr)
	{
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}

bool Renderer::Init(int width, int height, bool windowMode)
{
	// Direct3D9 인터페이스 생성
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == nullptr)
		return false;

	D3DPRESENT_PARAMETERS pp = {};
	pp.BackBufferWidth = width;              // 백버퍼 가로 크기
	pp.BackBufferHeight = height;            // 백버퍼 세로 크기
	pp.BackBufferFormat = D3DFMT_A8R8G8B8;   // 백버퍼 포맷
	pp.SwapEffect = D3DSWAPEFFECT_DISCARD;   // 스왑 방식
	pp.Windowed = windowMode;                // 창모드 여부

	// 디바이스 생성
	HRESULT hr = m_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		App::GetInst()->GetHwnd(),
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&pp,
		&m_pDevice);

	if (FAILED(hr))
	{
		Release();
		return false;
	}
	// 스프라이트 객체 생성
	hr = D3DXCreateSprite(m_pDevice, &m_pSprite);

	if (FAILED(hr))
	{
		Release();
		return false;
	}
	return true;
}

void Renderer::Begin()
{
	if (m_pDevice == nullptr)
		return;

	// 화면을 검은색으로 지움
	m_pDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.f, 0);
	// 렌더링 시작
	m_pDevice->BeginScene();
}

void Renderer::End()
{
	if (m_pDevice == nullptr)
		return;

	// 렌더링 종료
	m_pDevice->EndScene();

	// 백버퍼 내용을 화면에 출력
	m_pDevice->Present(0, 0, 0, 0);
}