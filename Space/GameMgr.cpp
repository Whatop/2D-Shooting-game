#include "stdafx.h"
#include "GameMgr.h"
#include "UI.h"

GameMgr::GameMgr()
{
}

GameMgr::~GameMgr()
{
}

void GameMgr::Init()
{
	m_isCreateUI = false;
	m_DebugMode = false;
	m_isCreatePlayer = false;
	AutoCamera = true;
}

void GameMgr::Release()
{
}

void GameMgr::CreateUI()
{
	if (!m_isCreateUI)
	{
		UI::GetInst()->Init();
		m_isCreateUI = true;
	}
}

void GameMgr::ReleaseUI()
{
	UI::GetInst()->Release();
	UI::GetInst()->ReleaseInst();
	m_isCreateUI = false;
}

void GameMgr::CreatePlayer()
{
	ObjMgr->AddObject(new Player(), "Player");
	m_isCreatePlayer = true;
}

void GameMgr::PlayerDeath()
{
	m_isCreatePlayer = false;
	ObjMgr->DeleteObject("Player");
	ObjMgr->DeleteObject("pBullet");
}


void GameMgr::Update()
{
	if (INPUT->GetKey('V') == KeyState::DOWN)
	{
		if (m_DebugMode)
		{
			m_DebugMode = false;
			
			std::cout << "디버깅 모드 비 활성화" << std::endl;
		}
		else if (!m_DebugMode)
		{
			m_DebugMode = true;
			std::cout << "디버깅 모드 활성화" << std::endl;
		}
	}
	if (INPUT->GetKey(VK_F1) == KeyState::DOWN) {
		if (!AutoCamera) {
			AutoCamera = true;


			std::cout << "AUTO 카메라 OFF" << std::endl;
		}
		else {
			AutoCamera = false;
			std::cout << "AUTO 카메라 ON" << std::endl;
		}
	}


	if (m_isCreateUI)
		UI::GetInst()->Update();
}

void GameMgr::Render()
{
	if (m_isCreateUI)
		UI::GetInst()->Render();
}
