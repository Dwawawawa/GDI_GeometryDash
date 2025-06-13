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

	void CreateBlock(float x, float y, int index);
	void CreateSpike(float x, float y, int index);
	void CreateMiniSpike(float x, float y, int index);
	void CreateBlock2(float x, float y, int index);
	void CreatePlatform(float x, float y, int index);
	void CreateSpikeBed(float x, float y, int index);
	void CreateGoal(float x, float y, int index);

	int GetBlockStartIndex() const { return m_blockStartIndex; }
	int GetBlockCount() const { return m_blockCount; }
	void UpdateBlockCount(int count) { m_blockCount = count; }

private:
	void CreatePlayer();
	void UpdatePlayerInfo();

	GameObjectBase* m_pBackground = nullptr;
	RECT m_rect = { 0, 0, 0, 0 };
	
	const float totalLength = 0.0f + (45 * 122) + (63 * 122) + (140 * 122) + (150 * 122) + (183.0f * 122);
	int m_blockStartIndex = 1; 
	float m_Fdinstance = 0;
	int m_blockCount = 0;
	int m_retryCount = 0;
	int m_coinCount = 0;
	bool m_bIsDead = false;
	bool m_bIsEnd = false;

	GameObject* GetPlayer() const { return (GameObject*)m_GameObjectPtrTable[0]; }

	MyFirstWndGame* m_pGame = nullptr;
	MapGenerator* m_pMapGenerator = nullptr;
};