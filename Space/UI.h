#pragma once
class UI : public Singleton<UI>
{
	Sprite* m_HpBar;
	Sprite* m_BossBar;
	TextMgr* m_Test;

public:
	UI();
	~UI();

	void Init();
	void Release();

	void Update();
	void Render();
};

