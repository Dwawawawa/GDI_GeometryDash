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

	// �����̽��� �Է� ó�� �Լ� �߰�
	void OnSpaceKeyPressed();

private:
	GameObjectBase* m_pBackground = nullptr;
	renderHelp::BitmapInfo* m_pEndingBitmapInfo = nullptr;

	MyFirstWndGame* m_pGame = nullptr;

	// ��õ� Ƚ��
	int m_retryCount = 0;
	
	// ȹ�� ����
	int m_coinCount= 0;

	// ����� �Է� ��� ���� �÷���
	bool m_bWaitingForInput = true;

	// �޽��� ǥ�� ����
	RECT m_retryRect = { 0, 0, 0, 0 };
	RECT m_coinRect = { 0, 0, 0, 0 };
	RECT m_continueRect = { 0, 0, 0, 0 };
};
