#pragma once
#include "TextBar.h"   // 추가

class UI : public Singleton<UI>
{
	Sprite* PlayerBar;
	Sprite* BossBar;
	Sprite* MiniBossBar;

	TextMgr* m_Test;
	TextMgr* Money;
	Sprite* Pack[6];

	Sprite* Boom[3];

	Sprite* UIScoreFrame;

	TextMgr* ScoreText;
	TextMgr* StageText;
	TextMgr* SoundText;

	Sprite* StateWindow;

	TextMgr* m_CardLvTxt[6];

	// KILL, ITEM, BONUS, ALL
	float limit[4];

	float ScoredaleyTime;
	TextBar* MessageBar = nullptr;  // 추가: 상단 상태/알림 텍스트바
public:
	UI();
	~UI();

	void Init();
	void Release();

	void Update();
	void Render();

	void ScoreUI();

	void ScoreTextUI();
	void PushMessage(const std::wstring& msg,bool stick);
};

