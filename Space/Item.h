#pragma once
class Item : public Object
{
	Sprite* m_Item;
	Vec2 m_Dire;
	float m_Speed;
public:
	Item(Vec2 Pos);
	~Item();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object obj);

	void Move();
};

