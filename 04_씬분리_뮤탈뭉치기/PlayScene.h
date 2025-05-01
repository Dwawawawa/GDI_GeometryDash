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
    void CreateEnemy();

    void UpdatePlayerInfo();
    void UpdateEnemyInfo();
    //-- ¹è°æ
    GameObjectBase* m_pBackground = nullptr;
    RECT m_rect = { 0, 0, 0, 0 };
    //--
    GameObject* GetPlayer() const { return (GameObject*)m_GameObjectPtrTable[0]; }

    MyFirstWndGame* m_pGame = nullptr;

    int m_CollisionCount = 0;
};

