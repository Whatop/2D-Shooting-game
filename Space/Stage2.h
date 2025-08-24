#pragma once
class Stage2 : public Scene
{
	Sprite* m_BackGround[10][4];
	Sprite* UpWall;
	Sprite* DownWall;
	Sprite* Right_Limit;
	Sprite* Left_Limit;
	Sprite* MoneyColBox;

	Sprite* m_Choice;
	Sprite* ChoicePack[3];

	Sprite* ScoreScene;
	Sprite* ScoreText;

	float ScaleScene;
	float ScaleText;
	SoundMgr* m_Bgm;

	int RCrad[5];
public:
	Stage2();
	~Stage2();

	void Init();
	void Release();

	void Update(float deltaTime, float time);
	void Render();

	void BGInit();
	void OnCollisionCard();

	void RestBG();
	void NextScene();
	void MoveBG();
};

