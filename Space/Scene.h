#pragma once

// Scene은 각 장면(타이틀, 스테이지, 로비 등)의 공통 인터페이스
// 순수 가상 함수만 가지고 있어서, 실제 씬 클래스들이 이걸 상속받아 구현하게 됨
class Scene
{
public:
	Scene();
	~Scene();

	// 씬 시작 시 초기화
	virtual void Init() = 0;

	// 씬 종료 시 정리
	virtual void Release() = 0;

	// 매 프레임 갱신
	// deltaTime : 프레임 간 시간
	// Time      : 누적 시간
	virtual void Update(float delatTime, float Time) = 0;

	// 매 프레임 렌더링
	virtual void Render() = 0;
};