// @brief  나전칠기 프로그래밍 기초 연습장
#include "INC_Windows.h"
#include "GameTimer.h"
#include "GeoShapeManager.h"

#include "WindowManager.h"

#include <iostream>

int main()
{
	WindowManager window;
	HWND hWnd = window.WindowManager::CreateWidow(L"NzWindowClass", L"원하나", 1280, 720);
	if (NULL == hWnd)
	{
		std::cout << "윈도우 생성 실패" << std::endl;
		system("pause");
		return -1;
	}

	window.WindowManager::GameInit();

	window.WindowManager::GameLoop();

	DestroyWindow(hWnd);

	return 0;

}