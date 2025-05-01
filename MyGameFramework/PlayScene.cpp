#include "PlayScene.h"
#include "MyFirstWndGame.h"
#include "GameObject.h"
#include "Utillity.h"
#include "Collider.h"
#include <assert.h>
#include <iostream>
#include <time.h>

using namespace learning;

constexpr int MAX_GAME_OBJECT_COUNT = 1000;

void PlayScene::Initialize(NzWndBase* pWnd)
{
    srand(static_cast<unsigned int>(time(nullptr)));

    m_pGame = dynamic_cast<MyFirstWndGame*>(pWnd);
    assert(m_pGame != nullptr);
}

void PlayScene::FixedUpdate()
{
  
}

void PlayScene::Update(float deltaTime)
{
    UpdatePlayerInfo();

	if (m_bIsDead) {
		m_retryCount++;
		std::cout << "재시도 횟수: " << m_retryCount << std::endl;
		m_pGame->ChangeScene(SceneType::SCENE_PLAY);
        return;
	}
    else if (m_bIsEnd) {
        std::cout << "총 재시도 횟수: " << m_retryCount << std::endl;
		m_pGame->ChangeScene(SceneType::SCENE_ENDING);
		return;
    }


    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        if (m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i]->Update(deltaTime);
        }
    }

    //=====맵 스크롤링은 다른 함수로 뺴야겠다.====
    static bool isDelete = false;
    // 맵 스크롤링 속도 설정
    const float scrollSpeed = 0.8f;  // 픽셀/밀리초
    for (int i = m_blockStartIndex; i < m_blockStartIndex + m_blockCount; i++)
    {
        if (m_GameObjectPtrTable[i]) {
            Vector2f pos = m_GameObjectPtrTable[i]->GetPosition();
            pos.x -= scrollSpeed * deltaTime;
            m_GameObjectPtrTable[i]->SetPosition(pos.x, pos.y);

            if (pos.x < -100) {
                delete m_GameObjectPtrTable[i];
                m_GameObjectPtrTable[i] = nullptr;
                isDelete = true;
            }
        }
    }

    if(isDelete)
	{
		
        isDelete = false;
	}

	m_levelPosition += 1;

}

void PlayScene::Render(HDC hDC)
{
    assert(m_pGame != nullptr && "Game object is not initialized!");

    m_pBackground->Render(hDC);

    if (m_pGround) {
        m_pGround->Render(hDC);
    }


    for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
    {
        if (m_GameObjectPtrTable[i])
        {
            m_GameObjectPtrTable[i]->Render(hDC);
        }
    }

#pragma region progressbar
	//// 간단한 진행 표시줄 그리기
	//int width = m_pGame->GetWidth();
	//int totalLength = 13000; // Stereo Madness 맵 길이
	//int progressWidth = (int)((float)m_levelPosition / totalLength * 300);

	//// 진행 표시줄 배경
	//HBRUSH hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
	//HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBlackBrush);
	//Rectangle(hDC, width - 320, 20, width - 20, 30);

	//// 진행 표시줄 내용
	//HBRUSH hGreenBrush = CreateSolidBrush(RGB(0, 255, 0));
	//SelectObject(hDC, hGreenBrush);
	//Rectangle(hDC, width - 320, 20, width - 320 + progressWidth, 30);

	//// 브러시 정리
	//SelectObject(hDC, hOldBrush);
	//DeleteObject(hBlackBrush);
	//DeleteObject(hGreenBrush);
#pragma endregion
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

		delete[] m_GameObjectPtrTable;
        m_GameObjectPtrTable = nullptr;
	}

	if (m_pGround) {
		delete m_pGround;
		m_pGround = nullptr;
	}

	if (m_pBackground)
	{
		delete m_pBackground;
		m_pBackground = nullptr;
	}
   
}

void PlayScene::Enter()
{
	// 블록 카운트와 인덱스 초기화
    m_bIsDead = false;
    m_blockStartIndex = 1;
	m_blockCount = 0;
    m_levelPosition = 0;
    m_bIsEnd = false;

    //
    m_GameObjectPtrTable = new GameObjectBase * [MAX_GAME_OBJECT_COUNT];

	for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
	{
		m_GameObjectPtrTable[i] = nullptr;
	}


	//배경 설정
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


    CreatePlayer();
	CreateStereoMadnessMap();
}

