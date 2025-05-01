#include <iostream>
//#include "INC_Windows.h"
#include <Windows.h>
#include "GeoShapeManager.h"
namespace
{

    namespace
    {
        class DebugConsole
        {
            DebugConsole(const DebugConsole&) = delete;
            DebugConsole& operator=(const DebugConsole&) = delete;
            DebugConsole(DebugConsole&&) = delete;
            DebugConsole& operator=(DebugConsole&&) = delete;
        public:
            DebugConsole()
            {
                AllocConsole();

                FILE* stream;
                freopen_s(&stream, "CONOUT$", "w", stdout);
                freopen_s(&stream, "CONIN$", "r", stdin);
            }

            ~DebugConsole()
            {
                fclose(stdout);
                fclose(stdin);

                FreeConsole();
            }

            static void Show()
            {
                ::ShowWindow(GetConsoleWindow(), SW_SHOW);
            }

            static void Hide()
            {
                ::ShowWindow(GetConsoleWindow(), SW_HIDE);
            }

            static void SetTitle(const wchar_t* title)
            {
                ::SetConsoleTitle(title);
            }

            static void ToggleShow()
            {
                HWND hwnd = GetConsoleWindow();
                if (hwnd)
                {
                    if (::IsWindowVisible(hwnd))
                    {
                        Hide();
                    }
                    else
                    {
                        Show();
                    }
                }
            }
        };

        DebugConsole g_debugConsole;
    }


    // @breif  윈도우 프로시저
    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        switch (msg)
        {
        case WM_CREATE:// 윈도우 메세지 만들기
        {
            std::cout << "WM_CREATE" << std::endl;
        }break;

        case WM_CLOSE:
        {
            std::cout << "WM_CLOSE" << std::endl;
            // 윈도우를 종료하기 위한 메시지
            PostQuitMessage(0); // WM_QUIT
        }break;
        case WM_PAINT:
        {
            std::cout << "WM_PAINT" << std::endl;

            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            FillRect(hdc, &clientRect, (HBRUSH)(COLOR_WINDOW + 1));

            simplegeo::g_GeoShapeManager.Draw(hdc);

            EndPaint(hwnd, &ps);

        }break;
        case WM_LBUTTONDOWN:
        {
            std::cout << "WM_LBUTTONDBLCLK" << std::endl;
            // 마우스 왼쪽 버튼 더블 클릭
            // 좌표를 가져온다.
            int x = LOWORD(lparam);
            int y = HIWORD(lparam);


            // 나는 그 x, y 좌표를 통해 깨달았으면 그 배열에 접근했으면 좋겠어.
            int index = simplegeo::g_GeoShapeManager.CheckShapeArr(x, y);
            if (index == -1) {
                simplegeo::g_GeoShapeManager.AddCircle(x, y, 10, RGB(255, 0, 0));
            }
            else {
                simplegeo::g_GeoShapeManager.DeleteShape(index);
            }

            ::InvalidateRect(hwnd, NULL, TRUE);

        }break;
        case WM_RBUTTONDOWN:
        {
            std::cout << "WM_RBUTTONDBLCLK" << std::endl;
            // 마우스 오른쪽 버튼 더블 클릭
            // 좌표를 가져온다.
            int x = LOWORD(lparam);
            int y = HIWORD(lparam);

            
            int index = simplegeo::g_GeoShapeManager.CheckShapeArr(x, y);
            if (index == -1) {
                simplegeo::g_GeoShapeManager.AddRectangle(x - 10, y + 10, x + 10, y - 10, RGB(0, 255, 0));
            }
            else {
                simplegeo::g_GeoShapeManager.DeleteShape(index);
            }
            
            ::InvalidateRect(hwnd, NULL, TRUE);
        }break;
        case WM_KEYDOWN:
        {
            if (wparam == 'C' || wparam == 'c')
            {
                std::cout << "WM_KEYDOWN" << std::endl;
                DebugConsole::ToggleShow();
            }
        }break;
        default:
            return::DefWindowProc(hwnd, msg, wparam, lparam);
        }//아무짓 안할래~
        return NULL;
    }

    // @breif  윈도우 클래스 등록
    HWND CreateWidow(const wchar_t* className, const wchar_t* windowName, int width, int height)
    {
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpszClassName = className;
        wc.lpfnWndProc = WndProc; // 윈도우 프로시저(함수)의 포인터 등록
        //wc.lpfnWndProc = &WndProc; 이거랑 위랑 같데... 얜 전역이라 돼. 멤버땐 안된데...


        ATOM classId = 0;
        if (!GetClassInfoEx(HINSTANCE(), L"NzWindowClass", &wc))
        {
            classId = RegisterClassEx(&wc); // 운영체제한테 지금 만든 클래스 정보를 가지고 '윈도우 등록 정보' 하나 만들어 줘

            if (0 == classId) return NULL;
        }

        RECT rc = { 0, 0, width, height };

        AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false); // 창 크기를 조정해 줍니다.

        std::cout << "width: " << rc.right - rc.left << " height: " << rc.bottom - rc.top << std::endl;

        // 드디어 준비된 WNDCLASSEX wc를 운영체제에 등록해서 받은 식별자 키를 가지고 (classId)고 원도우 (커널) 객체를 만들어 달라 하고 윈도우 핸들을 받아 옵니다.
        HWND hWnd = CreateWindowEx(NULL, MAKEINTATOM(classId), L"", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
            rc.right - rc.left, rc.bottom - rc.top, HWND(), HMENU(), HINSTANCE(), NULL);

        if (NULL == hWnd) return (HWND)(NULL); // 못 만들었다면 ㅠㅠ 실패

        ::SetWindowText((HWND)hWnd, windowName); // 원도우창에 텍스트 설정

        ShowWindow((HWND)hWnd, SW_SHOW); // 원도우 보여줘.(hide하면 안보여줘)
        UpdateWindow((HWND)hWnd); // 업데이트를 해줘야 실제 wm_message 처리가 가능해 집니다.

        return hWnd;
    }


    void GameLoop()
    {
        MSG msg = {};
        while (WM_QUIT != msg.message)
        {
            if (PeekMessage(&msg, HWND(), 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg); // 문자 a인지 키 a인지 번역
                DispatchMessage(&msg); 
                
            }
            else
            {
                ///HERE: GAME LOGIC UPATE & RENDERING
            }
        }
    }
}

int main()
{
    // GUI 창 생성
    HWND hWnd = CreateWidow(L"NzWindowClass", L"나전칠기", 1280, 720);

    if (NULL == hWnd)
    {
        std::cout << "윈도우 생성 실패" << std::endl;
        system("pause");
        return -1;
    }
    
    std::cout << "hWnd: " << hWnd << std::endl;
    //std::cout << (*hWnd).unused << std::endl; 런타임 에러!

    // 게임 루프를 넣어 보자.
    GameLoop();

    DestroyWindow(hWnd);

	return 0;
}