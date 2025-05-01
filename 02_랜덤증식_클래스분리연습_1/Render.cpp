#include "Render.h"
#include <iostream>

namespace render
{
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

            PostQuitMessage(0);
        }break;
        default:
            return::DefWindowProc(hwnd, msg, wparam, lparam);
        }
        return NULL;
    }

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
}
