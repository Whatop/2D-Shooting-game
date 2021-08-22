#pragma once
class ChoiceWindow : public Object
{
	Sprite* m_Choice;
	Sprite* ChoicePack[6];

	int RCrad[5];
public:
	ChoiceWindow();
	~ChoiceWindow();

	void Update(float deltaTime, float Time);
	void Render();

	void OnCollisionCard();
};

