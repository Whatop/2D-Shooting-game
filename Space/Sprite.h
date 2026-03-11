#pragma once

// 특정 색을 투명 처리할 때 쓰는 컬러키 상수들
#define COLORKEY_PURPLE D3DCOLOR_XRGB(88, 64, 172)
#define COLORKEY_GASTER D3DCOLOR_XRGB(195, 134, 255)
#define COLORKEY_PINK D3DCOLOR_XRGB(255, 102, 255)
//#define COLORKEY_SKY D3DCOLOR_XRGB(75, 169, 218)
#define COLORKEY_WHITE D3DCOLOR_XRGB(255,255,255)
#define COLORKEY_BLACK D3DCOLOR_XRGB(0,0,0)
#define COLORKEY_GREEN D3DCOLOR_XRGB(0,255,0)

class Texture;

// Sprite는 Object를 상속받는 "이미지 1장 렌더링 객체"
// Texture를 들고 있고, 위치/스케일/회전은 Object 쪽 정보를 사용
class Sprite : public Object
{
	LPD3DXSPRITE m_pSp;   // DirectX Sprite 렌더링 객체
	Texture* m_Texture;   // 실제 텍스처 데이터

public:
	Sprite();
	~Sprite();

	RECT m_Rect; // 텍스처에서 어느 영역을 그릴지 지정하는 사각형

public:
	// 파일에서 텍스처를 불러와 Sprite 초기화
	bool Init(std::wstring fileName, D3DCOLOR ColorKey);

public:
	// 현재 Sprite가 들고 있는 텍스처 반환
	Texture* GetSpriteTexture() { return m_Texture; }

	// Sprite 생성용 정적 함수
	static Sprite* Create(std::wstring fileName, D3DCOLOR ColorKey = COLORKEY_GREEN);

	// 실제 렌더링
	void Render() override;

public:
	// 알파/색상 조절 값
	int A, R, G, B;
};