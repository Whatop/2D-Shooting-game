#pragma once
class Stage1 : public Scene
{
	Sprite* m_BackGround[6][4];
	Sprite* UpWall;
	Sprite* DownWall;
	Sprite* Right_Limit;
	Sprite* Left_Limit;

	Sprite* MoneyColBox;
	Sprite* m_Choice;
	Sprite* ChoicePack[3];

	Sprite* ScoreScene;
	Sprite* ScoreText;
	SoundMgr* m_Bgm;
	float ScaleScene;
	float ScaleText;
	int RCrad[3];

public:
	Stage1();
	~Stage1();

	void Init();
	void Release();

	void Update(float deltaTime, float time);
	void Render();

	void BGInit();

	void MoveBG();
	void ResetBG();
	void OnCollisionCard();
	void NextScene();
};

