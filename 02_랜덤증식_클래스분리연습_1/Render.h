#pragma once
#include "INC_Windows.h"


namespace render
{
	// @breif  ������ ���ν���
	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	// @breif  ������ Ŭ���� ���
	HWND CreateWidow(const wchar_t* className, const wchar_t* windowName, int width, int height);

}