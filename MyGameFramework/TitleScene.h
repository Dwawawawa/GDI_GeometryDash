#pragma once
#include "Scene.h"
#include "RenderHelp.h"
#include "GameObject.h"

class GameObjectBase;
class MyFirstWndGame;

class TitleScene : public Scene
{
public:
    TitleScene() = default;
    ~TitleScene() override = default;

    void Initialize(NzWndBase* pWnd) override;
    void Finalize() override;

    void Enter() override;
    void Leave() override;

    void FixedUpdate() override {}
    void Update(float deltaTime) override;
    void Render(HDC hDC) override;

    void OnSpaceKeyPressed();

private:

    GameObjectBase* m_pBackground = nullptr;
    renderHelp::BitmapInfo* m_pTitleBitmapInfo = nullptr;
    

    MyFirstWndGame* m_pGame = nullptr;

    WCHAR m_szTitle[64] = L"Title Scene";

    bool m_bWaitingForInput = true;

    RECT m_rect = { 0, 0, 0, 0 };
};

