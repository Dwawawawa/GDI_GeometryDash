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
		std::cout << "��õ� Ƚ��: " << m_retryCount << std::endl;
		m_pGame->ChangeScene(SceneType::SCENE_PLAY);
        return;
	}
    else if (m_bIsEnd) {
        std::cout << "�� ��õ� Ƚ��: " << m_retryCount << std::endl;
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

    //=====�� ��ũ�Ѹ��� �ٸ� �Լ��� ���߰ڴ�.====
    static bool isDelete = false;
    // �� ��ũ�Ѹ� �ӵ� ����
    const float scrollSpeed = 0.8f;  // �ȼ�/�и���
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
	//// ������ ���� ǥ���� �׸���
	//int width = m_pGame->GetWidth();
	//int totalLength = 13000; // Stereo Madness �� ����
	//int progressWidth = (int)((float)m_levelPosition / totalLength * 300);

	//// ���� ǥ���� ���
	//HBRUSH hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
	//HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBlackBrush);
	//Rectangle(hDC, width - 320, 20, width - 20, 30);

	//// ���� ǥ���� ����
	//HBRUSH hGreenBrush = CreateSolidBrush(RGB(0, 255, 0));
	//SelectObject(hDC, hGreenBrush);
	//Rectangle(hDC, width - 320, 20, width - 320 + progressWidth, 30);

	//// �귯�� ����
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
	// ��� ī��Ʈ�� �ε��� �ʱ�ȭ
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


	//��� ����
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
    // ��� ���� ������Ʈ ����
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
    
    // �÷��̾� ��ġ�� �� ���� ����
    int height = m_pGame->GetHeight();
    pNewObject->SetPosition(200.0f, height-500.0f); 

    pNewObject->SetSpeed(0.0f); // ������Ʈ�� �뽬���� �����ۿ� ����.
    
    // ���� �̹��� ũ�⿡ �°� ����
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
	float baseX = 0.0f; // ���� X ��ġ

	// ���� 1: �⺻ Ʃ�丮�� ��Ʈ (������ ��ϰ� ������ũ)

	// �⺻ �ٴ� ��� - ���߿� ��(�׶���)���� ��ü
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
    
	// ������Ʈ ��� ī��Ʈ
	m_blockCount = index - m_blockStartIndex;
}

void PlayScene::CreateMapSection2() {
	int height = m_pGame->GetHeight();
	int index = m_blockStartIndex + m_blockCount;
	float baseX = 0.0f + (45 * 122); // ���� 1 ���� ������
    //float baseX = 0.0f; //test

	// ���� 2: �÷��� ���� ����
	for (int i = 0; i < 65; i++) {
		CreateBlock(baseX + (i * 122), height - 61.0f, index++);
	}

    // 2-1
	CreateSpike(baseX + (6 * 122), height - 61.0f - 122.0f, index++);
	CreateSpike(baseX + (7 * 122), height - 61.0f - 122.0f, index++);

    //2-2
    //2-2�� 2�� ���
	for (int i = 14; i < 27; i++) {
		CreateBlock2(baseX + (i * 122), height - 61.0f - 122.0f, index++);
	}
    CreateSpikeBed(baseX + (28 * 122), height - 20.0f - 122.0f, index++);
    

    //2-3
    //2-3�� 2�� ���
	for (int i = 30; i < 45; i++) {
		CreateBlock2(baseX + (i * 122), height - 61.0f - 122.0f, index++);
	}
    CreateSpikeBed(baseX + (46 * 122), height - 20.0f - 122.0f, index++);
    CreateSpike(baseX + (37 * 122), height - 61.0f - 122.0f * 2 , index++);


    //2-4
	//2-4�� 2,3�� ���
	for (int i = 48; i < 63; i++) {
		CreateBlock(baseX + (i * 122), height - 61.0f - 122.0f, index++);
        CreateBlock2(baseX + (i * 122), height - 61.0f - 122.0f - 122.0f, index++);
	}
    CreateSpike(baseX + (55 * 122), height - 61.0f - 122.0f * 3, index++);

	// ������Ʈ ��� ī��Ʈ
	m_blockCount = index - m_blockStartIndex;
}

