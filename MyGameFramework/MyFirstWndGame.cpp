#define _CRTDBG_MAP_ALLOC

#include "MyFirstWndGame.h"
#include "GameTimer.h"
#include "Collider.h"
#include "GameObject.h"
#include "RenderHelp.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "EndingScene.h"
#include <iostream>
#include <cassert>


using namespace learning;
using BitmapInfo = renderHelp::BitmapInfo;

//constexpr int MAX_GAME_OBJECT_COUNT = 1000;

bool MyFirstWndGame::Initialize()
{
    // 
    m_pGameTimer = new GameTimer();
    m_pGameTimer->Reset();

    const wchar_t* className = L"Geometry Dash";
    const wchar_t* windowName = L"Stereo Madness";

    if (false == __super::Create(className, windowName, 1560, 1000))
    {
        return false;
    }

	// 오디오 시스템 초기화 (오디오 실패해도 게임은 계속 진행)
	if (!InitializeAudio())
	{
		std::cout << "Failed to initialize audio system" << std::endl;
	}


    RECT rcClient = {};
    GetClientRect(m_hWnd, &rcClient);
    m_width = rcClient.right - rcClient.left;
    m_height = rcClient.bottom - rcClient.top;

    m_hFrontDC = GetDC(m_hWnd);
    m_hBackDC = CreateCompatibleDC(m_hFrontDC);
    m_hBackBitmap = CreateCompatibleBitmap(m_hFrontDC, m_width, m_height);

    m_hDefaultBitmap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBitmap);

#pragma region resource
    m_pPlayerBitmapInfo = renderHelp::CreateBitmapInfo(L"../Resource/geometryDash/player.png");
    m_pBlock1BitmapInfo = renderHelp::CreateBitmapInfo(L"../Resource/geometryDash/block.png");
    m_pBackgroundBitmapInfo = renderHelp::CreateBitmapInfo(L"../Resource/geometryDash/background.png");
	
    m_pSpikeBitmapInfo = renderHelp::CreateBitmapInfo(L"../Resource/geometryDash/spike.png");
	m_pBlock2BitmapInfo = renderHelp::CreateBitmapInfo(L"../Resource/geometryDash/block02.png");
	m_PlatformBitmapInfo = renderHelp::CreateBitmapInfo(L"../Resource/geometryDash/reg_platform.png");
	m_pSpikeBedBitmapInfo = renderHelp::CreateBitmapInfo(L"../Resource/geometryDash/spikebed.png");
	m_pGoalBitmapInfo = renderHelp::CreateBitmapInfo(L"../Resource/geometryDash/goal.png");


    if (m_pPlayerBitmapInfo == nullptr || m_pBackgroundBitmapInfo == nullptr)
    {
        std::cout << "Bitmap Load Failed!" << std::endl;
        return false;
    }

    std::cout << "Bitmap Load OK" << std::endl;

#pragma endregion

    m_pScenes[SceneType::SCENE_TITLE] = new TitleScene();
    m_pScenes[SceneType::SCENE_TITLE]->Initialize(this);   

    m_pScenes[SceneType::SCENE_PLAY] = new PlayScene();
    m_pScenes[SceneType::SCENE_PLAY]->Initialize(this);

    m_pScenes[SceneType::SCENE_ENDING] = new EndingScene();
    m_pScenes[SceneType::SCENE_ENDING]->Initialize(this);

    return true;
}

void MyFirstWndGame::Run()
{
    MSG msg = { 0 };
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_LBUTTONDOWN)
            {
                MyFirstWndGame::OnLButtonDown(LOWORD(msg.lParam), HIWORD(msg.lParam));
			}
			else if (msg.message == WM_KEYDOWN && msg.wParam == VK_SPACE)
			{
				
                MyFirstWndGame::OnSpaceKeyDown();
			}
            else if (msg.message == WM_MOUSEMOVE)
            {
                MyFirstWndGame::OnMouseMove(LOWORD(msg.lParam), HIWORD(msg.lParam));
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            Update();
            Render();
        }
    }
}

void MyFirstWndGame::Finalize()
{
    FinalizeAudio();

    delete m_pGameTimer;
    m_pGameTimer = nullptr;

    for(auto & pScene : m_pScenes)
    {
        if (pScene)
        {
            pScene->Finalize();
            delete pScene;
            pScene = nullptr;
        }
    }

    __super::Destroy();
}

