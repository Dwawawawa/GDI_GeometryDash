#include "PlayScene.h"
#include "MyFirstWndGame.h"
#include "GameObject.h"
#include "Utillity.h"
#include <assert.h>
#include <iostream>

using namespace learning;

constexpr int MAX_GAME_OBJECT_COUNT = 1000;

void PlayScene::Initialize(NzWndBase* pWnd)
{
    m_pGame = dynamic_cast<MyFirstWndGame*>(pWnd);
    assert(m_pGame != nullptr);

    m_GameObjectPtrTable = new GameObjectBase*[MAX_GAME_OBJECT_COUNT];

    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        m_GameObjectPtrTable[i] = nullptr;
    }

    //--배경이 씬과 상관없이 넘어갈려면... 근데 유니티 스타일이라서 어쩔 수 없나;;
    Background* pNewObject = new Background(ObjectType::BACKGROUND);
    pNewObject->SetPosition(0.0f, 0.0f);

    int width = m_pGame->GetWidth();
    int height = m_pGame->GetHeight();

    pNewObject->SetWidth(width);
    pNewObject->SetHeight(height);

    pNewObject->SetBitmapInfo(m_pGame->GetBackgroundBitmapInfo());

    m_rect.left = width / 2 - 50;
    m_rect.top = height / 2 - 50;
    m_rect.right = m_rect.left + 100;
    m_rect.bottom = m_rect.top + 100;

    m_pBackground = pNewObject;
    //--
}

void PlayScene::FixedUpdate()
{
    assert(m_pGame != nullptr && "Game object is not initialized!");

    Vector2f enemyPos = m_pGame->EnemySpawnPosition();
    if (enemyPos.x != 0 && enemyPos.y != 0)
    {
        // 사실 큐가 너무 쓰고 싶었으나 참았음
        CreateEnemy();
        m_pGame->ResetEnemySpawnPosition();
    }
}

void PlayScene::Update(float deltaTime)
{
    UpdatePlayerInfo();

    UpdateEnemyInfo();

    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        if (m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i]->Update(deltaTime);
        }
    }

    if (m_CollisionCount >= 5)
    {
        m_pGame->ChangeScene(SceneType::SCENE_ENDING);
    }
}

void PlayScene::Render(HDC hDC)
{
    assert(m_pGame != nullptr && "Game object is not initialized!");

    m_pBackground->Render(hDC);
    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        if (m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i]->Render(hDC);
        }
    }
}

void PlayScene::Finalize()
{
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

      delete [] m_GameObjectPtrTable;
   }
}

void PlayScene::Enter()
{
    // [CHECK]. 첫 번째 게임 오브젝트는 플레이어 캐릭터로 고정!
    CreatePlayer();

}

void PlayScene::Leave()
{
}

void PlayScene::CreatePlayer()
{
    assert(m_GameObjectPtrTable[0] == nullptr && "Player object already exists!");
    assert(m_pGame != nullptr && "Game object is not initialized!");

    GameObject* pNewObject = new GameObject(ObjectType::PLAYER);

    pNewObject->SetName("Player");
    pNewObject->SetPosition(10.0f, 10.0f); // 일단, 임의로 설정 
    pNewObject->SetSpeed(0.5f); // 일단, 임의로 설정  
    pNewObject->SetWidth(100); // 일단, 임의로 설정
    pNewObject->SetHeight(100); // 일단, 임의로 설정

    pNewObject->SetBitmapInfo(m_pGame->GetPlayerBitmapInfo());
    pNewObject->SetColliderCircle(50.0f); // 일단, 임의로 설정. 오브젝트 설정할 거 다 하고 나서 하자.

    m_GameObjectPtrTable[0] = pNewObject;
}

