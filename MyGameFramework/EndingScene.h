#pragma once
#include "Scene.h"
#include "RenderHelp.h"
#include "GameObject.h"

class MyFirstWndGame;

class EndingScene : public Scene
{
public:
	EndingScene() = default;
	~EndingScene() override = default;

	void Initialize(NzWndBase* pWnd) override;
	void Finalize() override;

	void Enter() override;
	void Leave() override;

	void FixedUpdate() override {}
	void Update(float deltaTime) override;
	void Render(HDC hDC) override;

	// 스페이스바 입력 처리 함수 추가
	void OnSpaceKeyPressed();

private:
	GameObjectBase* m_pBackground = nullptr;
	renderHelp::BitmapInfo* m_pEndingBitmapInfo = nullptr;

	MyFirstWndGame* m_pGame = nullptr;

	// 재시도 횟수
	int m_retryCount = 0;
	
	// 획득 코인
	int m_coinCount= 0;

	// 사용자 입력 대기 상태 플래그
	bool m_bWaitingForInput = true;

	// 메시지 표시 영역
	RECT m_retryRect = { 0, 0, 0, 0 };
	RECT m_coinRect = { 0, 0, 0, 0 };
	RECT m_continueRect = { 0, 0, 0, 0 };
};
