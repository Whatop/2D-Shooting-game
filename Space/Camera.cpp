#include"stdafx.h"
#include "Camera.h"
Camera::Camera()
{
	Init();
}

Camera::~Camera()
{
}


void Camera::Init()
{
	 
	m_Rotation = 0;
	m_Position = Vec2(0, -180);
	m_Scale = Vec2(1.f, 1.f);
	Follow(nullptr);
	m_MinMapSize = Vec2(960, 0); //Stage1
	m_MaxMapSize = Vec2(7400, 0); //Stage1
	isVibration = false;
	ShakeTime = 0;
}

void Camera::Translate()
{

}

void Camera::Follow(Object* obj)
{
	if (obj != nullptr)
	{
		m_Position.x = obj->m_Position.x - App::GetInst()->m_Width / 2;
		m_Position.y = obj->m_Position.y - App::GetInst()->m_Height / 2;
	}
}

void Camera::Side_Scroll(Object* obj, float fixed_value,bool Auto)
{
	if (obj != nullptr)
	{
		if (!GameInfo->CameraStop) {
			if (!Auto) {
				if (m_MinMapSize.x <= obj->m_Position.x && m_MaxMapSize.x >= obj->m_Position.x)
					m_Position.x = obj->m_Position.x - App::GetInst()->m_Width / 2;
				
			}
			else {	
				if (m_MaxMapSize.x >= m_Position.x)
					m_Position.x += 100 * dt;
				else {
					GameInfo->CameraStop = true;
				}
			}

			m_Position.y = fixed_value - App::GetInst()->m_Height / 2;
		}
	}
}

void Camera::Update(float deltaTime, float time)
{

	if (m_Rotation >= 360)
		m_Rotation = 0;

	ShakeTime += dt;
	if (isVibration && ShakeTime < 4.f) {
		m_Position.y += (sin(2.0f * 3.14159f * ShakeTime * 4) * 1.3f +
			sin(2.0f * 3.14159f * ShakeTime * 8 + 0.2f) * 1.2f +
			sin(2.0f * 3.14159f * ShakeTime * 16 + 0.5f) * 1.1f) * (5 - ShakeTime) / 5;
	}
}

void Camera::Render()
{
	D3DXMatrixRotationZ(&mRot, D3DXToRadian(m_Rotation));
	D3DXMatrixTranslation(&mTrans, -m_Position.x, -m_Position.y, 1.f);
	D3DXMatrixScaling(&mScale, m_Scale.x, m_Scale.y, 1.f);

	mWorld = mScale * mRot * mTrans;


}

