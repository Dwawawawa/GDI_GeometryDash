#include "EndingScene.h"
#include "MyFirstWndGame.h"
#include "GameObject.h"
#include "Utillity.h"
#include <cassert>
#include <iostream>

void EndingScene::Initialize(NzWndBase* pWnd)
{
    m_pGame = dynamic_cast<MyFirstWndGame*>(pWnd);
    assert(m_pGame != nullptr && "Game object is not initialized!");

    // 엔딩 배경 생성
    Background* pNewObject = new Background(ObjectType::BACKGROUND);
    pNewObject->SetPosition(0.0f, 0.0f);

    int width = m_pGame->GetWidth();
    int height = m_pGame->GetHeight();

    pNewObject->SetWidth(width);
    pNewObject->SetHeight(height);

    // 엔딩 이미지 로드
    m_pEndingBitmapInfo = renderHelp::CreateBitmapInfo(L"../Resource/geometryDash/ending.png");
    if (m_pEndingBitmapInfo != nullptr) {
        pNewObject->SetBitmapInfo(m_pEndingBitmapInfo);
    }
    else {
        // 백업용으로 기본 배경 사용
        pNewObject->SetBitmapInfo(m_pGame->GetBackgroundBitmapInfo());
    }

    m_pBackground = pNewObject;

    // 메시지 표시 영역 설정
    m_retryRect.left = width / 2 - 250;
    m_retryRect.top = height / 2 - 50;
    m_retryRect.right = m_retryRect.left + 500;
    m_retryRect.bottom = m_retryRect.top + 100;

    m_continueRect.left = width / 2 - 250;
    m_continueRect.top = height / 2 + 100;
    m_continueRect.right = m_continueRect.left + 500;
    m_continueRect.bottom = m_continueRect.top + 50;

    // 대기 상태로 설정
    m_bWaitingForInput = true;
}

void EndingScene::Update(float deltaTime)
{
	// 스페이스바 입력이 MyFirstWndGame::Run()에서 감지되면 m_bWaitingForInput이 false로 변경됨
	if (!m_bWaitingForInput) {
		m_pGame->ChangeScene(SceneType::SCENE_TITLE);
	}
}

void EndingScene::Render(HDC hDC)
{
	assert(m_pGame != nullptr && "Game object is not initialized!");
	m_pBackground->Render(hDC);

	// 성공 메시지와 재시도 횟수 표시
	WCHAR retryMessage[128];
	swprintf_s(retryMessage, L"%d번의 시도 끝에 성공했습니다!", m_retryCount);

	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);

	// 재시도 횟수 메시지 표시
	DrawText(hDC, retryMessage, -1, &m_retryRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	// 계속하기 메시지 표시
	DrawText(hDC, L"스페이스바를 눌러 계속하기", -1, &m_continueRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void EndingScene::Finalize()
{
	if (m_pBackground)
	{
		delete m_pBackground;
		m_pBackground = nullptr;
	}

	// 비트맵 정보 리소스는 게임에서 관리하므로 여기서 해제하지 않음
	m_pEndingBitmapInfo = nullptr;
}

void EndingScene::Enter()
{
	// PlayScene에서 저장된 재시도 횟수 가져오기
	m_retryCount = static_cast<MyFirstWndGame*>(m_pGame)->GetRetryCount();

	// 입장 시 다시 대기 상태로 설정
	m_bWaitingForInput = true;
}

void EndingScene::Leave()
{
}

void EndingScene::OnSpaceKeyPressed()
{
	// 스페이스바 입력 감지
	m_bWaitingForInput = false;
}

