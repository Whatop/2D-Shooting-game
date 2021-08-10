#pragma once
class Stage1 : public Scene
{
	Sprite* m_Map;
	Sprite* UpWall;
	Sprite* DownWall;
	Sprite* LeftWall;
	Sprite* RightWall;
	Sprite* Right_Limit;
	Sprite* Left_Limit;
public:
	Stage1();
	~Stage1();

	void Init();
	void Release();

	void Update(float deltaTime, float time);
	void Render();
};

