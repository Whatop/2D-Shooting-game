#pragma once
class Pet : public Object
{
	Sprite* m_Pet;

	Vec2 Dire;
	float m_Speed;
	float Limit;
public:
	Pet();
	~Pet();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);
	
	void Move();
};

