#define _CRTDBG_MAP_ALLOC

#include "MyFirstWndGame.h"
#include "GameTimer.h"
#include "Collider.h"
#include "GameObject.h"
#include "RenderHelp.h"
#include <iostream>
#include <assert.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

using namespace learning;

constexpr int MAX_GAME_OBJECT_COUNT = 1000;

bool MyFirstWndGame::Initialize()
{
    m_pGameTimer = new GameTimer();
    m_pGameTimer->Reset();

    const wchar_t* className = L"씬매니저";
    const wchar_t* windowName = L"씬매니저";

    if (false == __super::Create(className, windowName, 1024, 720))
    {
        return false;
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
    m_pPlayerBitmapInfo = renderHelp::CreateBitmapInfo(L"../Resource/redbird.png");
    m_pEnemyBitmapInfo = renderHelp::CreateBitmapInfo(L"../Resource/graybird.png");

    if (m_pPlayerBitmapInfo == nullptr || m_pEnemyBitmapInfo == nullptr)
    {
        std::cout << "Bitmap Load Failed!" << std::endl;
        return false;
    }

    std::cout << "Bitmap Load OK" << std::endl;

#pragma endregion


    m_GameObjectPtrTable = new GameObjectBase * [MAX_GAME_OBJECT_COUNT];

    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        m_GameObjectPtrTable[i] = nullptr;
    }

    // [CHECK]. 첫 번째 게임 오브젝트는 플레이어 캐릭터로 고정!
    CreatePlayer();

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
            else if (msg.message == WM_RBUTTONDOWN)
            {
                MyFirstWndGame::OnRButtonDown(LOWORD(msg.lParam), HIWORD(msg.lParam));
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
    delete m_pGameTimer;
    m_pGameTimer = nullptr;


    if (m_GameObjectPtrTable)
    {
        for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
        {
            if (m_GameObjectPtrTable[i])
            {
                delete m_GameObjectPtrTable[i];
                m_GameObjectPtrTable[i] = nullptr;
            }
        }
        delete[] m_GameObjectPtrTable;
    }

    __super::Destroy();
}

void MyFirstWndGame::FixedUpdate()
{
    float x = m_EnemySpawnPos.x;
    float y = m_EnemySpawnPos.y;

    if (m_EnemySpawnPos.x != 0 && m_EnemySpawnPos.y != 0)
    {
        CreateEnemy();
    }
}

void MyFirstWndGame::LogicUpdate()
{

    UpdatePlayerInfo();

    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        if (m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i]->Update(m_fDeltaTime);
        }
    }
}

void MyFirstWndGame::CreatePlayer()
{
    assert(m_GameObjectPtrTable[0] == nullptr && "Player object already exists!");

    GameObject* pNewObject = new GameObject(ObjectType::PLAYER);

    pNewObject->SetName("Player");
    pNewObject->SetPosition(0.0f, 0.0f); // 일단, 임의로 설정 
    pNewObject->SetSpeed(0.5f); // 일단, 임의로 설정   
    pNewObject->SetWidth(100);
    pNewObject->SetHeight(100);

    pNewObject->SetBitmapInfo(m_pPlayerBitmapInfo);
    pNewObject->SetColliderCircle(50.0f); // 일단, 임의로 설정. 오브젝트 설정할 거 다 하고 나서 하자.

    m_GameObjectPtrTable[0] = pNewObject;
}

