#pragma once
class Stage2 : public Scene
{
	Sprite* m_BackGround[10][4];
	Sprite* UpWall;
	Sprite* DownWall;
	Sprite* Right_Limit;
	Sprite* Left_Limit;
public:
	Stage2();
	~Stage2();

	void Init();
	void Release();

	void Update(float deltaTime, float time);
	void Render();

	void BGInit();
};

