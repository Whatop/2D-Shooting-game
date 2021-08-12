#pragma once
class StoreScene : public Scene
{
	Sprite* m_BackGround;
	
	//ÆÄÃ÷ 3°³ ·£´ý
	Sprite* RandomPart1;
	Sprite* RandomPart2;
	Sprite* RandomPart3;

	// °ø°Ý·Â UP, Ã¼·Â UP, ÅºÈ¯ ±¸¸Å, Æê

	// ÅºÈ¯ Æ¯¼ö´É·Â


public:
	StoreScene();
	~StoreScene();

	void Init();
	void Update(float deltaTime, float Time);
	void Render();
	void Release();
};