void MyFirstWndGame::ChangeScene(SceneType eSceneType)
{
    m_pScenes[m_eCurrentScene]->Leave();
    m_eCurrentScene = eSceneType;
    m_pScenes[m_eCurrentScene]->Enter();
}

void MyFirstWndGame::FixedUpdate()
{
    m_pScenes[m_eCurrentScene]->FixedUpdate();
}

void MyFirstWndGame::LogicUpdate()
{
    m_pScenes[m_eCurrentScene]->Update(m_fDeltaTime);

}


bool MyFirstWndGame::InitializeAudio()
{
	AudioSystem* pAudioSystem = AudioSystem::GetInstance();
	if (!pAudioSystem->Initialize())
	{
		return false;
	}

	// Stereo Madness 배경 음악 로드
	if (!pAudioSystem->LoadBackgroundMusic("../Resource/geometryDash/Geometry Dash - Stereo Madness.mp3"))
	{
		std::cerr << "Failed to load background music" << std::endl;
		return false;
	}

	// 배경 음악 재생 시작
	pAudioSystem->PlayBackgroundMusic(true); // 루프 재생

	return true;
}

void MyFirstWndGame::FinalizeAudio()
{
	AudioSystem::GetInstance()->Shutdown();
	AudioSystem::DestroyInstance();
}

void MyFirstWndGame::Update()
{
    m_pGameTimer->Tick();

    LogicUpdate();

    m_fDeltaTime = m_pGameTimer->DeltaTimeMS();
    m_fFrameCount += m_fDeltaTime;

    while (m_fFrameCount >= 200.0f)
    {
        FixedUpdate();
        m_fFrameCount -= 200.0f;
    }
}

void MyFirstWndGame::Render()
{
    //Clear the back buffer
    ::PatBlt(m_hBackDC, 0, 0, m_width, m_height, WHITENESS);

    m_pScenes[m_eCurrentScene]->Render(m_hBackDC);

    BitBlt(m_hFrontDC, 0, 0, m_width, m_height, m_hBackDC, 0, 0, SRCCOPY);
}

void MyFirstWndGame::OnResize(int width, int height)
{
    std::cout << __FUNCTION__ << std::endl;

    learning::SetScreenSize(width, height);

    __super::OnResize(width, height);

    m_hBackBitmap = CreateCompatibleBitmap(m_hFrontDC, m_width, m_height);

    HANDLE hPrevBitmap = (HBITMAP)SelectObject(m_hBackDC, m_hBackBitmap);

    DeleteObject(hPrevBitmap);
}

void MyFirstWndGame::OnClose()
{
    std::cout << __FUNCTION__ << std::endl;

    SelectObject(m_hBackDC, m_hDefaultBitmap);

    DeleteObject(m_hBackBitmap);
    DeleteDC(m_hBackDC);

    ReleaseDC(m_hWnd, m_hFrontDC);
}

void MyFirstWndGame::OnMouseMove(int x, int y)
{
    /*   std::cout << __FUNCTION__ << std::endl;
       std::cout << "x: " << x << ", y: " << y << std::endl;*/
    m_MousePosPrev = m_MousePos;
    m_MousePos = { x, y };
}

void MyFirstWndGame::OnLButtonDown(int x, int y)
{
    /*  std::cout << __FUNCTION__ << std::endl;
 std::cout << "x: " << x << ", y: " << y << std::endl;*/

    m_PlayerTargetPos.x = x;
    m_PlayerTargetPos.y = y;

}

void MyFirstWndGame::OnSpaceKeyDown()
{
	// 현재 씬에 따라 다른 처리
	switch (m_eCurrentScene)
	{
	case SCENE_TITLE:
		// 타이틀 씬에서는 스페이스바를 눌러 게임 시작
		static_cast<TitleScene*>(m_pScenes[m_eCurrentScene])->OnSpaceKeyPressed();
		break;

	case SCENE_PLAY:
		// 플레이 씬에서는 점프 동작
		// 이미 PlayScene::UpdatePlayerInfo()에서 처리되므로 별도 코드 불필요
		break;

	case SCENE_ENDING:
		// 엔딩 씬에서는 스페이스바를 눌러 타이틀로 돌아가기
		static_cast<EndingScene*>(m_pScenes[m_eCurrentScene])->OnSpaceKeyPressed();
		break;

	default:
		break;
	}
}