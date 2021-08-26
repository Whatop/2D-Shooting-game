#pragma once
class Boom : public Object
{
	Animation* m_Boom;
	Sprite* m_ColBox;
	
	float ScaleUp;
public:
	Boom();
	~Boom();

	void Update(float deltaTime, float Time);
	void Render();

	void OnCollision(Object* obj);

};

