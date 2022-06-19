#pragma once
#include "BaseMgr.h"
#include <d3dcompiler.h>
class Shader
{
public:
	int				m_iIndex;
	std::wstring	m_csName;
	std::wstring	m_szMsg;
	TCHAR			m_szBuffer[256];

	ID3D11VertexShader* m_pVertexShader = nullptr;
	ID3D11PixelShader* m_pPixelShader = nullptr;

	ID3DBlob* m_pVSCodeResult = nullptr;
	ID3DBlob* m_pErrorMsgs = nullptr;
	ID3DBlob* m_pPSCodeResult = nullptr;
public:
	virtual bool    CreateVertexShader(ID3D11Device* pd3dDevice,
		std::wstring filename, std::string entry = "VS");
	virtual bool    CreatePixelShader(ID3D11Device* pd3dDevice,
		std::wstring filename, std::string entry = "PS");
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	bool	Load(ID3D11Device* pd3dDevice, std::wstring filename);
public:
	Shader();
	virtual ~Shader();
};
//-----------------------------------------------------------------------------
class ShaderMgr : public BaseMgr<Shader, ShaderMgr>
{
	friend class Singleton<ShaderMgr>;
public:
	Shader* CreateVertexShader(ID3D11Device* pd3dDevice,std::wstring filename, std::string entry);
	Shader* CreatePixelShader(ID3D11Device* pd3dDevice,	std::wstring filename, std::string entry);
private:
	ShaderMgr();
public:
	~ShaderMgr();
};
#define I_Shader ShaderMgr::Get()