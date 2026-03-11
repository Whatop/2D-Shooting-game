#pragma once

// 현재 카메라 위치를 바로 쓰기 위한 매크로
#define CamPos Camera::GetInst()->m_Position

class Camera : public Singleton<Camera>
{
private:
	Matrix mRot;    // 회전 행렬
	Matrix mTrans;  // 이동 행렬
	Matrix mScale;  // 스케일 행렬
	Matrix mWorld;  // 최종 카메라 월드 행렬

public:
	Camera();
	~Camera();

	bool isVibration; // 화면 흔들림 사용 여부
	float ShakeTimeX; // X축 흔들림 누적 시간
	float ShakeTimeY; // Y축 흔들림 누적 시간

	float m_Rotation;    // 카메라 회전값
	Vec2 m_Position;     // 카메라 위치
	Vec2 m_Scale;        // 카메라 스케일
	Vec2 m_MinMapSize;   // 맵 최소 범위
	Vec2 m_MaxMapSize;   // 맵 최대 범위

	float XShakePosition; // 현재 코드상 거의 사용 안 하는 값

	// 최종 월드 행렬 반환
	Matrix GetWorld()
	{
		return mWorld;
	}

	void Init();
	void Translate(); // 현재 비어 있음
	void Follow(Object* obj);
	void Side_Scroll(Object* obj, float fixed_value, bool Auto);
	void Update(float deltaTime, float time);
	void Render();
};