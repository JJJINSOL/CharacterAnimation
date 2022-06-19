#define  _CRT_SECURE_NO_WARNINGS
#include "TFbxObj.h"
bool Fbx::Init()
{	
	m_fTime = 61;
	return true;
}
bool Fbx::Frame()
{
	return true;
}
//애니메이션 보간
T::TMatrix Fbx::Interplate(TFbxImporter* pAnimImp, FbxModel* pModel, float fTime)
{
	T::TMatrix matAnim;
	Scene tScene = pAnimImp->m_Scene;


	int iStart = max(tScene.iStart, fTime);
	int iEnd = min(tScene.iEnd, fTime + 1);

	// 보간 = A ~ 7.5f ~ B
	//       9.5f <=10   ~     20 -> 20.1
	Track A = pModel->m_AnimTrack[iStart];
	Track B = pModel->m_AnimTrack[iEnd];
	float s = fTime- (float)iStart; // 0~1

	T::TVector3 pos;
	T::D3DXVec3Lerp(&pos, &A.t, &B.t, s);

	T::TVector3 scale;
	T::D3DXVec3Lerp(&scale, &A.s, &B.s, s);

	T::TQuaternion rotation;
	T::D3DXQuaternionSlerp(&rotation, &A.r, &B.r, s);

	T::TMatrix matScale;
	T::D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z);

	T::TMatrix matRotation;
	T::D3DXMatrixRotationQuaternion(&matRotation, &rotation);
	
	matAnim = matScale * matRotation;
	matAnim._41 = pos.x;
	matAnim._42 = pos.y;
	matAnim._43 = pos.z;

	return matAnim;
}
bool Fbx::Render()
{		
	TFbxImporter* pAnimImp = nullptr;
	if (m_pAnimImporter!=nullptr)
	{
		pAnimImp = m_pAnimImporter;
	}
	else
	{
		pAnimImp = m_pMeshImp;
	}
	m_fTime += g_fSecPerFrame * pAnimImp->m_Scene.iFrameSpeed * m_fDir * 0.33f;// m_fSpeed;
	
	if (idel == true && m_fTime >= 60.0f)
	{
		m_fTime = 0.0f;
	}
	else if (walk == true && m_fTime >= 91.0f)
	{
		m_fTime = 61.0f;
	}
	else if (run == true && m_fTime >= 116.0f)
	{
		m_fTime = 92.0f;
	}
	else if (jump == true && m_fTime >= 205.0f)
	{
		m_fTime = 120.0f;
	}
	else if (attack == true && m_fTime >= 289.0f)
	{
		m_fTime = 205.0f;
	}
	else
	{
		//애니메이션 무한 재생
		if (m_fTime >= pAnimImp->m_Scene.iEnd)
		{
			//m_fDir *= -1.0f;
			m_fTime = pAnimImp->m_Scene.iStart;
		}
	}
	if (Input::Get().GetKey(VK_F1) == KEY_PUSH)
	{
		all = false;
		idel = true;
		walk = false;
		run = false;
		jump = false;
		attack = false;
		m_fTime = 0.0f;
	}
	if (Input::Get().GetKey(VK_F2) == KEY_PUSH)
	{
		all = false;
		idel = false;
		walk = true;
		run = false;
		jump = false;
		attack = false;
		m_fTime = 61.0f;
	}
	if (Input::Get().GetKey(VK_F3) == KEY_PUSH)
	{
		all = false;
		idel = false;
		walk = false;
		run = true;
		jump = false;
		attack = false;
		m_fTime = 92.0f;
	}
	if (Input::Get().GetKey(VK_F4) == KEY_PUSH)
	{
		all = false;
		idel = false;
		walk = false;
		run = false;
		jump = true;
		attack = false;
		m_fTime = 120.0f;
	}
	if (Input::Get().GetKey(VK_F5) == KEY_PUSH)
	{
		all = false;
		idel = false;
		walk = false;
		run = false;
		jump = false;
		attack = true;
		m_fTime = 205.0f;
	}
	if (Input::Get().GetKey(VK_F6) == KEY_PUSH)
	{
		all = true;
		idel = false;
		walk = false;
		run = false;
		jump = false;
		attack = false;
		m_fTime = 0.0f;
	}
	int iFrame = m_fTime;
	iFrame = max(0, min(pAnimImp->m_Scene.iEnd-1, iFrame));

	for (int iObj = 0; iObj < m_pMeshImp->m_DrawList.size(); iObj++)
	{
		FbxModel* pFbxObj = m_pMeshImp->m_DrawList[iObj];		
		if (pFbxObj->m_bSkinned)//스키닝
		{			
			for( auto data : pAnimImp->m_pFbxModelMap)
			{
				std::wstring name = data.first;
				FbxModel* pAnimModel = data.second;
				//매핑
				auto model = m_pMeshImp->m_pFbxModelMap.find(name);
				if (model == m_pMeshImp->m_pFbxModelMap.end())
				{
					continue; // error
				}
				FbxModel* pTreeModel = model->second;
				if (pTreeModel == nullptr)
				{
					continue; // error
				}
				//binepose
				auto binepose = pFbxObj->m_dxMatrixBindPoseMap.find(name);
				if (binepose != pFbxObj->m_dxMatrixBindPoseMap.end()&& pAnimModel)
				{
					TMatrix matInverseBindpose = binepose->second;
					m_matBoneArray.matBoneWorld[pTreeModel->m_iIndex] = matInverseBindpose * Interplate(pAnimImp, pAnimModel, m_fTime);
						//pAnimModel->m_AnimTrack[iFrame].matTrack;
				}
				T::D3DXMatrixTranspose( &m_matBoneArray.matBoneWorld[pTreeModel->m_iIndex],
										&m_matBoneArray.matBoneWorld[pTreeModel->m_iIndex]);
			}
		}
		//스킨 없을때
		else
		{
			for (int inode = 0; inode < m_pMeshImp->m_TreeList.size(); inode++)
			{		
				FbxModel* pFbxModel = m_pMeshImp->m_TreeList[inode];
				if (pFbxModel->m_AnimTrack.size() > 0)
				{					
					m_matBoneArray.matBoneWorld[inode] = Interplate(pAnimImp, pFbxModel, m_fTime);
														//pFbxModel->m_AnimTrack[iFrame].matTrack;
					
				}
				T::D3DXMatrixTranspose(&m_matBoneArray.matBoneWorld[inode],&m_matBoneArray.matBoneWorld[inode]);
			}
		}

		m_pContext->UpdateSubresource(m_pMeshImp->m_pBoneCB, 0, NULL, &m_matBoneArray, 0, 0);
		m_pContext->VSSetConstantBuffers(2, 1, &m_pMeshImp->m_pBoneCB);

		T::TVector3 vLight(cosf(g_fGameTimer) * 100.0f,	100,sinf(g_fGameTimer) * 100.0f);
		T::D3DXVec3Normalize(&vLight, &vLight);
		
		vLight = vLight * -1.0f;
		
		pFbxObj->m_LightConstantList.vLightDir.x = vLight.x;
		pFbxObj->m_LightConstantList.vLightDir.y = vLight.y;
		pFbxObj->m_LightConstantList.vLightDir.z = vLight.z;
		pFbxObj->m_LightConstantList.vLightDir.w = 1.0f;

		pFbxObj->SetMatrix(&m_matWorld, &m_matView, &m_matProj);
		pFbxObj->Render();
	}
	return true;
}
bool Fbx::Release()
{
	return true;
}