#pragma once
#include "BaseMgr.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
#pragma comment	(lib, "d3dcompiler.lib")
class Texture
{
public:
	int				m_iIndex;
	std::wstring	m_csName;
	std::wstring	m_szMsg;
	TCHAR			m_szBuffer[256];

	//인터페이스 필요
	//텍스쳐와 관련된 인터페이스
	ComPtr<ID3D11Texture2D> m_pTexture;
	//ID3D11Texture2D* m_pTexture;//텍스쳐는 파일인데 파일을 로드하면 이게 만들어짐
	
	//view = 응용프로그램에서 shader로 전달되는 리소스이다
	ComPtr<ID3D11ShaderResourceView> m_pSRV;
	//ID3D11ShaderResourceView* m_pSRV;//텍스쳐를 GPU에 전달할때는 이게 되야됨
	
	D3D11_TEXTURE2D_DESC		m_TextureDesc;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	bool	Load(ID3D11Device* pd3dDevice, std::wstring filename);
public:
	Texture();
	virtual ~Texture();
};

class TextureMgr : public BaseMgr<Texture, TextureMgr>
{
	friend class Singleton<TextureMgr>;
private:
	TextureMgr() {};
public:
	~TextureMgr() {};
};
#define I_Texture TextureMgr::Get()