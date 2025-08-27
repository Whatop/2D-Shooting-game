#pragma once
#include "TextBar.h"   // Ãß°¡

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

	class TextBar* MessageBar = nullptr;
	class TextMgr* StageText = nullptr;
	class TextMgr* SoundText = nullptr;
	class TextMgr* m_CardLvTxt[6] = { nullptr };
	class TextTypeSfx* m_TypeSfx = nullptr;

	Sprite* StateWindow;


	// KILL, ITEM, BONUS, ALL
	float limit[4];

	float ScoredaleyTime;
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

