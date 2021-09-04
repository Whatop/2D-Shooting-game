#pragma once

class Coin : public Object
{
	Animation* LoopCoin;
	Sprite* m_Coin;

	float m_Speed;
	float AccTime; // 가속도 타임
public:
	Coin(Vec2 Pos);
	~Coin();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);
	
	void Move();
};

