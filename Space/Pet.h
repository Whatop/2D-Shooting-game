#pragma once
class Pet : public Object
{
	Sprite* m_Pet;
public:
	Pet();
	~Pet();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);
};

