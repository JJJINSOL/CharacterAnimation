#pragma once
#include "DxObject.h"
class Object2D : public DxObject
{
public:
	std::vector<SimpleVertex> m_VertexList2D;
	RECT			m_rtSource;//소스 이미지의 u,v 좌표/텍스쳐
	RECT			m_rtDraw;//화면의 드로우 영역
	T::TVector4		m_vColor;
	T::TVector2		m_vPos;
	T::TVector2		m_vDirection;//방향
public:
	float m_fAlpha = 0.0f;
	bool m_bFadeIn = false;
	bool m_bFadeOut = false;
	virtual void FadeIn();
	virtual void FadeOut();

	virtual void SetRectSouce(RECT rt);
	virtual void SetRectDraw(RECT rt);
	virtual void AddPosition(T::TVector2 vPos);//현재위치의 포지션이 덧셈이된다
	virtual void SetPosition(T::TVector2 vPos);//포지션세팅
	virtual void UpdateRectDraw(RECT rt);//커졋다작아졌다
	// 화면좌표 위치를 중점으로 NDC 변환
	virtual void	Convert(T::TVector2 center, float fWidth, float fHeight,std::vector<SimpleVertex>& retList);
	// 화면좌표계를 NDC 변환
	virtual void	Convert(std::vector<SimpleVertex>& list, std::vector<SimpleVertex>& retList);
	
	virtual void	ConvertIndex(T::TVector2 center, float fWidth, float fHeight,
								 std::vector<SimpleVertex>& retList);
	// 화면좌표계를 NDC 변환
	virtual void	ConvertIndex(std::vector<SimpleVertex>& list,
								 std::vector<SimpleVertex>& retList);

	virtual bool SetVertexData() override;
	virtual bool SetIndexData() override;
	virtual bool Frame() override;

	bool Load(ID3D11Device* pd3dDevice, std::wstring filename)
	{
		return true;
	};
	virtual void UpdateData() {}
	Object2D();
	virtual ~Object2D();
};

