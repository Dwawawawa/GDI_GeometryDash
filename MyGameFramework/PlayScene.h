#pragma once
#include "Scene.h"

class GameObjectBase;
class GameObject;
class MyFirstWndGame;

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
  
private:
    void CreatePlayer();
    
    void CreateBlock(float x, float y, int index); 

    void CreateSpike(float x, float y, int index);
    void CreateMiniSpike(float x, float y, int index);
    void CreateBlock2(float x, float y, int index);
    void CreatePlatform(float x, float y, int index);
    void CreateSpikeBed(float x, float y, int index);
    void CreateGoal(float x, float y, int index);

	// 새로운 장애물 생성 함수(랜덤)
	//void GenerateNewObstacles();
    
	// Stereo Madness 맵 생성 함수
	void CreateStereoMadnessMap();
	void CreateMapSection1(); 
	void CreateMapSection2(); 
	void CreateMapSection3(); 
	void CreateMapSection4(); 

    void UpdatePlayerInfo();


    //배경
    GameObjectBase* m_pBackground = nullptr;
    RECT m_rect = { 0, 0, 0, 0 };
    

    //인덱스 관리를 위한 변수
    int m_blockStartIndex = 1;
    int m_blockCount = 0;
    int m_levelPosition = 0; // 레벨 진행도를 추적하는 변수
    bool m_bIsDead = false;
    bool m_bIsEnd = false;

	// 재시도 횟수 추가
	int m_retryCount = 0;

    GameObject* GetPlayer() const { return (GameObject*)m_GameObjectPtrTable[0]; }    

    MyFirstWndGame* m_pGame = nullptr;
};

