#include "stdafx.h"
#include "InputScoreScene.h"
#include "MainScene.h"

InputScoreScene::InputScoreScene()
{
}

InputScoreScene::~InputScoreScene()
{
}

void InputScoreScene::Init()
{
	ObjMgr->Release();
	GameMgr::GetInst()->ReleaseUI();
	Camera::GetInst()->m_Position = Vec2(0, 0);
	m_BG = Sprite::Create(L"Painting/Scene/Black.png");
	m_BG->SetPosition(1920 / 2, 1080 / 2);

	m_ScoreText = Sprite::Create(L"Painting/Scene/Score.png");
	m_ScoreText->SetPosition(1920 / 2, 600);

	m_NameText = Sprite::Create(L"Painting/Scene/Name.png");
	m_NameText->SetPosition(1920 / 2, 300);

	m_BackButton = Sprite::Create(L"Painting/Scene/BackButton.png");
	m_BackButton->SetPosition(1700, 900);

	m_Name = new TextMgr();
	m_Score = new TextMgr();

	m_Name->Init(72, false, false, "Arial");
	m_Score->Init(72, false, false, "Arial");

	m_isTextEntered = true;
	GameInfo->m_Scene = StageScene::NONE;
}

void InputScoreScene::Release()
{
}

void InputScoreScene::Update(float deltaTime, float Time)
{
	if (m_isTextEntered)
	{
		if (name.size() > 10)
			m_isTextEntered = false;

		for (int i = 0; i < 256; i++)
		{
			m_PrevKey[i] = m_Key[i];
			m_Key[i] = static_cast<bool>(GetAsyncKeyState(i));
		}
		for (int i = 0x41; i < 0x5A; i += 0x01)
		{
			bool key = false;
			bool prevkey = false;
			prevkey = m_PrevKey[i];
			key = m_Key[i];
			if (key && !prevkey)
			{
				std::string str;
				str = (char)i;
				name += str;
			}
		}
		if (INPUT->GetKey(VK_BACK) == KeyState::DOWN && name.size() >= 1)
		{
			name = name.substr(1);
		}
	}

	if (name.size() > 0)
	{
		m_OverOne = true;
	}
	else if (name.size() <= 0)
	{
		m_OverOne = false;
	}

	if (m_OverOne)
	{
		if (CollisionMgr::GetInst()->MouseWithBoxSize(m_BackButton) && INPUT->GetButtonDown())
		{
			GameInfo->m_Rank->name = name;
			GameInfo->m_Rank->score = GameInfo->m_Score;
			SceneDirector::GetInst()->ChangeScene(new MainScene());
		}
	}
}

void InputScoreScene::Render()
{
	m_BG->Render();
	m_ScoreText->Render();
	m_NameText->Render();
	m_BackButton->Render();

	Renderer::GetInst()->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
	m_Score->print(std::to_string(int(GameMgr::GetInst()->m_Score)), 1100, 570);
	m_Name->print(name, 1100, 270);
	Renderer::GetInst()->GetSprite()->End();
}
