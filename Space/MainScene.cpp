#include "stdafx.h"
#include "MainScene.h"
#include "Stage1.h"
#include "Stage2.h"
#include "RankScene.h"
#include "InputScoreScene.h"

MainScene::MainScene()
{
}

MainScene::~MainScene()
{
}
void MainScene::Init()
{
    ObjMgr->Release();
    GameInfo->Init();
    Camera::GetInst()->m_Position = Vec2(0,0);

    m_MainScene = Sprite::Create(L"Painting/MainScreen/Main.png");
    m_MainScene->SetPosition(1920 / 2, 1080 / 2);

    Exp = Sprite::Create(L"Painting/MainScreen/Exp.png");
    Exp->SetPosition(1920 / 2, 1080 / 2);
    Exp->m_Visible = false;

    m_Title = Sprite::Create(L"Painting/MainScreen/Title.png");
    m_Title->SetPosition(1920 / 2, 300);

    m_Button[0] = Sprite::Create(L"Painting/MainScreen/Button/start.png");
    m_Button[0]->SetPosition(300, 900);

    m_Button[1] = Sprite::Create(L"Painting/MainScreen/Button/way.png");
    m_Button[1]->SetPosition(1920/2, 900);

    m_Button[2] = Sprite::Create(L"Painting/MainScreen/Button/exit.png");
    m_Button[2]->SetPosition(1920 - 300, 900);

    m_Button[4] = Sprite::Create(L"Painting/Scene/BackButton.png");
    m_Button[4]->SetPosition(1920-200,400);
    m_Button[4]->m_Visible = false;


    std::cout << "MainScene 이동" << std::endl;
    GameInfo->GunReset();
    GameInfo->m_Scene = StageScene::NONE;
    GameInfo->m_Score = 0;
    GameInfo->Stage = 0;
    isExplain = false;  
    SoundMgr::GetInst()->StopAll();
}

void MainScene::Release()
{
}

void MainScene::Update(float deltaTime, float time)
{
    //게임시작, 게임소개, 게임방법, 게임랭킹(score), 크래딧(credit)
    if (CollisionMgr::GetInst()->MouseWithBoxSize(m_Button[0]) && INPUT->GetButtonDown())
    {
        SceneDirector::GetInst()->ChangeScene(new Stage1());
        INPUT->ButtonDown(false);
    }
    else if (CollisionMgr::GetInst()->MouseWithBoxSize(m_Button[1]) && INPUT->GetButtonDown())
    {
            isExplain = true;

            Exp->m_Visible = isExplain;
            m_Button[4]->m_Visible = isExplain;
        INPUT->ButtonDown(false);
    }
    else if (CollisionMgr::GetInst()->MouseWithBoxSize(m_Button[2]) && INPUT->GetButtonDown())
    {
        App::GetInst()->Release();
        exit(0);
    }
    else if (CollisionMgr::GetInst()->MouseWithBoxSize(m_Button[4]) && INPUT->GetButtonDown())
    {
        isExplain = false;
        Exp->m_Visible = isExplain;
        m_Button[4]->m_Visible = isExplain;
        INPUT->ButtonDown(false);
    }
 
}

void MainScene::Render()
{
    if (isExplain) {
        Exp->Render();
        m_Button[4]->Render();
    }
    else {
        m_MainScene->Render();
        m_Button[0]->Render();
        m_Button[1]->Render();
        m_Button[2]->Render();
        m_Title->Render();
    }
}

