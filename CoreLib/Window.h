#pragma once
#include "std.h"
class Window
{
public:
	
	//인스턴스 = 주민등록번호
	HINSTANCE m_hInsatance;
	//핸들 = 이름
	HWND m_hWnd;
	RECT m_rtClient;
	RECT m_rtWindow;

	//1. 윈도우 클래스 등록 / 허가 (운영체제)
	BOOL  SetWinClass(HINSTANCE hInstance);
	//2. 윈도우 생성
	BOOL  SetWindow(const WCHAR* szTitle = L"Sample", 
					int iWidth = 800, 
					int iHeight = 600);
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	virtual void ResizeDevice(UINT iWidth, UINT iHeight);
	bool  WinRun();

	Window();
	virtual ~Window() {}
};

