#pragma once
class Box : public Object
{
	Sprite* m_Box;
public:
	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);

	void Move();
};

