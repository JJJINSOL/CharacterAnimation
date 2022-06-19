#pragma once
#include <winsock2.h>
#include <windows.h>
#include <d3d11.h>
#include <dxgidebug.h>
#include <tchar.h>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <string>
#include <functional>
#include <iostream>
#include <atlconv.h> // A2W
#include "Collision.h"
#include <wrl.h> 
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
#include "TMath.h"
using namespace Microsoft::WRL;

#pragma comment	(lib, "d3d11.lib")

#ifdef _DEBUG
#pragma comment	(lib, "CoreLib_d.lib")
#else
#pragma comment	(lib, "CoreLib_r.lib")
#endif

#pragma comment	(lib, "ws2_32.lib")

using namespace std;

#define BASIS_EPSILON		((FLOAT)  0.001f)
#define BASIS_PI			((FLOAT)  3.141592654f)
#define DegreeToRadian( degree ) ((degree) * (BASIS_PI / 180.0f))
#define RadianToDegree( radian ) ((radian) * (180.0f / BASIS_PI))
#define MAKECOLOR_ARGB(a, r, g, b)	(((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)
#define IS_IN_RANGE(value,r0,r1) (( ((r0) <= (value)) && ((value) <= (r1)) ) ? 1 : 0)


#define randf(x) (x*rand()/(float)RAND_MAX)
#define randf2(x,off) (off+x*rand()/(float)RAND_MAX)
#define randstep(fMin,fMax) (fMin+((float)fMax-(float)fMin)*rand()/(float)RAND_MAX)
#define clamp(x,MinX,MaxX) if (x>MaxX) x=MaxX; else if (x<MinX) x=MinX;

extern RECT		g_rtClient;
extern HWND		g_hwnd;
extern float	g_fSecPerFrame;
extern float	g_fGameTimer;
extern POINT	g_ptMouse;

static std::wstring to_mw(const std::string& _src)
{
	USES_CONVERSION;//유니코드 <-> 멀티바이트
	return std::wstring(A2W(_src.c_str()));//멀티바이트 -> 유니코드
};

static std::string to_wm(const std::wstring& _src)
{
	USES_CONVERSION;
	return std::string(W2A(_src.c_str()));//유니코드 -> 멀티바이트
};

template<class T>
class Singleton //객체의 인스턴스가 오직 1개만 생성되는 패턴
{
public://T->자식
	static T& Get()
	{
		static T theSingle;
		return theSingle;
	}
};

static void DisplayText(const char* fmt, ...)
{
	//가변인자(파라미터의 수가 정해지지않은것)
	va_list arg;                    //각 가변인자의 시작 주소를 가리키는 포인터(목록)
	va_start(arg, fmt);             //가변인자 중 첫번째 인자의 주소를 가르킴(포인터,고정인수)
	char buf[1024 + 256] = { 0, };  //
	vsprintf_s(buf, fmt, arg);      //조립된 문자열을 화면이 아닌 버퍼에 전달받음
	OutputDebugStringA((char*)buf); //디버깅 중 출력

	va_end(arg);                    //사용한 가변 인자 끝냄
}
static void MemoryReporting()
{
#if defined(DEBUG) | defined(_DEBUG)
	HMODULE dxgidebugdll = GetModuleHandleW(L"dxgidebug.dll");
	decltype(&DXGIGetDebugInterface) GetDebugInterface = reinterpret_cast<decltype(&DXGIGetDebugInterface)>(GetProcAddress(dxgidebugdll, "DXGIGetDebugInterface"));
	IDXGIDebug* debug;
	GetDebugInterface(IID_PPV_ARGS(&debug));
	OutputDebugStringW(L"Starting Live Direct3D Object Dump:\r\n");
	debug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_ALL);//개체 정보 보고
	OutputDebugStringW(L"Completed Live Direct3D Object Dump.\r\n");
	debug->Release();
#endif
}

#define GAME_START int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow ){   Sample core;   
#define GAME_WIN(s,x,y) if (core.SetWinClass(hInstance) == FALSE) return 1;   if (core.SetWindow(L#s, x, y) == FALSE) return 1;   core.GameRun();    return 1;}
#define SIMPLE_WIN() if (core.SetWinClass(hInstance) == FALSE) return 1;   if (core.SetWindow() == FALSE) return 1;   core.GameRun();    return 1;}

//GAME_START, GAME_WIN 실행시키는 매크로
#define GAME_RUN(s,x,y) GAME_START; GAME_WIN(s,x,y);

//GAME_START, SIMPLE_WIN 실행시키는 매크로
#define RUN() GAME_START; SIMPLE_WIN();

#define BEGIN_START(S) friend class Singleton<S>

//int WINAPI wWinMain(HINSTANCE hInstance,
//					  HINSTANCE hPrevInstance,
//					  LPWSTR lpCmdLine,
//					  int nCmdShow)
//{
//	TWindow win;
// ------------------------------------------------------------------------
//	if (win.WinRegisterClass(hInstance) == FALSE) return 1;
//	if (win.SetWindow(L"Win Sample") == FALSE) return 1;
//	win.WinRun();
//	return 1;
//}