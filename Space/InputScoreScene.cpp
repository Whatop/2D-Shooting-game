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
	m_BackButton->SetPosition(1700, 500);

	m_Name = new TextMgr();
	m_Score = new TextMgr();

	m_Name->Init(72, false, false, "Arial");
	m_Score->Init(72, false, false, "Arial");

	m_isTextEntered = true;
	GameInfo->m_Scene = StageScene::NONE;
    SoundMgr::GetInst()->StopAll();


    m_Bgm = new SoundMgr("Sound/Store.wav", true);
    m_Bgm->play();
    m_Bgm->volumeSetting(0.1f);
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

        // 알파벳 A~Z
        for (int i = 0x41; i <= 0x5A; i++)
        {
            bool key = m_Key[i];
            bool prevKey = m_PrevKey[i];
            if (key && !prevKey)
            {
                name += static_cast<char>(i);
            }
        }

        // Backspace → 마지막 글자 지우기
        if (INPUT->GetKey(VK_BACK) == KeyState::DOWN && !name.empty())
        {
            name.pop_back();
        }
    }

    m_OverOne = !name.empty();

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
