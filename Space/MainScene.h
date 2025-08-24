#pragma once
class MainScene : public Scene
{
	Sprite* m_MainScene;
	Sprite* Exp;
	Sprite* m_Title;
	Sprite* m_Button[5];

	SoundMgr* BG;
	SoundMgr* Button;

	bool isExplain;
public:
	MainScene();
	~MainScene();

	void Init();
	void Release();

	void Update(float deltaTime,float time);
	void Render();
};
