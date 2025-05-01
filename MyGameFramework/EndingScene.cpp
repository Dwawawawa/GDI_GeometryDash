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

    // ���� ��� ����
    Background* pNewObject = new Background(ObjectType::BACKGROUND);
    pNewObject->SetPosition(0.0f, 0.0f);

    int width = m_pGame->GetWidth();
    int height = m_pGame->GetHeight();

    pNewObject->SetWidth(width);
    pNewObject->SetHeight(height);

    // ���� �̹��� �ε�
    m_pEndingBitmapInfo = renderHelp::CreateBitmapInfo(L"../Resource/geometryDash/ending.png");
    if (m_pEndingBitmapInfo != nullptr) {
        pNewObject->SetBitmapInfo(m_pEndingBitmapInfo);
    }
    else {
        // ��������� �⺻ ��� ���
        pNewObject->SetBitmapInfo(m_pGame->GetBackgroundBitmapInfo());
    }

    m_pBackground = pNewObject;

    // �޽��� ǥ�� ���� ����
    m_retryRect.left = width / 2 - 250;
    m_retryRect.top = height / 2 - 50;
    m_retryRect.right = m_retryRect.left + 500;
    m_retryRect.bottom = m_retryRect.top + 100;

    m_continueRect.left = width / 2 - 250;
    m_continueRect.top = height / 2 + 100;
    m_continueRect.right = m_continueRect.left + 500;
    m_continueRect.bottom = m_continueRect.top + 50;

    // ��� ���·� ����
    m_bWaitingForInput = true;
}

void EndingScene::Update(float deltaTime)
{
	// �����̽��� �Է��� MyFirstWndGame::Run()���� �����Ǹ� m_bWaitingForInput�� false�� �����
	if (!m_bWaitingForInput) {
		m_pGame->ChangeScene(SceneType::SCENE_TITLE);
	}
}

void EndingScene::Render(HDC hDC)
{
	assert(m_pGame != nullptr && "Game object is not initialized!");
	m_pBackground->Render(hDC);

	// ���� �޽����� ��õ� Ƚ�� ǥ��
	WCHAR retryMessage[128];
	swprintf_s(retryMessage, L"%d���� �õ� ���� �����߽��ϴ�!", m_retryCount);

	SetTextColor(hDC, RGB(255, 255, 255));
	SetBkMode(hDC, TRANSPARENT);

	// ��õ� Ƚ�� �޽��� ǥ��
	DrawText(hDC, retryMessage, -1, &m_retryRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	// ����ϱ� �޽��� ǥ��
	DrawText(hDC, L"�����̽��ٸ� ���� ����ϱ�", -1, &m_continueRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void EndingScene::Finalize()
{
	if (m_pBackground)
	{
		delete m_pBackground;
		m_pBackground = nullptr;
	}

	// ��Ʈ�� ���� ���ҽ��� ���ӿ��� �����ϹǷ� ���⼭ �������� ����
	m_pEndingBitmapInfo = nullptr;
}

void EndingScene::Enter()
{
	// PlayScene���� ����� ��õ� Ƚ�� ��������
	m_retryCount = static_cast<MyFirstWndGame*>(m_pGame)->GetRetryCount();

	// ���� �� �ٽ� ��� ���·� ����
	m_bWaitingForInput = true;
}

void EndingScene::Leave()
{
}

void EndingScene::OnSpaceKeyPressed()
{
	// �����̽��� �Է� ����
	m_bWaitingForInput = false;
}

