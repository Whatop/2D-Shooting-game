#pragma once
class MainScene : public Scene
{
	Sprite* m_Map;
	Sprite* UpWall;
	Sprite* DownWall;
	Sprite* LeftWall;
	Sprite* RightWall;
public:
	MainScene();
	~MainScene();

	void Init();
	void Release();

	void Update(float deltaTime,float time);
	void Render();
};

