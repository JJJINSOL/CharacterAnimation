#define  _CRT_SECURE_NO_WARNINGS
#include "TFbxImporter.h"

TMatrix TFbxImporter::DxConvertMatrix(TMatrix m)
{
	//y, z 값 반대로 저장 -> DX <-> MAX
	TMatrix mat;
	mat._11 = m._11; mat._12 = m._13; mat._13 = m._12;
	mat._21 = m._31; mat._22 = m._33; mat._23 = m._32;
	mat._31 = m._21; mat._32 = m._23; mat._33 = m._22;
	mat._41 = m._41; mat._42 = m._43; mat._43 = m._42;
	mat._14 = mat._24 = mat._34 = 0.0f;
	mat._44 = 1.0f;
	return mat;
}
TMatrix TFbxImporter::ConvertMatrix(FbxMatrix& m)
{
	//fbx 매트릭스를 DX에
	TMatrix mat;
	float* pMatArray = reinterpret_cast<float*>(&mat); //reinterpret_cast - 타입 캐스트
	double* pSrcArray = reinterpret_cast<double*>(&m);
	for (int i = 0; i < 16; i++)
	{
		pMatArray[i] = pSrcArray[i];
	}
	return mat;
}
TMatrix TFbxImporter::ConvertAMatrix(FbxAMatrix& m)
{
	//fbxa 매트릭스를 DX에
	TMatrix mat;
	float* pMatArray = reinterpret_cast<float*>(&mat);
	double* pSrcArray = reinterpret_cast<double*>(&m);
	for (int i = 0; i < 16; i++)
	{
		pMatArray[i] = pSrcArray[i];
	}
	return mat;
}
void TFbxImporter::ParseAnimation()
{

	FbxTime::SetGlobalTimeMode(FbxTime::eFrames30);
	FbxAnimStack* stack = m_pFbxScene->GetSrcObject<FbxAnimStack>(0);
	if (stack == nullptr) return;

	FbxString TakeName = stack->GetName();
	FbxTakeInfo* TakeInfo = m_pFbxScene->GetTakeInfo(TakeName);
	FbxTimeSpan LocalTimeSpan = TakeInfo->mLocalTimeSpan;
	
	FbxTime start		= LocalTimeSpan.GetStart();
	FbxTime end			= LocalTimeSpan.GetStop();
	FbxTime Duration	= LocalTimeSpan.GetDuration();//지속

	FbxTime::EMode TimeMode = FbxTime::GetGlobalTimeMode();

	FbxLongLong s = start.GetFrameCount(TimeMode);
	FbxLongLong n = end.GetFrameCount(TimeMode);

	m_Scene.iStart = s;
	m_Scene.iEnd = n;
	m_Scene.iFrameSpeed = 30;

	// 1초에 30 frame 
	// 1Frame = 160 Tick
	// 50 Frame 
	FbxTime time;
	Track tTrack;
	for (FbxLongLong t = s; t <= n; t++)
	{
		time.SetFrame(t, TimeMode);
		for (int iObj = 0; iObj < m_TreeList.size(); iObj++)
		{
			FbxAMatrix matGlobal =	m_TreeList[iObj]->m_pFbxNode->EvaluateGlobalTransform(time);			
			tTrack.iFrame = t;
			tTrack.matTrack = DxConvertMatrix(ConvertAMatrix(matGlobal));//행렬 y,z 값 바꾸기
			// 행렬분해
			// 행렬을 분해(SRT)
			T::D3DXMatrixDecompose(&tTrack.s, &tTrack.r, &tTrack.t,	&tTrack.matTrack);
			m_TreeList[iObj]->m_AnimTrack.push_back(tTrack);		
		}
	}	
}
void TFbxImporter::PreProcess(FbxNode* node, FbxModel* fbxParent)
{
	FbxModel* fbx = nullptr;
	if (node != nullptr)
	{
		//노드 정보
		fbx = new FbxModel;
		fbx->m_pFbxParent = node->GetParent();
		fbx->m_pFbxNode = node;
		fbx->m_csName = to_mw(node->GetName());
		fbx->m_pParentObj = fbxParent;
		fbx->m_iIndex = m_TreeList.size();
		m_TreeList.push_back(fbx);
		m_pFbxNodeMap.insert(std::make_pair(node, fbx->m_iIndex));
		m_pFbxModelMap.insert(std::make_pair(fbx->m_csName, fbx));
	}
	// camera, light, mesh, shape, animation
	FbxMesh* pMesh = node->GetMesh();	
	if (pMesh)//매시 있으면
	{
		m_DrawList.push_back(fbx);		
	}
	int iNumChild = node->GetChildCount();  //자식 노드 개수
	for (int iNode = 0; iNode < iNumChild; iNode++)
	{
		FbxNode* child = node->GetChild(iNode);
		PreProcess(child, fbx);
	}
}
bool TFbxImporter::Load(std::string filename)
{
	Init();

	bool bRet = m_pFbxImporter->Initialize(filename.c_str());
	bRet = m_pFbxImporter->Import(m_pFbxScene);//가져오기
	m_pRootNode = m_pFbxScene->GetRootNode();
	PreProcess(m_pRootNode, nullptr);
	ParseAnimation();
	for (int iObj = 0; iObj < m_DrawList.size(); iObj++)
	{
		//datamesh 가져와~
		ParseMesh(m_DrawList[iObj]);		
	}
		
	Release();
	return true;
}
bool TFbxImporter::ParseMeshSkinning(FbxMesh* pFbxMesh, FbxModel* pObject)
{
	int iDeformerCount = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);
	if (iDeformerCount == 0)
	{
		return false;
	}
	// 정점의 개수와 동일한다.
	int iVertexCount = pFbxMesh->GetControlPointsCount();//vertex 개수
	pObject->m_WeightList.resize(iVertexCount);

	for (int dwDeformerIndex = 0; dwDeformerIndex < iDeformerCount; dwDeformerIndex++)
	{
		auto pSkin = reinterpret_cast<FbxSkin*>(pFbxMesh->GetDeformer(dwDeformerIndex, FbxDeformer::eSkin));
		DWORD dwClusterCount = pSkin->GetClusterCount();
		// dwClusterCount의 행렬이 전체 정점에 영향을 주었다.
		for (int dwClusterIndex = 0; dwClusterIndex < dwClusterCount; dwClusterIndex++)
		{
			auto pCluster = pSkin->GetCluster(dwClusterIndex);
			////
			FbxAMatrix matXBindPose;
			FbxAMatrix matReferenceGlobalInitPosition;
			pCluster->GetTransformLinkMatrix(matXBindPose);
			pCluster->GetTransformMatrix(matReferenceGlobalInitPosition);
			FbxMatrix matBindPose = matReferenceGlobalInitPosition.Inverse() * matXBindPose;

			TMatrix matInvBindPos = DxConvertMatrix(ConvertMatrix(matBindPose));
			matInvBindPos = matInvBindPos.Invert();
			int  iBoneIndex = m_pFbxNodeMap.find(pCluster->GetLink())->second;
			std::wstring name = m_TreeList[iBoneIndex]->m_csName;
			pObject->m_dxMatrixBindPoseMap.insert(make_pair(name, matInvBindPos));

			int  dwClusterSize = pCluster->GetControlPointIndicesCount();			
			// 영향을 받는 정점들의 인덱스
			int* pIndices = pCluster->GetControlPointIndices();
			double* pWeights = pCluster->GetControlPointWeights();
			// iBoneIndex의 영향을 받는 정점들이 dwClusterSize개 있다.
			for (int i = 0; i < dwClusterSize; i++)
			{
				// n번 정점(pIndices[i])은 iBoneIndex의 행렬에 
				// pWeights[i]의 가중치로 적용되었다.
				int iVertexIndex = pIndices[i];
				float fWeight = pWeights[i];
				pObject->m_WeightList[iVertexIndex].InsertWeight(iBoneIndex, fWeight);
			}
		}
	}
	return true;
}
void TFbxImporter::ParseMesh(FbxModel* pObject)//datamesh를 가져와라
{
	FbxMesh* pFbxMesh = pObject->m_pFbxNode->GetMesh();

	pObject->m_bSkinned = ParseMeshSkinning(pFbxMesh, pObject);
	// 기하행렬(초기 정점 위치를 변환할 때 사용)
	FbxAMatrix geom;
	//월드 좌표를 로컬 좌표로 변환하는 행렬
	FbxVector4 trans = pObject->m_pFbxNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 rot = pObject->m_pFbxNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 scale = pObject->m_pFbxNode->GetGeometricScaling(FbxNode::eSourcePivot);
	geom.SetT(trans);
	geom.SetR(rot);
	geom.SetS(scale);

	FbxAMatrix normalMatrix = geom;
	normalMatrix = normalMatrix.Inverse();
	normalMatrix = normalMatrix.Transpose();

	// 레이어 ( 1번에 랜더링, 여러번에 걸쳐서 랜더링 개념)
	std::vector<FbxLayerElementUV*>				VertexUVSet;    //정점 UV
	std::vector<FbxLayerElementVertexColor*>	VertexColorSet; //정점 컬러
	std::vector<FbxLayerElementMaterial*>		MaterialSet;    //질감

	int iLayerCount = pFbxMesh->GetLayerCount();

	for (int iLayer = 0; iLayer < iLayerCount; iLayer++)
	{
		FbxLayer* pFbxLayer = pFbxMesh->GetLayer(iLayer);
		if (pFbxLayer->GetUVs() != nullptr)
		{
			VertexUVSet.push_back(pFbxLayer->GetUVs());
		}
		if (pFbxLayer->GetVertexColors() != nullptr)
		{
			VertexColorSet.push_back(pFbxLayer->GetVertexColors());
		}
		if (pFbxLayer->GetMaterials() != nullptr)
		{
			MaterialSet.push_back(pFbxLayer->GetMaterials());
		}
	}

	//  1개의 오브젝트가 여러장의 텍스처를 사용한다.
	//  각각의 텍스처를 이름을 얻고 저장한다.
	//  어떤 페이스(폴리곤)가 어떤 텍스처를 사용하니?
	//  같은 텍스처를 사용하는 폴리곤들 끼리 저장한다.
	
	//한 객체가 사용하는 material 개수
	int iNumMtrl = pObject->m_pFbxNode->GetMaterialCount();

	for (int iMtrl = 0; iMtrl < iNumMtrl; iMtrl++)
	{
		FbxSurfaceMaterial* pSurface = pObject->m_pFbxNode->GetMaterial(iMtrl);
		if (pSurface)
		{
			std::string texturename = ParseMaterial(pSurface);
			std::wstring szTexFileName = L"../../data/fbx/";
			szTexFileName += to_mw(texturename);

			pObject->m_szTexFileList.push_back(szTexFileName);

			pObject->m_pTextureList.push_back(I_Texture.Load(pObject->m_szTexFileList[iMtrl]));
		}
	}
	if(pObject->m_szTexFileList.size() > 0)
	{
		pObject->m_szTexFileName = pObject->m_szTexFileList[0];
	}
	if (iNumMtrl > 0)
	{
		pObject->m_pSubVertexList.resize(iNumMtrl);
		pObject->m_pSubIWVertexList.resize(iNumMtrl);
	}
	else //material 최소 1개여야됨
	{
		pObject->m_pSubVertexList.resize(1);
		pObject->m_pSubIWVertexList.resize(1);
	}
	//============================================================
	//FBX MESH / 
	int iBasePolyIndex = 0;

	// 삼각형, 사각형
	int iNumPolyCount = pFbxMesh->GetPolygonCount();//폴리곤 총 개수(1 폴리곤 = 2 페이스)

	FbxVector4* pVertexPositions = pFbxMesh->GetControlPoints(); //정점 좌표

	int iNumFace = 0;
	for (int iPoly = 0; iPoly < iNumPolyCount; iPoly++)
	{
		//FBX는 한개의 폴리곤이 삼각형만 지원하는 것이 아니어서
		//GetPolygonSize를 통해 1개의 폴리곤을 구성하는 꼭지점 인덱스 수를 얻어오기
		int iPolySize = pFbxMesh->GetPolygonSize(iPoly);
		iNumFace = iPolySize - 2;//꼭지점 개수에서 -2하면 페이스 개수

		int iSubMtrl = 0;
		if (iNumMtrl >= 1 && MaterialSet[0] != nullptr)
		{
			//서브 material 인덱스 얻기
			iSubMtrl = GetSubMaterialIndex(iPoly, MaterialSet[0]);
		}
		for (int iFace = 0; iFace < iNumFace; iFace++)
		{
			// 1  2
			// 0  3
			// (max)021,032 ->  (dx)012, 023
			int VertexIndex[3] = { 0, iFace + 2, iFace + 1 };//정점 인덱스
				 
			int CornerIndex[3];//꼭지점 인덱스
			//GetPolygonVertex - 1개의 폴리곤을 구성하는 꼭지점 인덱스 집합
			CornerIndex[0] = pFbxMesh->GetPolygonVertex(iPoly, VertexIndex[0]);
			CornerIndex[1] = pFbxMesh->GetPolygonVertex(iPoly, VertexIndex[1]);
			CornerIndex[2] = pFbxMesh->GetPolygonVertex(iPoly, VertexIndex[2]);
			for (int iIndex = 0; iIndex < 3; iIndex++)
			{
				Vertex tVertex;	
				// Max(x,z,y) ->(dx)x,y,z    
				FbxVector4 v = pVertexPositions[CornerIndex[iIndex]];
				v = geom.MultT(v);
				tVertex.p.x = v.mData[0];
				tVertex.p.y = v.mData[2];
				tVertex.p.z = v.mData[1];

				// uv
				int u[3];
				u[0] = pFbxMesh->GetTextureUVIndex(iPoly, VertexIndex[0]);
				u[1] = pFbxMesh->GetTextureUVIndex(iPoly, VertexIndex[1]);
				u[2] = pFbxMesh->GetTextureUVIndex(iPoly, VertexIndex[2]);
				if (VertexUVSet.size() > 0)
				{
					FbxLayerElementUV* pUVSet = VertexUVSet[0];
					FbxVector2 uv;
					//사용하는 텍스쳐 읽기
					ReadTextureCoord(pFbxMesh,pUVSet,CornerIndex[iIndex],u[iIndex],	uv);
					tVertex.t.x = uv.mData[0];
					tVertex.t.y = 1.0f - uv.mData[1];
				}

				FbxColor color = FbxColor(1, 1, 1, 1);
				if (VertexColorSet.size() > 0)
				{
					color = ReadColor(pFbxMesh,
									  VertexColorSet.size(),
									  VertexColorSet[0],
									  CornerIndex[iIndex],
									  iBasePolyIndex + VertexIndex[iIndex]);
				}
				tVertex.c.x = color.mRed;
				tVertex.c.y = color.mGreen;
				tVertex.c.z = color.mBlue;
				tVertex.c.w = pObject->m_iIndex;


				FbxVector4 normal = ReadNormal(pFbxMesh, CornerIndex[iIndex], iBasePolyIndex + VertexIndex[iIndex]);
				normal = normalMatrix.MultT(normal);
				tVertex.n.x = normal.mData[0]; // x
				tVertex.n.y = normal.mData[2]; // z
				tVertex.n.z = normal.mData[1]; // y


				// 가중치
				VertexIW iwVertex;
				if (pObject->m_bSkinned)
				{
					Weight* weight = &pObject->m_WeightList[CornerIndex[iIndex]];
					for (int i = 0; i < 4; i++)
					{
						iwVertex.i[i] = weight->Index[i];
						iwVertex.w[i] = weight->weight[i];
					}
				}
				else
				{
					// 일반오브젝트 에니메이션을 스키닝 케릭터 화 작업.
					iwVertex.i[0] = pObject->m_iIndex;
					iwVertex.w[0] = 1.0f;
				}
				//pObject->m_VertexList.push_back(tVertex);//36
				pObject->m_pSubVertexList[iSubMtrl].push_back(tVertex);
				pObject->m_pSubIWVertexList[iSubMtrl].push_back(iwVertex);
			}
		}

		iBasePolyIndex += iPolySize;
	}
	
}
bool TFbxImporter::CreateConstantBuffer(ID3D11Device* pDevice)
{
	HRESULT hr;
	//gpu메모리에 버퍼 할당(원하는 할당 크기)
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(BoneWorld);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	//D3D11_SUBRESOURCE_DATA sd;
	//ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	//sd.pSysMem = &m_matBoneArray;

	if (FAILED(hr = pDevice->CreateBuffer(&bd, NULL,&m_pBoneCB)))
	{
		return false;
	}
	return true;
}
bool TFbxImporter::Init()
{
	//fbx 초기화 작업
	m_pFbxManager = FbxManager::Create();
	m_pFbxImporter = FbxImporter::Create(m_pFbxManager, "");
	m_pFbxScene = FbxScene::Create(m_pFbxManager, "");	
	return true;
}
bool TFbxImporter::Frame()
{
	return true;
}
bool TFbxImporter::Render()
{
	return true;
}
bool TFbxImporter::Release()
{
	if(m_pBoneCB)m_pBoneCB->Release();
	m_pBoneCB = nullptr;
	for (int iObj = 0; iObj < m_DrawList.size(); iObj++)
	{
		m_DrawList[iObj]->Release();
	}
	if(m_pFbxScene)m_pFbxScene->Destroy();
	if (m_pFbxImporter)m_pFbxImporter->Destroy();
	if (m_pFbxManager)m_pFbxManager->Destroy();
	m_pFbxScene = nullptr;
	m_pFbxImporter = nullptr;
	m_pFbxManager = nullptr;
	return true;
}
bool TFbxImporter::Load(ID3D11Device* pd3dDevice, std::wstring filename)
{
	if (Load(to_wm(filename).c_str()))
	{
		CreateConstantBuffer(pd3dDevice);
		Shader* pVShader = I_Shader.CreateVertexShader(pd3dDevice, L"../../data/shader/Character.hlsl", "VS");
		Shader* pPShader = I_Shader.CreatePixelShader(pd3dDevice, L"../../data/shader/Character.hlsl", "PS");
		for (int iObj = 0; iObj < m_DrawList.size(); iObj++)
		{
			m_DrawList[iObj]->Init();
			m_DrawList[iObj]->m_pVShader = pVShader;
			m_DrawList[iObj]->m_pPShader = pPShader;
			if (!m_DrawList[iObj]->Create(pd3dDevice,	m_pContext))
			{
				return false;
			}
		}
	}
	return true;
}