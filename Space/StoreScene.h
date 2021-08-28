#pragma once
class StoreScene : public Scene
{
	Sprite* m_BackGround;
	
	// 나가기
	Sprite* m_Button;

	// 카드 툴
	Sprite* CardFrame[6];

	// 탄환 구매
	Sprite* CardPack[6];


	Sprite* PetPack[6];


	// 공격력 UP, 체력 UP, 탄환 구매,


	// 펫

	// 탄환 특수능력 (업그레이드)

	int RCrad[6];
	int PCrad[6];
	bool isShake;

public:
	StoreScene();
	~StoreScene();

	void Init();
	void Update(float deltaTime, float Time);
	void Render();
	void Release();

	void OnCollisionCard();
};

