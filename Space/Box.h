#pragma once
class Box : public Object
{
	Animation* m_Box;
	Sprite* ColBox[5];
public:
	Box();
	~Box();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);

	void Move();
	
};

