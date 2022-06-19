#include "Timer.h"
float  g_fSecPerFrame = 0.0f;//초당 프레임
float  g_fGameTimer = 0.0f;//타이머
bool Timer::Init()
{
    m_fSecondPerFrame = 0.0f;
    m_fTimer = 0.0f;
    m_dwBeforeTime = timeGetTime();
    return false;
}

bool Timer::Frame()
{
    DWORD dwCurrentTime = timeGetTime();//현재 시간
    DWORD dwElapseTime = dwCurrentTime - m_dwBeforeTime;//경과시간

    //m_fSecondPerFrame = dwElapseTime / 1000.0f;//초당 프레임
    m_fSecondPerFrame = min(0.1f, dwElapseTime / 1000.0f);

    m_fTimer += m_fSecondPerFrame;

    g_fSecPerFrame = m_fSecondPerFrame;
    g_fGameTimer = m_fTimer;

    //프레임 경과 시간 - fps (frame per second)
    m_fFPSTimer += m_fSecondPerFrame;
    if (m_fFPSTimer >= 1.0f)
    {
        m_iFPS = m_iFPSCounter;
        m_iFPSCounter = 0;
        m_fFPSTimer -= 1.0f;
    }
    m_iFPSCounter++;

    m_dwBeforeTime = dwCurrentTime;
    return false;
}

bool Timer::Render()
{
    return false;
}

bool Timer::Release()
{
    return false;
}
