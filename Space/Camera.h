#pragma once

#define CamPos Camera::GetInst()->m_Position

class Camera : public Singleton<Camera>
{
private:
	Matrix mRot;
	Matrix mTrans;
	Matrix mScale;
	Matrix mWorld;
public:
	Camera();
	~Camera();

	bool isVibration;
	float ShakeTimeX;
	float ShakeTimeY;

	float m_Rotation;
	Vec2 m_Position;
	Vec2 m_Scale;
	Vec2 m_MinMapSize;
	Vec2 m_MaxMapSize;

	float XShakePosition;
	Matrix GetWorld()
	{
		return mWorld;
	}

	void Init();
	void Translate();
	void Follow(Object* obj);
	void Side_Scroll(Object* obj,float fixed_value,bool Auto);
	void Update(float deltaTime, float time);
	void Render();
};