void PlayScene::Leave()
{
    // 모든 게임 오브젝트 삭제
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
		m_GameObjectPtrTable = nullptr;
    }

	//delete[] m_GameObjectPtrTable;
	//m_GameObjectPtrTable = nullptr;

	if (m_pGround) {
		delete m_pGround;
		m_pGround = nullptr;
	}

	if (m_pBackground)
	{
		delete m_pBackground;
		m_pBackground = nullptr;
	}
}

void PlayScene::CreatePlayer()
{
    assert(m_GameObjectPtrTable[0] == nullptr && "Player object already exists!");
    assert(m_pGame != nullptr && "Game object is not initialized!");

    GameObject* pNewObject = new GameObject(ObjectType::PLAYER);

    pNewObject->SetName("Player");
    
    // 플레이어 위치를 땅 위에 설정
    int height = m_pGame->GetHeight();
    pNewObject->SetPosition(200.0f, height-500.0f); 

    pNewObject->SetSpeed(0.0f); // 지오메트리 대쉬에선 점프밖에 안해.
    
    // 실제 이미지 크기에 맞게 설정
    pNewObject->SetWidth(120); 
    pNewObject->SetHeight(120);

    pNewObject->SetBitmapInfo(m_pGame->GetPlayerBitmapInfo());
    pNewObject->SetColliderBox(120.0f, 120.0f);

    m_GameObjectPtrTable[0] = pNewObject;
}


void PlayScene::CreateBlock(float x, float y, int index)
{
	assert(index < MAX_GAME_OBJECT_COUNT && "Index out of bounds");
	assert(m_GameObjectPtrTable[index] == nullptr && "Object Block1 already exists at this index");

    GameObject* pNewObject = new GameObject(ObjectType::BLOCK1);

    pNewObject->SetName("Block1");
    pNewObject->SetPosition(x, y);

    pNewObject->SetWidth(122);
    pNewObject->SetHeight(122);

    pNewObject->SetBitmapInfo(m_pGame->GetBlock1BitmapInfo());

    pNewObject->SetColliderBox(122.0f, 122.0f);

    m_GameObjectPtrTable[index] = pNewObject;
    m_blockCount++;
}

void PlayScene::CreateSpike(float x, float y, int index)
{
    assert(index < MAX_GAME_OBJECT_COUNT && "Index out of bounds");
    assert(m_GameObjectPtrTable[index] == nullptr && "Object Spike already exists at this index");

    GameObject* pNewObject = new GameObject(ObjectType::SPIKE);

    pNewObject->SetName("Spike");
    pNewObject->SetPosition(x, y);

    pNewObject->SetWidth(122);
    pNewObject->SetHeight(122);

    pNewObject->SetBitmapInfo(m_pGame->GetSpikeBitmapInfo());

    pNewObject->SetColliderTriangle(122, 122);

    m_GameObjectPtrTable[index] = pNewObject;
    m_blockCount++;
}

void PlayScene::CreateMiniSpike(float x, float y, int index)
{
	assert(index < MAX_GAME_OBJECT_COUNT && "Index out of bounds");
	assert(m_GameObjectPtrTable[index] == nullptr && "Object MiniSpike already exists at this index");

	GameObject* pNewObject = new GameObject(ObjectType::MINISPKIE);

	pNewObject->SetName("MiniSpike");
	pNewObject->SetPosition(x, y);

	pNewObject->SetWidth(122);
	pNewObject->SetHeight(61);

	pNewObject->SetBitmapInfo(m_pGame->GetSpikeBitmapInfo());

	pNewObject->SetColliderTriangle(122, 61);

	m_GameObjectPtrTable[index] = pNewObject;
	m_blockCount++;
}

