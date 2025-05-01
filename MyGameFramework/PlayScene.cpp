#include "PlayScene.h"
#include "MyFirstWndGame.h"
#include "GameObject.h"
#include "Utillity.h"
#include "Collider.h"
#include "MapGenerator.h"
#include <cassert>
#include <iostream>
#include <ctime>

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
		m_pGame->SetRetryCount(m_retryCount);
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

	if (isDelete)
	{
		isDelete = false;
	}

	m_levelPosition += 1;
	//std::cout << m_levelPosition << std::endl;
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
	// 간단한 진행 표시줄 그리기
	int width = m_pGame->GetWidth();
	int totalLength = 3266; // Stereo Madness 맵 길이
	int progressWidth = (int)((float)m_levelPosition / totalLength * 300);

	// 진행 표시줄 배경
	HBRUSH hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBlackBrush);
	Rectangle(hDC, width - 320, 20, width - 20, 30);

	// 재시도 횟수 출력
	WCHAR szRetryCount[32];
	swprintf_s(szRetryCount, L"재시도: %d", m_retryCount);
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC, width - 320, 40, szRetryCount, lstrlen(szRetryCount));

	// 진행 표시줄 내용
	HBRUSH hGreenBrush = CreateSolidBrush(RGB(0, 255, 0));
	SelectObject(hDC, hGreenBrush);
	Rectangle(hDC, width - 320, 20, width - 320 + progressWidth, 30);

	// 브러시 정리
	SelectObject(hDC, hOldBrush);
	DeleteObject(hBlackBrush);
	DeleteObject(hGreenBrush);
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

	if (m_pMapGenerator)
	{
		delete m_pMapGenerator;
		m_pMapGenerator = nullptr;
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

	// 게임 오브젝트 배열 초기화
	m_GameObjectPtrTable = new GameObjectBase * [MAX_GAME_OBJECT_COUNT];

	for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
	{
		m_GameObjectPtrTable[i] = nullptr;
	}

	// 배경 설정
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

	// 플레이어 생성
	CreatePlayer();

	// 맵 생성기 초기화 및 맵 생성
	m_pMapGenerator = new MapGenerator(this, m_pGame);
	m_pMapGenerator->CreateStereoMadnessMap();
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

	if (m_pGround) {
		delete m_pGround;
		m_pGround = nullptr;
	}

	if (m_pBackground)
	{
		delete m_pBackground;
		m_pBackground = nullptr;
	}

	if (m_pMapGenerator)
	{
		delete m_pMapGenerator;
		m_pMapGenerator = nullptr;
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
	pNewObject->SetPosition(200.0f, height - 500.0f);

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

	pNewObject->SetColliderTriangle(120, 120);

	m_GameObjectPtrTable[index] = pNewObject;
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
}

void PlayScene::UpdatePlayerInfo()
{
	GameObject* pPlayer = GetPlayer();

	assert(pPlayer != nullptr);
	assert(m_pGame != nullptr && "MyFirstWndGame is null!");

	bool isGrounded = false;


	for (int i = m_blockStartIndex; i < m_blockStartIndex + m_blockCount; i++)
	{
		GameObject* pObject = static_cast<GameObject*>(m_GameObjectPtrTable[i]);
		if (pObject)
		{
			if (pPlayer->GetColliderBox() && pObject->GetColliderBox())
			{
				if (learning::Intersect(*pPlayer->GetColliderBox(), *pObject->GetColliderBox()))
				{
					// 원래는 플레이어 아래와 블록 톱만 비교했었는데...
					float playerBottom = pPlayer->GetPosition().y + pPlayer->GetColliderBox()->halfSize.y;
					float blockTop = pObject->GetPosition().y - pObject->GetColliderBox()->halfSize.y;
					// 이제는 나머지도 비교를 합니다.
					float playerTop = pPlayer->GetPosition().y - pPlayer->GetColliderBox()->halfSize.y;
					float playerLeft = pPlayer->GetPosition().x - pPlayer->GetColliderBox()->halfSize.x;
					float playerRight = pPlayer->GetPosition().x + pPlayer->GetColliderBox()->halfSize.x;
					float blockLeft = pObject->GetPosition().x - pObject->GetColliderBox()->halfSize.x;
					float blockRight = pObject->GetPosition().x + pObject->GetColliderBox()->halfSize.x;

					// 충돌 방향 결정 (상단 충돌 vs 측면 충돌)
					// 상단 충돌은 플레이어가 블록 위로 떨어질 때만 발생
					bool topCollision = playerBottom + 20 >= blockTop 
						&& playerBottom <= blockTop + 40 
						//&& pPlayer->GetVerticalVelocity() > 0
						; // 아래로 떨어지고 있어야 함


					switch (pObject->Type())
					{
					case ObjectType::BLOCK1:
					case ObjectType::BLOCK2:
					case ObjectType::PLATFORM:
						//착지가 가능하다고요!
						if (topCollision) {
							// 상단 충돌 - 플레이어를 블록 위로 배치
							pPlayer->SetPosition(pPlayer->GetPosition().x, blockTop - pPlayer->GetColliderBox()->halfSize.y);
							isGrounded = true;
							
						}
						else {
							// 측면 충돌 - 사망 처리
							m_bIsDead = true;
							std::cout << "블록 옆" << std::endl;
						}
						break;

					case ObjectType::SPIKEBED:
						// 가시에 닿으면 사망 처리
						m_bIsDead = true;
						std::cout << "침대 죽음" << std::endl;
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
						std::cout << "가시 죽음" << std::endl;
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