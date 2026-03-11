#pragma once

// 텍스처 리소스 하나를 관리하는 클래스
class Texture
{
	LPDIRECT3DTEXTURE9 m_pTexture; // 실제 Direct3D 텍스처
	LPDIRECT3DDEVICE9 m_pDev;      // 디바이스 참조
	Vec2 m_Size;                   // 텍스처 크기

public:
	Texture();
	~Texture();

public:
	// 파일에서 텍스처 로드
	bool Init(std::wstring fileName, D3DCOLOR ColorKey);

public:
	// 내부 텍스처 반환
	LPDIRECT3DTEXTURE9 GetTexture() { return m_pTexture; }

	// 텍스처 크기 반환
	Vec2 GetSize() { return m_Size; }
};