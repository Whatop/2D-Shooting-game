#pragma once

// 모든 게임 오브젝트의 공통 기반 클래스
class Object
{
private:
	float DestroyTime; // DelayDestroy에 사용하는 누적 시간

public:
	Object* m_Parent;        // 부모 오브젝트
	Matrix m_wMat;           // 월드 변환 행렬

	Vec2 m_Position;         // 위치
	Vec2 m_Scale;            // 스케일
	Vec2 m_Size;             // 크기
	Vec2 m_RotationCenter;   // 회전 중심점
	Vec2 m_ScaleCenter;      // 스케일 중심점

	float m_Radius;          // 원형 충돌 등에 사용할 반지름
	float m_Rotation;        // 회전값

	float m_Atk;             // 공격력 등 게임용 수치

	bool m_Destroy;          // 제거 예정 여부
	RECT m_Collision;        // 충돌 박스

	bool m_Visible;          // 렌더 여부

	int m_Layer;             // 레이어 값
	std::string m_Tag;       // 오브젝트 식별용 태그

public:
	Object();
	virtual ~Object();

protected:
	// 현재 오브젝트의 2D 변환행렬 계산
	// 부모가 있으면 부모 행렬까지 곱해서 반환
	Matrix GetMatrix();

public:
	// 프레임 갱신
	virtual void Update(float deltaTime, float time);

	// 렌더링
	virtual void Render();

	// 충돌 시 호출되는 가상 함수
	virtual void OnCollision(Object* other);

public:
	// 현재 위치에 상대 이동
	void Translate(float x, float y);

	// 스케일 설정
	void SetScale(float x, float y);

	// 위치 설정
	void SetPosition(Vec2 pos);
	void SetPosition(float x, float y);

	// 회전값 누적
	void Rotate(float r);

	// 파괴 여부 설정
	void SetDestroy(bool destroy) { m_Destroy = destroy; }

	// 태그 설정
	void SetTag(const std::string tag);

	// 부모 설정
	void SetParent(Object* obj);

	// 일정 시간이 지나면 파괴 처리
	void DelayDestroy(Object* obj, float destroyTime);

	// a ~ b 사이 랜덤 float 반환
	inline float RandRange(float a, float b) {
		return a + (b - a) * (float(rand()) / float(RAND_MAX));
	}

public:
	bool GetDestroy() { return m_Destroy; }
};