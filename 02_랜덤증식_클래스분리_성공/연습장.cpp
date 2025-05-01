// @brief  ����ĥ�� ���α׷��� ���� ������
#include "INC_Windows.h"
#include "GameTimer.h"
#include "GeoShapeManager.h"

#include "WindowManager.h"

#include <iostream>

int main()
{
	WindowManager window;
	HWND hWnd = window.WindowManager::CreateWidow(L"NzWindowClass", L"���ϳ�", 1280, 720);
	if (NULL == hWnd)
	{
		std::cout << "������ ���� ����" << std::endl;
		system("pause");
		return -1;
	}

	window.WindowManager::GameInit();

	window.WindowManager::GameLoop();

	DestroyWindow(hWnd);

	return 0;

}