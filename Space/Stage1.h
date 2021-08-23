#pragma once
class Stage1 : public Scene
{
	Sprite* m_BackGround[6][4];
	Sprite* UpWall;
	Sprite* DownWall;
	Sprite* Right_Limit;
	Sprite* Left_Limit;

	Sprite* m_Choice;
	Sprite* ChoicePack[3];

	int RCrad[5];
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
};

