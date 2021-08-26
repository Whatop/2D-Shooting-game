#pragma once
class Item : public Object
{
	Sprite* m_Item;
	Sprite* m_ColBox;
	Vec2 m_Dire;
	float m_Speed;
	float DestroyTime;
	bool Over;
	bool Under;
	int ItemCode;
	bool isBonus;
public:
	Item(Vec2 Pos, bool bonus = false);
	~Item();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object obj);

	void Move();
};