void PlayScene::CreateEnemy()
{
    assert(m_pGame != nullptr && "Game object is not initialized!");

    GameObject* pNewObject = new GameObject(ObjectType::ENEMY);

    pNewObject->SetName("Enemy");

    Vector2f enemyPos = m_pGame->EnemySpawnPosition();

    pNewObject->SetPosition(enemyPos.x, enemyPos.y);

    pNewObject->SetSpeed(0.1f); // 일단, 임의로 설정  
    pNewObject->SetWidth(100); // 일단, 임의로 설정
    pNewObject->SetHeight(100); // 일단, 임의로 설정

    
    pNewObject->SetBitmapInfo(m_pGame->GetEnemyBitmapInfo()); //여기
    

    pNewObject->SetColliderCircle(50.0f); // 일단, 임의로 설정. 오브젝트 설정할 거 다 하고 나서 하자.

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

void PlayScene::UpdatePlayerInfo()
{
    static GameObject* pPlayer = GetPlayer();

    assert(pPlayer != nullptr);
    assert(m_pGame != nullptr && "MyFirstWndGame is null!");

    Vector2f targetPos = m_pGame->PlayerTargetPosition();
    Vector2f playerPos = pPlayer->GetPosition();

    Vector2f playerDir = targetPos - playerPos;
    float distance = playerDir.Length(); // 거리 계산

    if (distance > 50.f) //임의로 설정한 거리
    {
        playerDir.Normalize(); // 정규화
        pPlayer->SetDirection(playerDir); // 플레이어 방향 설정
    }
    else
    {
        pPlayer->SetDirection(Vector2f(0, 0)); // 플레이어 정지
    }
}

void PlayScene::UpdateEnemyInfo()
{
    static GameObject* pPlayer = GetPlayer();
    assert(pPlayer != nullptr);

    Vector2f playerPos = GetPlayer()->GetPosition();
    Vector2f enemyPosArr[MAX_GAME_OBJECT_COUNT];
    static bool hasCollidedBefore[MAX_GAME_OBJECT_COUNT][MAX_GAME_OBJECT_COUNT] = { false };

    for (int i = 1; i < MAX_GAME_OBJECT_COUNT; ++i) //0번째는 언제나 플레이어!
    {
        if (m_GameObjectPtrTable[i] != nullptr)
        {
            GameObject* pEnemy = static_cast<GameObject*>(m_GameObjectPtrTable[i]);
            
            Vector2f enemyPos = pEnemy->GetPosition();
            enemyPosArr[i] = enemyPos;

            Vector2f enemyDir = playerPos - enemyPos;
            float distance = enemyDir.Length(); // 거리 계산

            if (distance > 50.f) //임의로 설정한 거리
            {
                enemyDir.Normalize(); // 정규화
                pEnemy->SetDirection(enemyDir); // 방향 설정
            }
            else
            {
                pEnemy->SetDirection(Vector2f(0, 0)); //  정지
            }
            

            for (int j = i + 1; j < MAX_GAME_OBJECT_COUNT; ++j) //0번째는 언제나 플레이어!
            {
                if (m_GameObjectPtrTable[j] != nullptr)
                {
                    Vector2f otherEnemyPos = static_cast<GameObject*>(m_GameObjectPtrTable[j])->GetPosition();
                   
                    const float COLLISON_THRESHOLD = 5.0f;
                    Vector2f diff = enemyPos - otherEnemyPos;
                    bool isColliding = (diff.Length() < COLLISON_THRESHOLD);

                    if (isColliding && !hasCollidedBefore[i][j]) {
                        m_CollisionCount++;

                        hasCollidedBefore[i][j] = true;
                        std::cout << "Enemy " << i << " and Enemy " << j << " collided! Total: " << m_CollisionCount << std::endl;
                    }
                                        
                    else if (!isColliding && hasCollidedBefore[i][j]) {// 아직은 안씀 충돌 해제를 위한 거리 임계값 (충돌 임계값보다 커야 함)
                        const float RESET_THRESHOLD = 20.0f;
                        if (diff.Length() > RESET_THRESHOLD)
                        {
                            hasCollidedBefore[i][j] = false;
                        }
                        
                    }
                }
            }

            
        }


    }
}

