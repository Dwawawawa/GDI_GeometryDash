#pragma once
#include "INC_Windows.h"
#include "GameManager.h"
class GameManager;

class WindowManager {
public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
	HWND CreateWidow(const wchar_t* className, const wchar_t* windowName, int width, int height);
	void GameInit();
	void GameUpdate(float deltaTime);
	void GameRender();
	void GameLoop();

	HWND GetHWND() const { return hWnd; };
	int GetClientWidth() { return clientWidth; };
	int GetClientHeight() const { return clientHeight; };
private:
	HWND hWnd;
	int clientWidth = 1280;
	int clientHeight = 720;
	GameManager* win_GameManager;
};