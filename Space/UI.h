#pragma once
class UI : public Singleton<UI>
{
	Sprite* PlayerBar;
	Sprite* BossBar;
	Sprite* MiniBossBar;
	TextMgr* m_Test;
	Sprite* Pack[6];

	Sprite* UIScoreFrame;

	TextMgr* ScoreText;

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

