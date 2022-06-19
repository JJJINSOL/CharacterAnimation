#include "WriteFont.h"
void WriteFont::DeleteDeviceResize()
{
	if (m_pd2dColorBrush)m_pd2dColorBrush->Release();
	if (m_pd2dRT)m_pd2dRT->Release();
}
bool WriteFont ::Init()
{
	//DirectWrite 로 텍스트를 그리는 방법
	//1. DirectWrite 팩토리 생성 -> DWriteCreateFactory - dwrite.h, dwrite.lib
	//2. 팩토리로 텍스트 포맷 생성 -> CreateTextFormat
	//3. Direct2D 렌더타겟의 DrawTextW 메소드를 사용해 그린다.

	//Direct2D 를 구성하는 객체들은 ID2D1Resource 인터페이스로부터 상속받아서 구현되고, 
	//ID2D1Factory 객체는 ID2D1Resource 를 상속받아 만들어진 객체를 생성하고 사용할 수 있도록 해줌
	// 
	//D2D1CreateFactory - Direct2D 리소스를 만드는 데 사용할 수 있는 팩터리 개체
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pd2dFactory);
	if (SUCCEEDED(hr))
	{                                   
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,        //프로세서 공유 모드
								 __uuidof(IDWriteFactory),          //COM 관련 인자  
								 (IUnknown**)(&m_pWriteFactory));   //반환값
		if (SUCCEEDED(hr))
		{
			hr = m_pWriteFactory->CreateTextFormat( L"궁서", //폰트 이름
													NULL, //폰트 컬렉션(폰트 안에 들어있는 다른 종류)
													DWRITE_FONT_WEIGHT_NORMAL, //굵기
													DWRITE_FONT_STYLE_NORMAL, //스타일
													DWRITE_FONT_STRETCH_NORMAL, //폰트 속성 중 하나
													20, //크기
													L"ko-kr", //언어 지역 이름 L"en-us"
													&m_pd2dTextFormat); //결과 저장되는 포인터
			hr = m_pWriteFactory->CreateTextFormat( L"Imprint MT Shadow",
													NULL,
													DWRITE_FONT_WEIGHT_NORMAL,
													DWRITE_FONT_STYLE_NORMAL,
													DWRITE_FONT_STRETCH_NORMAL,
													50,
													L"en-us",
													&m_pd2dMTShadowTF);
		}
	}

	if (SUCCEEDED(hr))
	{
		return true;
	}
	return false;
}
bool WriteFont::SetRenderTarget(IDXGISurface1* pSurface)
{
	//RenderTarget ->  Direct2D는 출력을 위해서 이 인터페이스 사용,ID2D1Resource 에 상속되어 있음

	UINT dpi = GetDpiForWindow(g_hwnd);

	//랜더링 옵션 설정 - D2D1_RENDER_TARGET_PROPERTIES 구조체-----------------------------------
	D2D1_RENDER_TARGET_PROPERTIES rtp;
	ZeroMemory(&rtp, sizeof(D2D1_RENDER_TARGET_PROPERTIES));

	//하드웨어가 사용가능 할 시 하드웨어로 그렇지 않을 경우는 자동으로 소프트웨어를 사용하여 랜더링
	rtp.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;

	rtp.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);

	//화면이 확대 될 경우에 출력된 스트링이 흐려지는 현상을 보정하기 위해 사용
	rtp.dpiX = dpi;
	rtp.dpiY = dpi;

	//GDI와 호환이 없고 Direct3D 명령 스트림의 사용으로 랜더 타켓이 생성
	rtp.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	
	//하드웨어 랜더링에 필요한 최소한의 Direct3D 기능 수준을 설정하는 것
	rtp.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	//랜더 타겟 뷰 생성
	HRESULT hr = m_pd2dFactory->CreateDxgiSurfaceRenderTarget(pSurface,	&rtp,&m_pd2dRT);
	//--------------------------------------------------------------------------------------------
	if (FAILED(hr))
	{
		return false;
	}
	//Direct2D에서도 영역을채우는 개념으로 Brush 를 사용해야 함
	if (FAILED(m_pd2dRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_pd2dColorBrush)))
	{
		return false;
	}
	return true;
}
bool WriteFont::Frame()
{
	return true;
}
void WriteFont::Draw(std::wstring msg, RECT rt, D2D1::ColorF color,IDWriteTextFormat* tf)
{
	m_pd2dRT->BeginDraw();// RenderTarget 을 사용한다.

	//D2D_RECT_F 구조체 -> 각 꼭지점에 의해 정의된 사각형
	D2D_RECT_F fRT;
	fRT.top = rt.top;
	fRT.left = rt.left;
	fRT.right = rt.right;
	fRT.bottom = rt.bottom;

	m_pd2dColorBrush->SetColor(color);
	if (tf == nullptr)
	{
		//글씨 그리기                               정의한 폰트
		m_pd2dRT->DrawText(msg.c_str(), msg.size(), m_pd2dTextFormat, &fRT, m_pd2dColorBrush);
	}
	else
	{
		m_pd2dRT->DrawText(msg.c_str(), msg.size(), tf, &fRT, m_pd2dColorBrush);
	}
	m_pd2dRT->EndDraw();// RenderTarget 을 끝낸다. 이 함수가 호출되면 화면에 출력된다.
}
bool WriteFont::Render()
{
	m_pd2dRT->BeginDraw();
	/*D2D_RECT_F rt;
	rt.top = 0;
	rt.left = 0;
	rt.right = g_rtClient.right;
	rt.bottom = g_rtClient.bottom;
	std::wstring msg = L"FPS 학원";
	m_pd2dRT->DrawText(msg.c_str(), msg.size(),m_pd2dTextFormat,&rt,m_pd2dColorBrush);*/
	m_pd2dRT->EndDraw();
	return true;
}
bool WriteFont::Release()
{
	if (m_pd2dMTShadowTF)m_pd2dMTShadowTF->Release();
	if (m_pd2dTextFormat)m_pd2dTextFormat->Release();
	if (m_pd2dColorBrush)m_pd2dColorBrush->Release();
	if (m_pd2dRT)m_pd2dRT->Release();
	if (m_pWriteFactory)m_pWriteFactory->Release();
	if (m_pd2dFactory)m_pd2dFactory->Release();
	return true;
}