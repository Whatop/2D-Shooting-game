#pragma once
class TextMgr
{
private:
	D3DXCOLOR m_Color;
	LPD3DXFONT m_pFont;
	RECT m_FontRect;
	Matrix m_wMat;

public:
	TextMgr();
	~TextMgr();

	float m_Angle;

	bool Init(int height, bool bold, bool italic, const std::string& fontname);
	int print(const std::string& str, int x, int y);
	// 추가: 와이드 문자열 출력
	int  print(const std::wstring& str, int x, int y);

	void SetColor(int a, int r, int g, int b);

	void Release();
};

