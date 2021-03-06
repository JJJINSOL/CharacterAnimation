#include "Sample.h"
#include "ObjectMgr.h"
void	Sample::CreateResizeDevice(UINT iWidth, UINT iHeight)
{
	int k = 0;
}
void	Sample::DeleteResizeDevice(UINT iWidth, UINT iHeight)
{
	int k = 0;
}
bool	Sample::Init()
{			
	std::vector<std::wstring> listname;
	listname.push_back(L"../../data/fbx/Man.fbx");
	
	// 0 ~ 60		idel
	// 61 ~ 91		walk;
	// 92 ~ 116		run
	// 120 ~ 205	jump
	// 205 ~ 289	attack

	I_ObjectMgr.Set(m_pd3dDevice.Get(), m_pImmediateContext.Get());
	m_FbxObj.resize(listname.size());
	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		Fbx* pFbx = &m_FbxObj[iObj];
		pFbx->Init();

		pFbx->m_pd3dDevice = m_pd3dDevice.Get();
		pFbx->m_pContext = m_pImmediateContext.Get();

		pFbx->m_pMeshImp = I_ObjectMgr.Load(listname[iObj]);
		pFbx->m_DrawList.resize(pFbx->m_pMeshImp->m_DrawList.size());
		pFbx->SetPosition(T::TVector3(iObj * 100.0f, 0, 0));
		for (int iDraw = 0; iDraw < pFbx->m_pMeshImp->m_DrawList.size(); iDraw++)
		{
			pFbx->m_pMeshImp->m_DrawList[iDraw]->m_pContext = m_pImmediateContext.Get();
		}
	}
	m_pMainCamera->CreateViewMatrix(T::TVector3(0, 50.0f, -130.0f),T::TVector3(0, 0.0f, 0));//0, 25.0f, -50.0f
	return true;
}
bool	Sample::Frame()
{		
	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		m_FbxObj[iObj].Frame();
	}
	return true;
}
bool	Sample::Render()
{		
	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		m_FbxObj[iObj].SetMatrix(nullptr,&m_pMainCamera->m_matView,&m_pMainCamera->m_matProj);
		m_FbxObj[iObj].Render();
	}
	std::wstring msg = L"FPS:";
	msg += std::to_wstring(m_GameTimer.m_iFPS);
	msg += L"  GT:";
	msg += std::to_wstring(m_GameTimer.m_fTimer);
	m_dxWrite.Draw(msg, g_rtClient, D2D1::ColorF(0, 0, 1, 1));
	return true;
}
bool	Sample::Release()
{
	for (int iObj = 0; iObj < m_FbxObj.size(); iObj++)
	{
		m_FbxObj[iObj].Release();
	}
	return true;
}
Sample::Sample()
{

}
Sample::~Sample()
{}
RUN();