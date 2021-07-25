#pragma once
class Boss : public Object
{
	Sprite* m_Boss;

public:
	Boss();
	~Boss();

	void Update(float deltaTime, float Time);
	void Render();
	void OnCollision(Object* obj);

	
};

