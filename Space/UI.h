#pragma once
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

	Sprite* StateWindow;

	TextMgr* m_CardLvTxt[6];

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
};

