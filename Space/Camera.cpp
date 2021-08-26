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
	m_MaxMapSize = Vec2(300000, 0); //Stage1
	isVibration = false;
	ShakeTimeX = 4;
	ShakeTimeY = 4;
	XShakePosition = 0.f;

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
				if (m_MaxMapSize.x >= m_Position.x) {
					m_Position.x += 100 * dt;
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

	ShakeTimeX += dt;
	ShakeTimeY += dt;
	if (isVibration && ShakeTimeY < 6.f) {
		m_Position.y += (sin(2.0f * 3.14159f * ShakeTimeY * 4) * 3.5f +
			sin(2.0f * 3.14159f * ShakeTimeY * 8 + 0.2f) * 3.4f +
			sin(2.0f * 3.14159f * ShakeTimeY * 16 + 0.5f) * 3.3f) * (5 - ShakeTimeY) / 5;
	}
	if (isVibration && ShakeTimeX < 0.5f) {
		m_Position.x += (sin(2.0f * 3.14159f * ShakeTimeX * 4) * 2.3f +
			sin(2.0f * 3.14159f * ShakeTimeX * 8 + 0.2f) * 2.2f +
			sin(2.0f * 3.14159f * ShakeTimeX * 16 + 0.5f) * 2.1f) * (5 - ShakeTimeX) / 5;
	}
	/*
	ÆøÅº ÅÍÆ®¸±¶§ Ãß°¡ÇÒ±î?
	m_Position.x += (sin(2.0f * 3.14159f * ShakeTimeX * 4) * 2.3f +
		sin(2.0f * 3.14159f * ShakeTimeX * 8 + 0.2f) * 2.2f +
		sin(2.0f * 3.14159f * ShakeTimeX * 16 + 0.5f) * 2.1f) * (5 - ShakeTimeX) / 5;*/
}

void Camera::Render()
{
	D3DXMatrixRotationZ(&mRot, D3DXToRadian(m_Rotation));
	D3DXMatrixTranslation(&mTrans, -m_Position.x, -m_Position.y, 1.f);
	D3DXMatrixScaling(&mScale, m_Scale.x, m_Scale.y, 1.f);

	mWorld = mScale * mRot * mTrans;
}

