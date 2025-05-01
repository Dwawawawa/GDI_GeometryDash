// @brief  나전칠기 프로그래밍 기초 연습장
#include "INC_Windows.h"
#include "GameTimer.h"
#include "GeoShapeManager.h"
#include <iostream>
#include <random>
#include <cstdlib>

namespace
{
    void GameRender(HWND hWnd);
    // @breif  윈도우 프로시저
    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
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

    // @breif  윈도우 클래스 등록
    HWND CreateWidow(const wchar_t* className, const wchar_t* windowName, int width, int height)
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

        HWND hWnd = CreateWindowEx(NULL, MAKEINTATOM(classId), L"", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
            rc.right - rc.left, rc.bottom - rc.top, HWND(), HMENU(), HINSTANCE(), NULL);

        if (NULL == hWnd) return (HWND)(NULL);

        ::SetWindowText((HWND)hWnd, windowName);

        ShowWindow((HWND)hWnd, SW_SHOW);
        UpdateWindow((HWND)hWnd);

        return hWnd;
    }

    int MakeRandomNum() {

        int seed = rand();
        std::mt19937 gen(seed);
        std::uniform_int_distribution<int> dist(0, 99);

        int num = dist(gen);

        return num;
    }

    void GameInit(HWND hWnd)
    {
        RECT rcClient = {};
        GetClientRect(hWnd, &rcClient);

        //const int circleRadius = 50;
        //const int& clientWidth = rcClient.right - rcClient.left;
        //const int& clientHeight = rcClient.bottom - rcClient.top;

        //simplegeo::g_GeoShapeManager.AddCircle(clientWidth / 2, clientHeight / 2, circleRadius, RGB(0, 0, 255));
    }

    void GameUpdate(float deltaTime, HWND hWnd)
    {
        static float acculatedTime = 0;
        // 멤버로 들고 있으면 되는데... c 스타일로 짤려고
        // 요건 고정 프레임 연습을 위한 초기 코드
        // 클래스로?
        acculatedTime += deltaTime;

        if (acculatedTime < 1000.0) return;
        // 이건 while이 아닌데... 이건 if인데...


        GameRender(hWnd);

        acculatedTime -= 1000.0;
        // 요건 고정 프레임 연습을 위한 시도
        std::cout << "1초랍니다." << std::endl;
    }

    void GameRender(HWND hWnd)
    {
        HDC hDC = GetDC(hWnd);

        //1. 실제 DC와 호환되는 메모리 DC 생성
        HDC hMemDC = CreateCompatibleDC(hDC);

        //2. 메모리 DC에 사용할 호환 비트맵 생성
        RECT clientRect = {};
        GetClientRect(hWnd, &clientRect);

        const int& width = clientRect.right - clientRect.left;
        const int& height = clientRect.bottom - clientRect.top;

        HBITMAP hBitmap = CreateCompatibleBitmap(hDC, width, height);

        //3. 메모리 DC에 비트맵 설정
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

        // 메모리 DC를 흰색으로 채움 . ClearScreen()
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
        FillRect(hMemDC, &clientRect, hBrush);
        DeleteObject(hBrush);




        const int circleRadius = 20;
        const int rectangleEdge = 20;
        const int& clientWidth = clientRect.right - clientRect.left;
        const int& clientHeight = clientRect.bottom - clientRect.top;

        if (MakeRandomNum() % 2 == 0)
        {
            simplegeo::g_GeoShapeManager.AddCircle(clientWidth * MakeRandomNum() / 100, clientHeight * MakeRandomNum() / 100, circleRadius, RGB(255, 0, 0));
        }
        else {
            int rectangleX = clientWidth * MakeRandomNum() / 100;
            int rectangleY = clientWidth * MakeRandomNum() / 100;
            simplegeo::g_GeoShapeManager.AddRectangle(rectangleX - rectangleEdge, rectangleY + rectangleEdge, rectangleX + rectangleEdge, rectangleY - rectangleEdge, RGB(0, 0, 255));
        }





        //4. 메모리 DC에 그리기
        simplegeo::g_GeoShapeManager.Draw(hMemDC);

        //5. 메모리 DC에 그려진 결과를 실제 DC(hdc)로 복사
        BitBlt(hDC, 0, 0, width, height, hMemDC, 0, 0, SRCCOPY);

        //6. 할당한 자원 정리: 원래의 비트맵 복원 후 삭제
        SelectObject(hMemDC, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(hMemDC);

        ReleaseDC(hWnd, hDC);
        // 이해를 돕기 위해 여기에 모든 코드가 있습니다.
        // 매 렌더 프레임 마다 객체를 생성, 삭제할 필요는 없습니다.
    }

    void GameLoop(HWND hWnd)
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
                GameUpdate(timer.DeltaTimeMS(), hWnd);
                //GameRender(hWnd);
            }
        }
    }
}//namespace

int main()
{
    HWND hWnd = CreateWidow(L"NzWindowClass", L"원하나", 1280, 720);

    if (NULL == hWnd)
    {
        std::cout << "윈도우 생성 실패" << std::endl;
        system("pause");
        return -1;
    }

    GameInit(hWnd);

    GameLoop(hWnd);

    DestroyWindow(hWnd);

    return 0;

}