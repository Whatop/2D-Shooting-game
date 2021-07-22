#pragma once
class UI : public Singleton<UI>
{
	TextMgr* m_Test;
public:
	UI();
	~UI();

	void Init();
	void Release();

	void Update();
	void Render();
};