void PlayScene::CreateBlock2(float x, float y, int index)
{
    assert(index < MAX_GAME_OBJECT_COUNT && "Index out of bounds");
    assert(m_GameObjectPtrTable[index] == nullptr && "Objectlock2 already exists at this index");

    GameObject* pNewObject = new GameObject(ObjectType::BLOCK2);

    pNewObject->SetName("Block2");
    pNewObject->SetPosition(x, y);

    pNewObject->SetWidth(122);
    pNewObject->SetHeight(122);

    pNewObject->SetBitmapInfo(m_pGame->GetBlock2BitmapInfo());

    pNewObject->SetColliderBox(122.0f, 122.0f);

    m_GameObjectPtrTable[index] = pNewObject;
    m_blockCount++;
}

void PlayScene::CreatePlatform(float x, float y, int index)
{
    assert(index < MAX_GAME_OBJECT_COUNT && "Index out of bounds");
    assert(m_GameObjectPtrTable[index] == nullptr && "Object already exists at this index");

    GameObject* pNewObject = new GameObject(ObjectType::PLATFORM);

    pNewObject->SetName("Platform");
    pNewObject->SetPosition(x, y);

    pNewObject->SetWidth(122);
    pNewObject->SetHeight(56);

    pNewObject->SetBitmapInfo(m_pGame->GetPlatformBitmapInfo());

    pNewObject->SetColliderBox(122.0f, 56.0f);

    m_GameObjectPtrTable[index] = pNewObject;
    m_blockCount++;
}

void PlayScene::CreateSpikeBed(float x, float y, int index)
{
    assert(index < MAX_GAME_OBJECT_COUNT && "Index out of bounds");
    assert(m_GameObjectPtrTable[index] == nullptr && "Object already exists at this index");

    GameObject* pNewObject = new GameObject(ObjectType::SPIKEBED);

    pNewObject->SetName("SpikeBed");
    pNewObject->SetPosition(x, y);

    pNewObject->SetWidth(366); // 122*3 
    pNewObject->SetHeight(40);

    pNewObject->SetBitmapInfo(m_pGame->GetSpikeBedBitmapInfo());

    pNewObject->SetColliderBox(366.0f, 40.0f);

    m_GameObjectPtrTable[index] = pNewObject;
    m_blockCount++;
}

void PlayScene::CreateGoal(float x, float y, int index)
{
    assert(index < MAX_GAME_OBJECT_COUNT && "Index out of bounds");
    assert(m_GameObjectPtrTable[index] == nullptr && "Object already exists at this index");

    GameObject* pNewObject = new GameObject(ObjectType::GOAL);

    pNewObject->SetName("Goal");
    pNewObject->SetPosition(x, y);

    pNewObject->SetWidth(160);
    pNewObject->SetHeight(160);

    pNewObject->SetBitmapInfo(m_pGame->GetGoalBitmapInfo());

    pNewObject->SetColliderBox(160.0f, 160.0f);

    m_GameObjectPtrTable[index] = pNewObject;
    m_blockCount++;
}


void PlayScene::CreateStereoMadnessMap() {

	CreateMapSection1();
	CreateMapSection2();
	//CreateMapSection3();
	//CreateMapSection4();
}

void PlayScene::CreateMapSection1() {
	int height = m_pGame->GetHeight();
	int index = m_blockStartIndex + m_blockCount;
	float baseX = 0.0f; // 시작 X 위치

	// 섹션 1: 기본 튜토리얼 파트 (간단한 블록과 스파이크)

	// 기본 바닥 블록 - 나중에 땅(그라운드)으로 교체
	for (int i = 0; i < 45; i++) {
		CreateBlock(baseX + (i * 122), height - 61.0f , index++);
	}

	// 1-1
	CreateSpike(baseX + (10 * 122), height - 61.0f - 122.0f, index++);
    CreateSpike(baseX + (11 * 122), height - 61.0f - 122.0f, index++);
    CreateSpike(baseX + (12 * 122), height - 61.0f - 122.0f, index++);

    // 1-2
    CreateMiniSpike(baseX + (19 * 122), height - 30.5f - 122.0f, index++);
    CreateSpike(baseX + (20 * 122), height - 61.0f - 122.0f, index++);

    // 1-3
    CreateSpike(baseX + (29 * 122), height - 61.0f - 122.0f, index++);
    CreateSpike(baseX + (30 * 122), height - 61.0f - 122.0f, index++);
    CreateBlock2(baseX + (31 * 122), height - 61.0f - 122.0f, index++);
    CreateSpikeBed(baseX + (33 * 122), height - 20.0f -122.0f, index++);
    CreateBlock(baseX + (35 * 122), height - 61.0f - 122.0f, index++);
    CreateBlock2(baseX + (35 * 122), height - 61.0f - 122.0f - 122.0f, index++);
	CreateSpikeBed(baseX + (37 * 122), height - 20.0f - 122.0f, index++);
	CreateBlock(baseX + (39 * 122), height - 61.0f - 122.0f, index++);
	CreateBlock(baseX + (39 * 122), height - 61.0f - 122.0f - 122.0f, index++);
    CreateBlock2(baseX + (39 * 122), height - 61.0f - 122.0f - 122.0f - 122.0f, index++);
    
	// 업데이트 블록 카운트
	m_blockCount = index - m_blockStartIndex;
}

