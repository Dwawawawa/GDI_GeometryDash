﻿#pragma once
#include "NzWndBase.h"
#include "Utillity.h"
#include "Scene.h"
#include "AudioSystem.h"

class GameTimer;
class Scene;

namespace renderHelp
{
    class BitmapInfo;
}

class MyFirstWndGame : public NzWndBase
{
public:
    MyFirstWndGame() = default;
    ~MyFirstWndGame() override = default;

    bool Initialize();
    void Run();
    void Finalize();

    void ChangeScene(SceneType eSceneType);

	void SetRetryCount(int count) { m_retryCount = count; }
	int GetRetryCount() const { return m_retryCount; }
    void SetCoinCount(int count) { m_coinCount = count; }
    int GetCoinCount() const { return m_coinCount; }


    void OnSpaceKeyDown();

private:
    void Update();
    void Render();

    void OnResize(int width, int height) override;
    void OnClose() override;

    void OnMouseMove(int x, int y);
    void OnLButtonDown(int x, int y);

    void FixedUpdate();
    void LogicUpdate();

	// 오디오 관련 메서드 추가
	bool InitializeAudio();
	void FinalizeAudio();

private:
    HDC m_hFrontDC = nullptr;
    HDC m_hBackDC = nullptr;
    HBITMAP m_hBackBitmap = nullptr;
    HBITMAP m_hDefaultBitmap = nullptr;

    GameTimer* m_pGameTimer = nullptr;
    float m_fDeltaTime = 0.0f;
    float m_fFrameCount = 0.0f;

    int m_eCurrentScene = SCENE_TITLE;
    Scene* m_pScenes[SceneType::SCENE_MAX] = { nullptr, nullptr, nullptr };
    
    int m_coinCount = 0;
    int m_retryCount = 0;

    struct MOUSE_POS
    {
        int x = 0;
        int y = 0;

        bool operator!=(const MOUSE_POS& other) const
        {
            return (x != other.x || y != other.y);
        }
    };

    MOUSE_POS m_MousePos = { 0, 0 };
    MOUSE_POS m_MousePosPrev = { 0, 0 };

    MOUSE_POS m_PlayerTargetPos = { 0, 0 };

#pragma region resource
    using BitmapInfo = renderHelp::BitmapInfo;

    BitmapInfo* m_pPlayerBitmapInfo = nullptr;
    BitmapInfo* m_pBlock1BitmapInfo = nullptr;
    BitmapInfo* m_pBackgroundBitmapInfo = nullptr;
    
    BitmapInfo* m_pSpikeBitmapInfo = nullptr;
    BitmapInfo* m_pBlock2BitmapInfo = nullptr;
    BitmapInfo* m_PlatformBitmapInfo = nullptr;
    BitmapInfo* m_pSpikeBedBitmapInfo = nullptr;
    BitmapInfo* m_pGoalBitmapInfo = nullptr;


public:
    using Vector2f = learning::Vector2f;
    Vector2f PlayerTargetPosition() const { return {static_cast<float>(m_PlayerTargetPos.x), static_cast<float>(m_PlayerTargetPos.y)}; }


    BitmapInfo* GetPlayerBitmapInfo() const { return m_pPlayerBitmapInfo; }
    BitmapInfo* GetBlock1BitmapInfo() const { return m_pBlock1BitmapInfo; }
    BitmapInfo* GetBackgroundBitmapInfo() const { return m_pBackgroundBitmapInfo; }

    BitmapInfo* GetSpikeBitmapInfo() const { return m_pSpikeBitmapInfo; }
    BitmapInfo* GetBlock2BitmapInfo() const { return m_pBlock2BitmapInfo; }
    BitmapInfo* GetPlatformBitmapInfo() const { return m_PlatformBitmapInfo; }
    BitmapInfo* GetSpikeBedBitmapInfo() const { return m_pSpikeBedBitmapInfo; }
    BitmapInfo* GetGoalBitmapInfo() const { return m_pGoalBitmapInfo; }


#pragma endregion
};