#pragma once
class Pet : public Object
{
	Sprite* m_Pet;

	Vec2 Dire;
	float m_Speed;
	float Limit;
	float Count;

	float ShotTime;

	int Gun;
public:
	Pet(int type = 1);
	~Pet();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);
	
	void Move();
	void Attack();
};

