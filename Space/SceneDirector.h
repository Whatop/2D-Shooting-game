#pragma once

// 현재 활성화된 씬을 관리하고,
// 업데이트/렌더링 순서를 조정하는 싱글톤 디렉터
class SceneDirector : public Singleton<SceneDirector>
{
private:
	Scene* m_CurrentScene; // 현재 실행 중인 씬

public:
	SceneDirector();
	~SceneDirector();

	// 씬 교체
	void ChangeScene(Scene* newScene);

	// 한 프레임 업데이트
	void Update(float deltaTime, float time);

	// 한 프레임 렌더링
	void Render();
	void Release();
};