#include "Window.h"
RECT g_rtClient;
HWND g_hwnd;
Window* g_pWindow = nullptr;
//윈도우 프로시저, 윈도우의 각종 메세지 처리 함수
//(1인자 - 핸들 / 2인자 - 메세지 / 3인자 - 핸들,정수 받아들일때 / 4인자 - 포인터값 전달) 
//WM = 윈도우 메세지
LRESULT  CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    g_pWindow -> MsgProc(hwnd, msg, wparam, lparam);
    switch (msg)
    {
    case WM_SIZE://윈도우 창크기 변경 됐을때
    {
        UINT iWidth = LOWORD(lparam);
        UINT iHeight = HIWORD(lparam);
        g_pWindow->ResizeDevice(iWidth, iHeight);
    }break;
    case WM_DESTROY: //WM_DESTORY = 창을 닫을 때 응용 프로그램에서 창에 할당한 자원을 해제하거나 후처리 작업을 할 수 있게 전달하는 윈도우 메시지
    {
        //메시지 큐에 WM_QUIT 메시지를 붙임
        PostQuitMessage(0);
    }break;
    default:
        //DefWindowProc = 윈도우의 공통적인 동작을 처리(ex. 창의 크기 축소, 확대 등)
        return DefWindowProc(hwnd, msg, wparam, lparam);
    }
    return 0;
}
LRESULT  Window::MsgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    return 0;
}
void Window::ResizeDevice(UINT iWidth, UINT iHeight)
{

}

//1. 윈도우 클래스 등록 / 허가 (운영체제)
BOOL Window::SetWinClass(HINSTANCE hInstance)
{
    m_hInsatance = hInstance;
    WNDCLASS wc;

    //memset과 같이 윈도우클래스 메모리 세팅
    ZeroMemory(&wc, sizeof(WNDCLASS));

    //CS = 클래스 / CS_HREDRAW = 윈도우의 이동이나 크기 조절시 클라이언트의 가로의 크기가 변경되면 다시 그림
    //              CS_VREDRAW = 윈도우의 이동이나 크기 조절시 클라이언트의 세로의 크기가 변경되면 다시 그림
    //즉, 가로 세로 크기 변경 가능
    //wc.style = CS_HREDRAW | CS_VREDRAW;
    //wc.style = WS_THICKFRAME;

    //lp = 포인터 / 함수 주소 / 윈도우 프로시저
    wc.lpfnWndProc = WndProc;

    //운영체제가 관리하는 번호 = 주민등록번호
    wc.hInstance = hInstance;

    //sz = 스트링 / L = LONG / 운영체제 등록 이름 / 설정된 이름 모두 곳곳마다 같아야함
    wc.lpszClassName = L"LJS_WINDOW";

    //Stock = 미리 만들어져서 가지고 있는것
    //윈도우 클래스 백그라운드
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    if (RegisterClass(&wc) == false)
    {
        return FALSE;
    }
    return TRUE;
}
//2. 윈도우 생성
BOOL Window::SetWindow(const WCHAR* szTitle, int iWidth, int iHeight)
{
    //시작점,가로,세로
    RECT rt = { 0,0, iWidth , iHeight };

    //윈도우 창크기 조정 세팅
    AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, FALSE);

    //(lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)
    m_hWnd = CreateWindow(
        L"LJS_WINDOW",
        szTitle,
        WS_OVERLAPPEDWINDOW,
        0, 0,
        rt.right - rt.left,
        rt.bottom - rt.top,
        NULL,
        NULL,
        m_hInsatance,
        NULL);
    if (m_hWnd == NULL)
    {
        return FALSE;
    }
    g_hwnd = m_hWnd;
    //윈도우 클라이언트 영역에서의 좌표값 반환
    GetClientRect(m_hWnd, &m_rtClient);

    //윈도우 화면 좌표 값으로 위치 정보를 반환
    GetWindowRect(m_hWnd, &m_rtWindow);

    g_rtClient = m_rtClient;

    ShowWindow(m_hWnd, SW_SHOW);
    return TRUE;
}
bool Window::WinRun()
{
    MSG msg;

    //PeekMessage = 메세지 큐에 메세지가 있는지 없는지 검사하고, msg 구조체에 메세지를 저장해주는 함수
    //메세지큐에 메세지가 없으면 블록
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return false;
        }
        TranslateMessage(&msg);// 메세지 번역
        DispatchMessage(&msg);// 윈도우프로시져 호출
    }

    return true;
}
Window:: Window()
{
    g_pWindow = this;
}