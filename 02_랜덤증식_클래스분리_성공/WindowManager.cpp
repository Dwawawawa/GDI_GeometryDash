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
	wc.lpfnWndProc = WndProc; // 윈도우 프로시저(함수)의 포인터 등록
	ATOM classId = 0;
	if (!GetClassInfoEx(HINSTANCE(), L"NzWindowClass", &wc))
	{
		classId = RegisterClassEx(&wc);
		if (0 == classId) return NULL;
	}
	RECT rc = { 0, 0, width, height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
	std::cout << "width: " << rc.right - rc.left << " height: " << rc.bottom - rc.top << std::endl;

	// 로컬 변수 hWnd를 생성
	HWND hWnd = CreateWindowEx(NULL, MAKEINTATOM(classId), L"", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		rc.right - rc.left, rc.bottom - rc.top, HWND(), HMENU(), HINSTANCE(), NULL);

	if (NULL == hWnd) return (HWND)(NULL);

	// 이 부분이 중요합니다: 로컬 변수 hWnd를 클래스 멤버 변수에 할당
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
	// 고정 프레임 연습을 위한 초기 코드
	std::cout << "1" << std::endl;
	win_GameManager->MultiplyRandomShape();
}

void WindowManager::GameRender()
{
	HDC hDC = GetDC(this->hWnd);

	//1.  실제 DC와 호환되는 메모리 DC 생성
	HDC hMemDC = CreateCompatibleDC(hDC);

	//2.  메모리 DC에 사용할 호환 비트맵 생성
	RECT clientRect = {};
	GetClientRect(this->hWnd, &clientRect);

	this->clientWidth = clientRect.right - clientRect.left;
	this->clientHeight = clientRect.bottom - clientRect.top;

	HBITMAP hBitmap = CreateCompatibleBitmap(hDC, clientWidth, clientHeight);

	//3. 메모리 DC에 비트맵 설정
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

	// 메모리 DC를 (흰색)으로 채움. ClearScreen()
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
	FillRect(hMemDC, &clientRect, hBrush);
	DeleteObject(hBrush);

	//4. 메모리 DC에 그리기
	simplegeo::g_GeoShapeManager.Draw(hMemDC);

	//5. 메모리 DC에 그려진 결과를 실제 DC(hdc)로 복사
	BitBlt(hDC, 0, 0, clientWidth, clientHeight, hMemDC, 0, 0, SRCCOPY);

	//6. 할당한 자원 정리: 원래의 비트맵 복원 후 삭제
	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hBitmap);
	DeleteDC(hMemDC);

	ReleaseDC(this->hWnd, hDC);

	// 이해를 돕기 위해 여기에 모든 코드가 있습니다.
	// 매 렌더 프레임마다 객체를 생성 삭제할 필요는 없습니다. 
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

