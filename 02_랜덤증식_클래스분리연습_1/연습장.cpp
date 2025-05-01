// @brief  나전칠기 프로그래밍 기초 연습장
#include "INC_Windows.h"
#include "GameTimer.h"
#include "GeoShapeManager.h"
#include "input.h"
#include "GameManager.h"
#include "render.h"
#include <iostream>

#include <cstdlib>

namespace
{
    void GameRender(HWND hWnd);
    
    void GameInit(HWND hWnd)
    {
        //뭘 넣어야 할까... 업데이트
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
        // 게임 루프에 갇히지 않기 위해
        

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

        //싱글톤으로 만든 객체
        simplegeo::GeoShapeManager& g_GeoShapeManager = simplegeo::GeoShapeManager::GetInstance();

        // 게임 로직 그리기
        game::GameManager::MultiplyShape(width, height, g_GeoShapeManager);



        //4. 메모리 DC에 그리기
        g_GeoShapeManager.Draw(hMemDC);

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
                GameUpdate(timer.DeltaTimeMS(),hWnd);
                //GameRender(hWnd);
            }
        }
    }
}//namespace

int main()
{
    HWND hWnd = render::CreateWidow(L"NzWindowClass", L"원하나", 1280, 720);

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