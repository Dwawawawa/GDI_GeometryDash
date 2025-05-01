#pragma once
#include "INC_Windows.h"


namespace render
{
	// @breif  윈도우 프로시저
	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	// @breif  윈도우 클래스 등록
	HWND CreateWidow(const wchar_t* className, const wchar_t* windowName, int width, int height);

}