void PlayScene::CreateMapSection3() {
	int height = m_pGame->GetHeight();
	int index = m_blockStartIndex + m_blockCount;
	float baseX = 10.0f + (22 * 122) + (21 * 122); // ���� 2 ���� ������

	// ���� 3: ������ ���� ���ϰ� ������ũ ����

	// ���2 ���� (�ٸ� �ؽ�ó�� ���)
	CreateBlock2(baseX, height - 61.0f, index++);
	CreateBlock2(baseX + (1 * 122), height - 61.0f, index++);

	// ������ũ �� �÷��� ����
	CreateSpike(baseX + (2 * 122), height - 91.0f, index++);
	CreatePlatform(baseX + (3 * 122), height - 150.0f, index++);
	CreateSpike(baseX + (5 * 122), height - 180.0f, index++); // ���� ��ġ�� ������ũ
	CreateBlock2(baseX + (6 * 122), height - 61.0f, index++);

	// ���� ���� ����
	for (int i = 0; i < 3; i++) {
		float x = baseX + ((7 + i * 3) * 122);
		CreateBlock2(x, height - 61.0f, index++);
		CreatePlatform(x + (1 * 122), height - 150.0f, index++);
		CreateSpike(x + (2 * 122), height - 180.0f, index++);
	}

	// ������ �κ� - ������ũ����� �÷��� ����
	CreateBlock2(baseX + (16 * 122), height - 61.0f, index++);
	CreateSpikeBed(baseX + (17 * 122), height - 30.0f, index++);
	CreatePlatform(baseX + (20 * 122), height - 150.0f, index++);
	CreateBlock2(baseX + (22 * 122), height - 61.0f, index++);

	// ������Ʈ ��� ī��Ʈ
	m_blockCount = index - m_blockStartIndex;
}

void PlayScene::CreateMapSection4() {
	int height = m_pGame->GetHeight();
	int index = m_blockStartIndex + m_blockCount;
	float baseX = 10.0f + (22 * 122) + (21 * 122) + (23 * 122); // ���� 3 ���� ������

	// ���� 4: ������ ���� ���� �� ��ǥ

	// ������ ������ũ���� ����
	CreateBlock2(baseX, height - 61.0f, index++);
	CreateSpikeBed(baseX + (1 * 122), height - 30.0f, index++);
	CreateBlock2(baseX + (5 * 122), height - 61.0f, index++);

	// ������ ���� ����
	CreatePlatform(baseX + (6 * 122), height - 150.0f, index++);
	CreateSpike(baseX + (8 * 122), height - 180.0f, index++);
	CreateBlock2(baseX + (9 * 122), height - 61.0f, index++);

	// ��ǥ �������� �̾����� ��
	for (int i = 0; i < 5; i++) {
		CreateBlock2(baseX + ((10 + i) * 122), height - 61.0f, index++);
	}

	// ��ǥ ������Ʈ
	CreateGoal(baseX + (15 * 122), height - 91.0f, index++);

	// ������Ʈ ��� ī��Ʈ
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
                        //������ �����ϴٰ��!

                        if (playerBottom >= blockTop && pPlayer->GetVerticalVelocity() > 0)
                        {
                            // �÷��̾ ��� ���� ��ġ ����
                            pPlayer->SetPosition(pPlayer->GetPosition().x, blockTop - pPlayer->GetColliderBox()->halfSize.y);
                            isGrounded = true;
                        }
                        break;

                    case ObjectType::SPIKE:
                    case ObjectType::SPIKEBED:
                        // ���ÿ� ������ ��� ó��
						m_bIsDead = true;
                        break;

                    case ObjectType::GOAL:
                        // �� ���� - ���� Ŭ����
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

	// ȭ�� �Ʒ��� �������� �� ��� ó��
	if (pPlayer->GetPosition().y > m_pGame->GetHeight() + 200) {
		m_bIsDead = true;
	}

	pPlayer->SetGrounded(isGrounded);

	// Ű���� �Է� ó��
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) // �����̽��� ���� ����
	{
		pPlayer->Jump();
	}

}
