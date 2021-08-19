#pragma once
class StoreScene : public Scene
{
	Sprite* m_BackGround;
	
	//파츠 3개 랜덤
	Sprite* RandomPart[3];

	// 공격력 UP, 체력 UP, 탄환 구매,
	
	// 펫

	// 탄환 특수능력 (업그레이드)



public:
	StoreScene();
	~StoreScene();

	void Init();
	void Update(float deltaTime, float Time);
	void Render();
	void Release();
};

