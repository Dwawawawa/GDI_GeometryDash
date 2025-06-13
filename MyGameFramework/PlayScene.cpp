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

constexpr int MAX_GAME_OBJECT_COUNT = 1210;

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
		std::cout << "�õ� Ƚ��: " << m_retryCount << std::endl;
		m_pGame->ChangeScene(SceneType::SCENE_PLAY);
		return;
	}
	else if (m_bIsEnd) {
		std::cout << "�� �õ� Ƚ��: " << m_retryCount << std::endl;
		m_pGame->SetRetryCount(m_retryCount);
		m_pGame->SetCoinCount(m_coinCount);
		m_pGame->ChangeScene(SceneType::SCENE_ENDING);
		return;
	}

	// ���⼭ �� �Ÿ� ��� ���� ���� �߰�
	if (m_Fdinstance >= totalLength) {
		m_bIsEnd = true;
		std::cout << "�Ÿ� ����! ���� Ŭ����!" << std::endl;
		m_pGame->SetRetryCount(m_retryCount);
		m_pGame->SetCoinCount(m_coinCount);
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

	static bool isDelete = false;
	// �� ��ũ�Ѹ� �ӵ� ����
	const float scrollSpeed = 0.8f;  // �ȼ�/�и���

	m_Fdinstance += scrollSpeed * deltaTime;

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
	// ������ ���� ǥ���� �׸���
	int width = m_pGame->GetWidth();
	int progressWidth = static_cast<int>((m_Fdinstance / totalLength )* 300);

	// ���� ǥ���� ���
	HBRUSH hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBlackBrush);
	Rectangle(hDC, width - 320, 20, width - 20, 30);

	// ��õ� Ƚ�� ���
	WCHAR szRetryCount[32];
	swprintf_s(szRetryCount, L"�õ� Ƚ��: %d", m_retryCount);
	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);
	TextOut(hDC, width - 320, 40, szRetryCount, lstrlen(szRetryCount));
	 
	// ȹ�� ���� Ƚ�� ���
	WCHAR szCoinCount[32];
	swprintf_s(szCoinCount, L"ȹ���� ����: %d / 3", m_coinCount);
	TextOut(hDC, width - 320, 60, szCoinCount, lstrlen(szCoinCount));


	// ���� ǥ���� ����
	HBRUSH hGreenBrush = CreateSolidBrush(RGB(0, 255, 0));
	SelectObject(hDC, hGreenBrush);
	Rectangle(hDC, width - 320, 20, width - 320 + progressWidth, 30);

	// �귯�� ����
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
	// ��� ī��Ʈ�� �ε��� �ʱ�ȭ
	m_bIsDead = false;
	m_Fdinstance = 0;
	m_blockCount = 0;
	m_blockStartIndex = 1;
	m_coinCount = 0;
	m_bIsEnd = false;

	// ���� ������Ʈ �迭 �ʱ�ȭ
	m_GameObjectPtrTable = new GameObjectBase * [MAX_GAME_OBJECT_COUNT];

	for (int i = 0; i < MAX_GAME_OBJECT_COUNT; ++i)
	{
		m_GameObjectPtrTable[i] = nullptr;
	}

	// ��� ����
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

	// �÷��̾� ����
	CreatePlayer();

	// �� ������ �ʱ�ȭ �� �� ����
	m_pMapGenerator = new MapGenerator(this, m_pGame);
	m_pMapGenerator->CreateStereoMadnessMap();
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

	// �÷��̾� ��ġ�� �� ���� ����
	int height = m_pGame->GetHeight();
	pNewObject->SetPosition(200.0f, height - 500.0f);

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
	Vector2f playerPos = pPlayer->GetPosition();
	float checkRadius = 300.0f; // �÷��̾� �ֺ� �˻� ����

	for (int i = m_blockStartIndex; i < m_blockStartIndex + m_blockCount; i++)
	{
		GameObject* pObject = static_cast<GameObject*>(m_GameObjectPtrTable[i]);
		if (pObject)
		{
			if (pPlayer->GetColliderBox() && pObject->GetColliderBox())
			{
				Vector2f objPos = pObject->GetPosition();

				// �÷��̾�� ����� ������Ʈ�� �浹 �˻�
				float distance = std::sqrt(std::pow(playerPos.x - objPos.x, 2) +
					std::pow(playerPos.y - objPos.y, 2));

				if (distance > checkRadius) continue;


				if (learning::Intersect(*pPlayer->GetColliderBox(), *pObject->GetColliderBox()))
				{
					// ������ �÷��̾� �Ʒ��� ��� �鸸 ���߾��µ�...
					float playerBottom = pPlayer->GetPosition().y + pPlayer->GetColliderBox()->halfSize.y;
					float blockTop = pObject->GetPosition().y - pObject->GetColliderBox()->halfSize.y;
					// ������ �������� �񱳸� �մϴ�.
					float playerTop = pPlayer->GetPosition().y - pPlayer->GetColliderBox()->halfSize.y;
					float playerLeft = pPlayer->GetPosition().x - pPlayer->GetColliderBox()->halfSize.x;
					float playerRight = pPlayer->GetPosition().x + pPlayer->GetColliderBox()->halfSize.x;
					float blockLeft = pObject->GetPosition().x - pObject->GetColliderBox()->halfSize.x;
					float blockRight = pObject->GetPosition().x + pObject->GetColliderBox()->halfSize.x;

					// �浹 ���� ���� (��� �浹 vs ���� �浹)
					// ��� �浹�� �÷��̾ ��� ���� ������ ���� �߻�
					bool topCollision = playerBottom + 20 >= blockTop 
						&& playerBottom <= blockTop + 40 
						//&& pPlayer->GetVerticalVelocity() > 0
						; // �Ʒ��� �������� �־�� ��


					switch (pObject->Type())
					{
					case ObjectType::BLOCK1:
					case ObjectType::BLOCK2:
					case ObjectType::PLATFORM:
						//������ �����ϴٰ��!
						if (topCollision) {
							// ��� �浹 - �÷��̾ ��� ���� ��ġ
							pPlayer->SetPosition(pPlayer->GetPosition().x, blockTop - pPlayer->GetColliderBox()->halfSize.y);
							isGrounded = true;
							
						}
						else {
							// ���� �浹 - ��� ó��
							m_bIsDead = true;
							std::cout << "��� ��" << std::endl;
						}
						break;

					case ObjectType::SPIKEBED:
						// ���ÿ� ������ ��� ó��
						m_bIsDead = true;
						std::cout << "ħ�� ����" << std::endl;
						break;

					case ObjectType::GOAL:
						// �� ���� - ���� Ŭ����
						m_coinCount++;
						delete m_GameObjectPtrTable[i];
						m_GameObjectPtrTable[i] = nullptr;

						//// �ð������� ������ ������� �Ϸ��� (���û���)
						//pObject->SetWidth(0);
						//pObject->SetHeight(0);

						//// �Ǵ� �ݶ��̴��� ��Ȱ��ȭ
						//static_cast<GameObject*>(pObject)->SetColliderBox(0, 0);
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
						std::cout << "���� ����" << std::endl;
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