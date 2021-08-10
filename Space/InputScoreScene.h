#pragma once
class InputScoreScene : public Scene
{
	Sprite* m_BG;
	Sprite* m_ScoreText;
	Sprite* m_NameText;

	TextMgr* m_Score;
	TextMgr* m_Name;

	Sprite* m_BackButton;
public:
	InputScoreScene();
	~InputScoreScene();

	bool m_isTextEntered;
	bool m_OverOne;
	int m_Key[256];
	int m_PrevKey[256];

	std::string name;

	void Init();
	void Release();

	void Update(float deltaTime, float Time);
	void Render();
};

