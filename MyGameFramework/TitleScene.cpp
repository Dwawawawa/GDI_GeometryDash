#include "TitleScene.h"
#include "MyFirstWndGame.h"
#include "GameObject.h"
#include "Utillity.h"
#include <cassert>
#include <iostream>
using namespace std;
using namespace learning;

void TitleScene::Initialize(NzWndBase* pWnd)
{
    m_pGame = dynamic_cast<MyFirstWndGame*>(pWnd);
    assert(m_pGame != nullptr && "Game object is not initialized!");

	// 타이틀 이미지 로드
	m_pTitleBitmapInfo = renderHelp::CreateBitmapInfo(L"../Resource/geometryDash/title.png");

    Background* pNewObject = new Background(ObjectType::BACKGROUND);
    pNewObject->SetPosition(0.0f, 0.0f);

    int width = m_pGame->GetWidth();
    int height = m_pGame->GetHeight();

    pNewObject->SetWidth(width);
    pNewObject->SetHeight(height);

   // 배경 이미지 설정 - 타이틀 이미지 또는 기본 배경
    if (m_pTitleBitmapInfo != nullptr) {
        pNewObject->SetBitmapInfo(m_pTitleBitmapInfo);
    } else {
        // 백업용으로 기본 배경 사용
        pNewObject->SetBitmapInfo(m_pGame->GetBackgroundBitmapInfo());
    }

	m_rect.left = width / 2 - 300;
	m_rect.top = height / 2 + 150;
	m_rect.right = m_rect.left + 600;
	m_rect.bottom = m_rect.top + 60;

    m_pBackground = pNewObject;

    wcscpy_s(m_szTitle, L"스페이스바를 눌러 시작하세요");


    
    m_bWaitingForInput = true;

}

void TitleScene::Update(float deltaTime)
{
	// 스페이스바 입력이 MyFirstWndGame::Run()에서 감지되면 m_bWaitingForInput이 false로 변경됨
	if (!m_bWaitingForInput) {
		m_pGame->ChangeScene(SceneType::SCENE_PLAY);
	}
}

void TitleScene::Render(HDC hDC)
{
    assert(m_pGame != nullptr && "Game object is not initialized!");
    m_pBackground->Render(hDC);
    

	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);
	DrawText(hDC, m_szTitle, -1, &m_rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void TitleScene::Finalize()
{
    if (m_pBackground)
    {
        delete m_pBackground;
        m_pBackground = nullptr;
    }

    m_pTitleBitmapInfo = nullptr;
}

void TitleScene::Enter()
{
    m_bWaitingForInput = true;
}

void TitleScene::Leave()
{
}

void TitleScene::OnSpaceKeyPressed()
{
	// 스페이스바 입력 감지
	m_bWaitingForInput = false;
}