void MyFirstWndGame::CreateEnemy()
{
    GameObject* pNewObject = new GameObject(ObjectType::ENEMY);

    pNewObject->SetName("Enemy");

    float x = m_EnemySpawnPos.x;
    float y = m_EnemySpawnPos.y;

    m_EnemySpawnPos = { 0, 0 };

    pNewObject->SetPosition(x, y);
    pNewObject->SetSpeed(1.0f); // 일단, 임의로 설정   
    pNewObject->SetWidth(100);
    pNewObject->SetHeight(100);
    pNewObject->SetBitmapInfo(m_pEnemyBitmapInfo);
    pNewObject->SetColliderCircle(50.0f); // 일단, 임의로 설정. 오브젝트 설정할 거 다 하고 나서 하자.
    
    //--- 그리지 마! ---    
    ColliderCircle cur;
    cur.center = Vector2f(x, y);
    cur.radius = 50.0f;

    ColliderCircle comparision;
    comparision.radius = 50.0f;

    bool isValiable = true;
    int j = 0;
    while (++j < MAX_GAME_OBJECT_COUNT) //0번째는 언제나 플레이어!
    {
        if (m_GameObjectPtrTable[j] != nullptr)  // null 체크는 항상 중요
        {
            comparision.center = m_GameObjectPtrTable[j]->GetPosition();
            if (Intersect(cur, comparision))
            {
                isValiable = false;
                break;
            }
        }
    }
    //---
    
    if(isValiable)
    {
        int i = 0;
        while (++i < MAX_GAME_OBJECT_COUNT) //0번째는 언제나 플레이어!
        {
            if (nullptr == m_GameObjectPtrTable[i])
            {
                m_GameObjectPtrTable[i] = pNewObject;
                break;

            }
        }

        if (i == MAX_GAME_OBJECT_COUNT)
        {
            // 게임 오브젝트 테이블이 가득 찼습니다.
            delete pNewObject;
            pNewObject = nullptr;
        }
    }
    else {
        delete pNewObject;
        pNewObject = nullptr;
    }
}

void MyFirstWndGame::UpdatePlayerInfo()
{
    static GameObject* pPlayer = GetPlayer();

    assert(pPlayer != nullptr);

    Vector2f mousePos(m_PlayerTargetPos.x, m_PlayerTargetPos.y);
    Vector2f playerPos = pPlayer->GetPosition();

    //--- 그리지 마! ---    
    ColliderCircle cur;
    cur.center = playerPos;
    cur.radius = 50.0f;

    ColliderCircle comparision;
    comparision.radius = 50.0f;

    int j = 0;
    while (++j < MAX_GAME_OBJECT_COUNT) //0번째는 언제나 플레이어!
    {
        if (m_GameObjectPtrTable[j] != nullptr)  // null 체크는 항상 중요
        {
            comparision.center = m_GameObjectPtrTable[j]->GetPosition();
            if (Intersect(cur, comparision))
            {
                m_GameObjectPtrTable[0]->SetIsOverlap(true);
                m_GameObjectPtrTable[j]->SetIsOverlap(true);
                break;
            }
            else
            {
                m_GameObjectPtrTable[0]->SetIsOverlap(false);
                m_GameObjectPtrTable[j]->SetIsOverlap(false);
            }
        }
    }
    
    //---


    Vector2f playerDir = mousePos - playerPos;
    float distance = playerDir.Length(); // 거리 계산

    if (distance > 0.1f) //임의로 설정한 거리
    {
        playerDir.Normalize(); // 정규화
        pPlayer->SetDirection(playerDir); // 플레이어 방향 설정
    }
    else
    {
        pPlayer->SetDirection(Vector2f(0, 0)); // 플레이어 정지
    }
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

    //메모리 DC에 그리기
    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        if (m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i]->Render(m_hBackDC);
        }
    }

    //메모리 DC에 그려진 결과를 실제 DC(m_hFrontDC)로 복사
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
    //std::cout << __FUNCTION__ << std::endl;
    //std::cout << "x: " << x << ", y: " << y << std::endl;
    m_MousePosPrev = m_MousePos;
    m_MousePos = { x, y };
}

void MyFirstWndGame::OnLButtonDown(int x, int y)
{
    //std::cout << __FUNCTION__ << std::endl;
    //std::cout << "x: " << x << ", y: " << y << std::endl;

    m_PlayerTargetPos.x = x;
    m_PlayerTargetPos.y = y;

}

void MyFirstWndGame::OnRButtonDown(int x, int y)
{
    std::cout << __FUNCTION__ << std::endl;
    std::cout << "x: " << x << ", y: " << y << std::endl;


    m_EnemySpawnPos.x = x;
    m_EnemySpawnPos.y = y;
    
}
