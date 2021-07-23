#pragma once
class MainScene : public Scene
{
	Sprite* m_Map;
	Sprite* UpWall;
	Sprite* DownWall;
	Sprite* LeftWall;
	Sprite* RightWall;
	Sprite* Right_Limit;
	Sprite* Left_Limit;
public:
	MainScene();
	~MainScene();

	void Init();
	void Release();

	void Update(float deltaTime,float time);
	void Render();
};

