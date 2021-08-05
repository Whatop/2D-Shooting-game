#pragma once
class UI : public Singleton<UI>
{
	Sprite* PlayerBar;
	Sprite* BossBar;
	Sprite* MiniBossBar;
	TextMgr* m_Test;

public:
	UI();
	~UI();

	void Init();
	void Release();

	void Update();
	void Render();
};

