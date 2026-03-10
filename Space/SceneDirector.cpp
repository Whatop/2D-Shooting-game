#include "stdafx.h"
#include "SceneDirector.h"

SceneDirector::SceneDirector()
	: m_CurrentScene(nullptr) // 시작 시 현재 씬 없음
{
}

SceneDirector::~SceneDirector()
{
	Release();
}

// 씬 교체
void SceneDirector::ChangeScene(Scene* newScene)
{
	// 기존 씬이 있으면 정리 후 삭제
	if (m_CurrentScene)
	{
		m_CurrentScene->Release();
		SafeDelete(m_CurrentScene);
	}

	// 새 씬으로 교체
	m_CurrentScene = newScene;

	// 새 씬 초기화
	m_CurrentScene->Init();
}
void SceneDirector::Release()
{
	if (m_CurrentScene != nullptr)
	{
		m_CurrentScene->Release();
		SafeDelete(m_CurrentScene);
		m_CurrentScene = nullptr;
	}
}

// 프레임 업데이트
void SceneDirector::Update(float deltaTime, float time)
{
	// 카메라 갱신
	Camera::GetInst()->Update(deltaTime, time);

	// 입력 갱신
	Input::GetInst()->Update();

	// 오브젝트 매니저 갱신
	ObjMgr->Update(deltaTime, time);

	// 게임 매니저 갱신
	GameMgr::GetInst()->Update();

	// 현재 씬 갱신
	if (m_CurrentScene)
		m_CurrentScene->Update(deltaTime, time);
}

// 프레임 렌더링
void SceneDirector::Render()
{
	// 현재 씬 렌더링
	if (m_CurrentScene)
		m_CurrentScene->Render();

	// 오브젝트 렌더링
	ObjMgr->Render();

	// 게임 매니저 렌더링
	GameMgr::GetInst()->Render();
}