#include "WindowManager.h"
#include "GameManager.h"
#include "GeoShapeManager.h"
#include "GameTimer.h"
#include <iostream>


LRESULT WindowManager::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		std::cout << "WM_CREATE" << std::endl;
	}break;

	case WM_CLOSE:
	{
		std::cout << "WM_CLOSE" << std::endl;

		PostQuitMessage(0); // WM_QUIT
	}break;
	default:
		return::DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return NULL;
}

HWND WindowManager::CreateWidow(const wchar_t* className, const wchar_t* windowName, int width, int height)
{
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = className;
	wc.lpfnWndProc = WndProc; // ������ ���ν���(�Լ�)�� ������ ���
	ATOM classId = 0;
	if (!GetClassInfoEx(HINSTANCE(), L"NzWindowClass", &wc))
	{
		classId = RegisterClassEx(&wc);
		if (0 == classId) return NULL;
	}
	RECT rc = { 0, 0, width, height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
	std::cout << "width: " << rc.right - rc.left << " height: " << rc.bottom - rc.top << std::endl;

	// ���� ���� hWnd�� ����
	HWND hWnd = CreateWindowEx(NULL, MAKEINTATOM(classId), L"", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		rc.right - rc.left, rc.bottom - rc.top, HWND(), HMENU(), HINSTANCE(), NULL);

	if (NULL == hWnd) return (HWND)(NULL);

	// �� �κ��� �߿��մϴ�: ���� ���� hWnd�� Ŭ���� ��� ������ �Ҵ�
	this->hWnd = hWnd;

	::SetWindowText((HWND)hWnd, windowName);
	ShowWindow((HWND)hWnd, SW_SHOW);
	UpdateWindow((HWND)hWnd);

	return hWnd;
}

void WindowManager::GameInit()
{
	//RECT rcClient = {};
	//
	//GetClientRect(this->hWnd, &rcClient);
	//


	//this->clientWidth = rcClient.right - rcClient.left;
	//this->clientHeight = rcClient.bottom - rcClient.top;

	win_GameManager = new GameManager();

}

void WindowManager::GameUpdate(float deltaTime)
{
	static float acculatedTime = 0;

	acculatedTime += deltaTime;

	if (acculatedTime < 1000.0) return;
	
	acculatedTime -= 1000.0;
	// ���� ������ ������ ���� �ʱ� �ڵ�
	std::cout << "1" << std::endl;
	win_GameManager->MultiplyRandomShape();
}

void WindowManager::GameRender()
{
	HDC hDC = GetDC(this->hWnd);

	//1.  ���� DC�� ȣȯ�Ǵ� �޸� DC ����
	HDC hMemDC = CreateCompatibleDC(hDC);

	//2.  �޸� DC�� ����� ȣȯ ��Ʈ�� ����
	RECT clientRect = {};
	GetClientRect(this->hWnd, &clientRect);

	this->clientWidth = clientRect.right - clientRect.left;
	this->clientHeight = clientRect.bottom - clientRect.top;

	HBITMAP hBitmap = CreateCompatibleBitmap(hDC, clientWidth, clientHeight);

	//3. �޸� DC�� ��Ʈ�� ����
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

	// �޸� DC�� (���)���� ä��. ClearScreen()
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
	FillRect(hMemDC, &clientRect, hBrush);
	DeleteObject(hBrush);

	//4. �޸� DC�� �׸���
	simplegeo::g_GeoShapeManager.Draw(hMemDC);

	//5. �޸� DC�� �׷��� ����� ���� DC(hdc)�� ����
	BitBlt(hDC, 0, 0, clientWidth, clientHeight, hMemDC, 0, 0, SRCCOPY);

	//6. �Ҵ��� �ڿ� ����: ������ ��Ʈ�� ���� �� ����
	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hBitmap);
	DeleteDC(hMemDC);

	ReleaseDC(this->hWnd, hDC);

	// ���ظ� ���� ���� ���⿡ ��� �ڵ尡 �ֽ��ϴ�.
	// �� ���� �����Ӹ��� ��ü�� ���� ������ �ʿ�� �����ϴ�. 
}

void WindowManager::GameLoop()
{
	GameTimer timer;
	timer.Reset();

	MSG msg = {};
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, HWND(), 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			///HERE: GAME LOGIC UPATE & RENDERING
			timer.Tick();
			GameUpdate(timer.DeltaTimeMS());
			GameRender();
		}
	}
}