void PlayScene::CreateMapSection2() {
	int height = m_pGame->GetHeight();
	int index = m_blockStartIndex + m_blockCount;
	float baseX = 0.0f + (45 * 122); // 섹션 1 이후 시작점
    //float baseX = 0.0f; //test

	// 섹션 2: 플랫폼 점프 섹션
	for (int i = 0; i < 65; i++) {
		CreateBlock(baseX + (i * 122), height - 61.0f, index++);
	}

    // 2-1
	CreateSpike(baseX + (6 * 122), height - 61.0f - 122.0f, index++);
	CreateSpike(baseX + (7 * 122), height - 61.0f - 122.0f, index++);

    //2-2
    //2-2의 2층 블록
	for (int i = 14; i < 27; i++) {
		CreateBlock2(baseX + (i * 122), height - 61.0f - 122.0f, index++);
	}
    CreateSpikeBed(baseX + (28 * 122), height - 20.0f - 122.0f, index++);
    

    //2-3
    //2-3의 2층 블록
	for (int i = 30; i < 45; i++) {
		CreateBlock2(baseX + (i * 122), height - 61.0f - 122.0f, index++);
	}
    CreateSpikeBed(baseX + (46 * 122), height - 20.0f - 122.0f, index++);
    CreateSpike(baseX + (37 * 122), height - 61.0f - 122.0f * 2 , index++);


    //2-4
	//2-4의 2,3층 블록
	for (int i = 48; i < 63; i++) {
		CreateBlock(baseX + (i * 122), height - 61.0f - 122.0f, index++);
        CreateBlock2(baseX + (i * 122), height - 61.0f - 122.0f - 122.0f, index++);
	}
    CreateSpike(baseX + (55 * 122), height - 61.0f - 122.0f * 3, index++);

	// 업데이트 블록 카운트
	m_blockCount = index - m_blockStartIndex;
}

void PlayScene::CreateMapSection3() {
	int height = m_pGame->GetHeight();
	int index = m_blockStartIndex + m_blockCount;
	float baseX = 10.0f + (22 * 122) + (21 * 122); // 섹션 2 이후 시작점

	// 섹션 3: 복잡한 점프 패턴과 스파이크 조합

	// 블록2 패턴 (다른 텍스처의 블록)
	CreateBlock2(baseX, height - 61.0f, index++);
	CreateBlock2(baseX + (1 * 122), height - 61.0f, index++);

	// 스파이크 및 플랫폼 조합
	CreateSpike(baseX + (2 * 122), height - 91.0f, index++);
	CreatePlatform(baseX + (3 * 122), height - 150.0f, index++);
	CreateSpike(baseX + (5 * 122), height - 180.0f, index++); // 높은 위치의 스파이크
	CreateBlock2(baseX + (6 * 122), height - 61.0f, index++);

	// 연속 점프 구간
	for (int i = 0; i < 3; i++) {
		float x = baseX + ((7 + i * 3) * 122);
		CreateBlock2(x, height - 61.0f, index++);
		CreatePlatform(x + (1 * 122), height - 150.0f, index++);
		CreateSpike(x + (2 * 122), height - 180.0f, index++);
	}

	// 마지막 부분 - 스파이크베드와 플랫폼 조합
	CreateBlock2(baseX + (16 * 122), height - 61.0f, index++);
	CreateSpikeBed(baseX + (17 * 122), height - 30.0f, index++);
	CreatePlatform(baseX + (20 * 122), height - 150.0f, index++);
	CreateBlock2(baseX + (22 * 122), height - 61.0f, index++);

	// 업데이트 블록 카운트
	m_blockCount = index - m_blockStartIndex;
}

