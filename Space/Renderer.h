#pragma once

class Renderer : public Singleton<Renderer>
{
private:
	LPDIRECT3D9 m_pD3D;          // Direct3D9 인터페이스
	LPDIRECT3DDEVICE9 m_pDevice; // 실제 렌더링 장치
	LPD3DXSPRITE m_pSprite;      // 2D 스프라이트 렌더링용 객체

public:
	Renderer();
	~Renderer();

	// 렌더러 자원 해제
	void Release();

	// Direct3D 및 디바이스 초기화
	bool Init(int width, int height, bool windowMode);

	// 외부에서 디바이스 접근
	LPDIRECT3DDEVICE9 GetDevice() const {
		return m_pDevice;
	}

	// 외부에서 스프라이트 접근
	LPD3DXSPRITE GetSprite() const {
		return m_pSprite;
	}

public:
	// 프레임 렌더링 시작
	void Begin();

	// 프레임 렌더링 종료 및 화면 출력
	void End();
};