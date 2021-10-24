#pragma once
class MainScene : public Scene
{
	Sprite* m_MainScene;
	Sprite* m_Title;
	Sprite* m_Button[4];

	SoundMgr* BG;
	SoundMgr* Button;
public:
	MainScene();
	~MainScene();

	void Init();
	void Release();

	void Update(float deltaTime,float time);
	void Render();
};
