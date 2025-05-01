// @brief  ����ĥ�� ���α׷��� ���� ������
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
        //�� �־�� �ұ�... ������Ʈ
    }

    void GameUpdate(float deltaTime, HWND hWnd)
    {
        static float acculatedTime = 0;
        // ����� ��� ������ �Ǵµ�... c ��Ÿ�Ϸ� ©����
        // ��� ���� ������ ������ ���� �ʱ� �ڵ�
        // Ŭ������?
        acculatedTime += deltaTime;

        if (acculatedTime < 1000.0) return;
        // �̰� while�� �ƴѵ�... �̰� if�ε�...
        // ���� ������ ������ �ʱ� ����
        

        GameRender(hWnd);

        acculatedTime -= 1000.0;
        // ��� ���� ������ ������ ���� �õ�
        std::cout << "1�ʶ��ϴ�." << std::endl;
    }

    void GameRender(HWND hWnd)
    {
        HDC hDC = GetDC(hWnd);

        //1. ���� DC�� ȣȯ�Ǵ� �޸� DC ����
        HDC hMemDC = CreateCompatibleDC(hDC);

        //2. �޸� DC�� ����� ȣȯ ��Ʈ�� ����
        RECT clientRect = {};
        GetClientRect(hWnd, &clientRect);

        const int& width = clientRect.right - clientRect.left;
        const int& height = clientRect.bottom - clientRect.top;

        HBITMAP hBitmap = CreateCompatibleBitmap(hDC, width, height);

        //3. �޸� DC�� ��Ʈ�� ����
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

        // �޸� DC�� ������� ä�� . ClearScreen()
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
        FillRect(hMemDC, &clientRect, hBrush);
        DeleteObject(hBrush);

        //�̱������� ���� ��ü
        simplegeo::GeoShapeManager& g_GeoShapeManager = simplegeo::GeoShapeManager::GetInstance();

        // ���� ���� �׸���
        game::GameManager::MultiplyShape(width, height, g_GeoShapeManager);



        //4. �޸� DC�� �׸���
        g_GeoShapeManager.Draw(hMemDC);

        //5. �޸� DC�� �׷��� ����� ���� DC(hdc)�� ����
        BitBlt(hDC, 0, 0, width, height, hMemDC, 0, 0, SRCCOPY);

        //6. �Ҵ��� �ڿ� ����: ������ ��Ʈ�� ���� �� ����
        SelectObject(hMemDC, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(hMemDC);

        ReleaseDC(hWnd, hDC);
        // ���ظ� ���� ���� ���⿡ ��� �ڵ尡 �ֽ��ϴ�.
        // �� ���� ������ ���� ��ü�� ����, ������ �ʿ�� �����ϴ�.
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
    HWND hWnd = render::CreateWidow(L"NzWindowClass", L"���ϳ�", 1280, 720);

    if (NULL == hWnd)
    {
        std::cout << "������ ���� ����" << std::endl;
        system("pause");
        return -1;
    }

    GameInit(hWnd);

    GameLoop(hWnd);

    DestroyWindow(hWnd);

    return 0;

}