void PlayScene::CreateMapSection4() {
	int height = m_pGame->GetHeight();
	int index = m_blockStartIndex + m_blockCount;
	float baseX = 10.0f + (22 * 122) + (21 * 122) + (23 * 122); // 섹션 3 이후 시작점

	// 섹션 4: 마지막 도전 구간 및 목표

	// 마지막 스파이크베드 구간
	CreateBlock2(baseX, height - 61.0f, index++);
	CreateSpikeBed(baseX + (1 * 122), height - 30.0f, index++);
	CreateBlock2(baseX + (5 * 122), height - 61.0f, index++);

	// 마지막 점프 도전
	CreatePlatform(baseX + (6 * 122), height - 150.0f, index++);
	CreateSpike(baseX + (8 * 122), height - 180.0f, index++);
	CreateBlock2(baseX + (9 * 122), height - 61.0f, index++);

	// 목표 지점으로 이어지는 길
	for (int i = 0; i < 5; i++) {
		CreateBlock2(baseX + ((10 + i) * 122), height - 61.0f, index++);
	}

	// 목표 오브젝트
	CreateGoal(baseX + (15 * 122), height - 91.0f, index++);

	// 업데이트 블록 카운트
	m_blockCount = index - m_blockStartIndex;
}

void PlayScene::UpdatePlayerInfo()
{
    GameObject* pPlayer = GetPlayer();

    assert(pPlayer != nullptr);
    assert(m_pGame != nullptr && "MyFirstWndGame is null!");

    bool isGrounded = false;
    

    for (int i = m_blockStartIndex; i <m_blockStartIndex + m_blockCount; i++)
    {
        GameObject* pObject = static_cast<GameObject*>(m_GameObjectPtrTable[i]);
        if (pObject) 
        {
            if (pPlayer->GetColliderBox() && pObject->GetColliderBox())
            {
                if (learning::Intersect(*pPlayer->GetColliderBox(), *pObject->GetColliderBox()))
                {
                    float playerBottom = pPlayer->GetPosition().y + pPlayer->GetColliderBox()->halfSize.y;
                    float blockTop = pObject->GetPosition().y - pObject->GetColliderBox()->halfSize.y;

                    switch (pObject->Type())
                    {
                    case ObjectType::BLOCK1:
                    case ObjectType::BLOCK2:
                    case ObjectType::PLATFORM:
                        //착지가 가능하다고요!

                        if (playerBottom >= blockTop && pPlayer->GetVerticalVelocity() > 0)
                        {
                            // 플레이어를 블록 위로 위치 조정
                            pPlayer->SetPosition(pPlayer->GetPosition().x, blockTop - pPlayer->GetColliderBox()->halfSize.y);
                            isGrounded = true;
                        }
                        break;

                    case ObjectType::SPIKE:
                    case ObjectType::SPIKEBED:
                        // 가시에 닿으면 사망 처리
						m_bIsDead = true;
                        break;

                    case ObjectType::GOAL:
                        // 골 도달 - 게임 클리어
                        m_bIsEnd = true;
                        break;
                    }
                }
            }
            else if (pPlayer->GetColliderBox() && pObject->GetColliderTriangle())
            {
                if (learning::Intersect(*pPlayer->GetColliderBox(), *pObject->GetColliderTriangle()))
                {
                    switch (pObject->Type())
                    {
                    case ObjectType::SPIKE:
                    case ObjectType::MINISPKIE:
                        m_bIsDead = true;
                        break;
                    }
                }
            }
        }

    }

	// 화면 아래로 떨어졌을 때 사망 처리
	if (pPlayer->GetPosition().y > m_pGame->GetHeight() + 200) {
		m_bIsDead = true;
	}

	pPlayer->SetGrounded(isGrounded);

	// 키보드 입력 처리
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) // 스페이스바 누름 감지
	{
		pPlayer->Jump();
	}

}
