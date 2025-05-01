#pragma once
#include "Scene.h"

class GameObjectBase;
class GameObject;
class MyFirstWndGame;
class MapGenerator;

class PlayScene :public Scene
{
public:
	PlayScene() = default;
	~PlayScene() override = default;

	void Initialize(NzWndBase* pWnd) override;
	void Finalize() override;

	void Enter() override;
	void Leave() override;

	void FixedUpdate() override;
	void Update(float deltaTime) override;
	void Render(HDC hDC) override;

	// MapGenerator에서 사용할 수 있도록 public으로 노출
	void CreateBlock(float x, float y, int index);
	void CreateSpike(float x, float y, int index);
	void CreateMiniSpike(float x, float y, int index);
	void CreateBlock2(float x, float y, int index);
	void CreatePlatform(float x, float y, int index);
	void CreateSpikeBed(float x, float y, int index);
	void CreateGoal(float x, float y, int index);

	// 블록 카운트 관련 접근자
	int GetBlockStartIndex() const { return m_blockStartIndex; }
	int GetBlockCount() const { return m_blockCount; }
	void UpdateBlockCount(int count) { m_blockCount = count; }

private:
	void CreatePlayer();
	void UpdatePlayerInfo();

	//배경
	GameObjectBase* m_pBackground = nullptr;
	RECT m_rect = { 0, 0, 0, 0 };

	//인덱스 관리를 위한 변수
	int m_blockStartIndex = 1;
	int m_blockCount = 0;
	int m_levelPosition = 0; // 레벨 진행도를 축적하는 변수
	int m_retryCount = 0;    // 재시도 횟수
	bool m_bIsDead = false;
	bool m_bIsEnd = false;

	GameObject* GetPlayer() const { return (GameObject*)m_GameObjectPtrTable[0]; }

	MyFirstWndGame* m_pGame = nullptr;
	MapGenerator* m_pMapGenerator = nullptr;
};