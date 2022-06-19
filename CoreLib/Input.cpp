#include "Input.h"
POINT g_ptMouse;
DWORD Input::GetKey(DWORD dwKey)
{
	return m_dwKeyState[dwKey];
}
bool Input::Init()
{
	ZeroMemory(&m_dwKeyState, sizeof(DWORD) * 256);
	return true;
}
T::TVector2 Input::GetDelta()
{
	T::TVector2 ret;
	m_fRadianY += (Input::Get().m_ptDeltaMouse.x / (float)g_rtClient.right) * BASIS_PI;
	m_fRadianX += (Input::Get().m_ptDeltaMouse.y / (float)g_rtClient.bottom) * BASIS_PI;
	return TVector2(m_fRadianX, m_fRadianY);
}
bool Input::Frame()
{
	POINT ptOffset = g_ptMouse;
	//화면(스크린)좌표계
	GetCursorPos(&m_ptMouse);
	//클라이언트(윈도우)좌표계
	ScreenToClient(g_hwnd, &m_ptMouse);
	g_ptMouse = m_ptMouse;

	if (m_ptMouse.x < 0 || m_ptMouse.x > g_rtClient.right || m_ptMouse.y < 0 || m_ptMouse.y > g_rtClient.bottom)
	{
		m_bDrag = false;
	}

	// 마우스 버튼 VK_LBUTTON,  VK_RBUTTON, VK_MBUTTON,
	for (int iKey = 0; iKey < 256; iKey++)
	{
		SHORT sKey = GetAsyncKeyState(iKey);//비동기로 해당 키 상태 알려줘!
		// 0000 0000 0000 0000 -> 입력X
		// 1000 0000 0000 0000 -> 무언가 키입력이 있다면 최상위 단위 비트 1이됨
		if (sKey & 0x8000)
		{
			if (m_dwKeyState[iKey] == KEY_FREE)//이전상태는 free인데 지금은 눌러진상태인경우
			{
				m_dwKeyState[iKey] = KEY_PUSH;//키가 눌러지는 순간임
			}
			else
			{	
				//이전상태가 free가 아니고 지금 눌러진 상태라면 계속 누르고 있는 상황
				m_dwKeyState[iKey] = KEY_HOLD;
			}
		}
		else
		{
			//지금 입력이 없음!
			if (m_dwKeyState[iKey] == KEY_PUSH || m_dwKeyState[iKey] == KEY_HOLD)
			{
				//이전에는 입력이 있었으니 눌러졌던 키에서 손을 떼는 순간임
				m_dwKeyState[iKey] = KEY_UP;
			}
			else
			{
				//이전도 지금도 입력없음!
				m_dwKeyState[iKey] = KEY_FREE;
			}
		}
	}
	m_dwMouseState[0] = m_dwKeyState[VK_LBUTTON];
	m_dwMouseState[1] = m_dwKeyState[VK_RBUTTON];
	m_dwMouseState[2] = m_dwKeyState[VK_MBUTTON];

	if (m_dwMouseState[0] == KEY_PUSH)
	{
		m_bDrag = true;
		m_ptMouseClick = m_ptMouse;
	}

	if (m_dwMouseState[0] == KEY_UP)
	{
		m_bDrag = false;
	}
	if (m_bDrag)
	{
		m_ptDeltaMouse.x = ptOffset.x - m_ptMouse.x;
		m_ptDeltaMouse.y = ptOffset.y - m_ptMouse.y;
	}
	return true;

	return true;
}
bool Input::Render()
{
	return true;
}
bool Input::Release() 
{
	return true;
}
Input::Input()
{
	m_ptDeltaMouse.x = 0.0f;
	m_ptDeltaMouse.y = 0.0f;
}
Input::~Input()
{

}