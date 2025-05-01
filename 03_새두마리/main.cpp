// @brief  나전칠기 프로그래밍 기초 연습장
#define _CRTDBG_MAP_ALLOC

#include "MyFirstWndGame.h"
#include <iostream>
#include <crtdbg.h>

int main()
{
	// 첫 줄은 꼭 이 함수를 호출하고 시작 합니다.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//_CrtSetBreakAlloc(160);


	MyFirstWndGame game;
	if (false == game.Initialize())
	{
		std::cout << "Failed to initialize the game. Error: " << GetLastError() << std::endl;
		return -1;
	}

	game.Run();

	game.Finalize();

	return 0;

}