#pragma once

// 일회성 이펙트 애니메이션을 재생하고,
// 끝나면 자기 자신을 제거하는 오브젝트
class EffectMgr : public Object
{
	Animation* Effect; // 실제 이펙트 애니메이션

public:
	// filename : 스프라이트 시트 경로
	// start/end : 시작/끝 프레임
	// speed : 재생 속도
	// Pos : 생성 위치
	// scalex/y : 크기 배율
	EffectMgr(std::wstring filename, int start, int end, float speed, Vec2 Pos, float scalex = 1, float scaley = 1);
	~EffectMgr();

	int Start; // 시작 프레임
	int End;   // 종료 프레임

	void Update(float deltaTime, float time);
	void Render();
	void OnCollision(Object* other);
};