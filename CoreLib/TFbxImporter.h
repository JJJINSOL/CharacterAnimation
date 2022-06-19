#pragma once
#include "Object3D.h"
#include <fbxsdk.h>

struct PNCT
{
	T::TVector3 p; //정점 위치
	T::TVector3 n; //정점 노말
	T::TVector4 c; //정점 컬러
	T::TVector2 t; //정점 텍스쳐 좌표
};
struct VertexIW
{
	float  i[4];
	float  w[4];
	VertexIW()
	{
		i[0] = i[1] = i[2] = i[3] = 0;
		w[0] = w[1] = w[2] = w[3] = 0.0f;
	}
};
// 멀티스트림  (VB[0], VB[1])
// 인풋레이아웃 ( PNCTIW ) -> 정점쉐이더 전달

struct Track
{
	UINT	iFrame;
	TMatrix matTrack;
	// SRT
	T::TVector3	   s; //크기
	T::TQuaternion r; //회전
	T::TVector3    t; //이동
};
struct Weight
{
	std::vector<int>	Index;  // 영향을 미치는 행렬의 인덱스
	std::vector<float>  weight; // 가중치
	void InsertWeight(int iBoneIndex, float fBoneWeight)
	{
		for (DWORD i = 0; i < Index.size(); ++i)
		{
			if (fBoneWeight > weight[i])
			{
				for (DWORD j = (Index.size() - 1); j > i; --j)
				{
					Index[j] = Index[j - 1];
					weight[j] = weight[j - 1];
				}
				Index[i] = iBoneIndex;
				weight[i] = fBoneWeight;
				break;
			}
		}
	}
	Weight()
	{
		Index.resize(8);
		weight.resize(8);
	}
};
struct Scene
{
	UINT   iStart;		 //애니메이션 시작 프레임
	UINT   iEnd;         //애니메이션 마지막 프레임
	UINT   iFrameSpeed;  //애니메이션 프레임 속도
};
//=================================================================================
class FbxModel : public Object3D
{
public:	
	int			 m_iIndex = -1;
	bool		 m_bSkinned = false;
	TMatrix		 m_matLocal;
	TMatrix		 m_matAnim;
	FbxNode*	 m_pFbxParent = nullptr;  //트리 형태로 저장되어 있다 fbx는
	FbxNode*	 m_pFbxNode = nullptr;
	FbxModel*	 m_pParentObj = nullptr;
	std::wstring m_szTexFileName;
	// submaterial
	std::vector<std::wstring>  m_szTexFileList;

	using SubVertex = std::vector<Vertex>;
	using SubVertexIW = std::vector<VertexIW>;

	std::vector<SubVertex>      m_pSubVertexList;
	std::vector<SubVertexIW>    m_pSubIWVertexList;

	std::vector<Weight>			m_WeightList;  //가중치 리스트

	std::vector<ID3D11Buffer*>   m_pVBList;
	std::vector<ID3D11Buffer*>   m_pVBWeightList;
	std::vector<Texture*>		 m_pTextureList;

	std::vector<Track>					m_AnimTrack;
	std::map<std::wstring, TMatrix>		m_dxMatrixBindPoseMap;
	//TMatrix aaa[255];
public:
	virtual bool    SetVertexData() override;	
	virtual bool	CreateVertexBuffer()override;
	virtual bool    SetIndexData() override;
	virtual bool	CreateInputLayout() override;
	virtual bool	PostRender() override;
	virtual bool    Release() override;
	
};

//FbxImporter 로 클래스 설정 안됨--------------------------------------------------
class TFbxImporter : public Object3D
{
public:
	Scene		m_Scene;
	float		m_fDir = 1.0f;
	float		m_fTime = 0.0f;
	float		m_fSpeed = 1.0f;
	BoneWorld	m_matBoneArray;
public:
	
	FbxManager*		m_pFbxManager;
	FbxImporter*	m_pFbxImporter;
	FbxScene*		m_pFbxScene;

	FbxNode*		m_pRootNode;

	std::map<FbxNode*, int>				m_pFbxNodeMap;
	std::map<std::wstring, FbxModel*>	m_pFbxModelMap;

	std::vector<FbxModel*>  m_DrawList;
	std::vector<FbxModel*>  m_TreeList;
	ID3D11Buffer* m_pBoneCB = nullptr;
public:
	bool			Load(ID3D11Device* pd3dDevice, std::wstring filename);
	virtual bool	Load(std::string filename);
	virtual void    PreProcess(FbxNode* node, FbxModel* fbxParent=nullptr);
	virtual void	ParseMesh(FbxModel* pObject);
	std::string		ParseMaterial(FbxSurfaceMaterial* pMtrl);

	void ReadTextureCoord(	FbxMesh* pFbxMesh, 
							FbxLayerElementUV* pUVSet, 
							int vertexIndex, 
							int uvIndex, 
							FbxVector2& uv);
	
	FbxColor ReadColor(const FbxMesh* mesh,
						DWORD dwVertexColorCount,
						FbxLayerElementVertexColor* pVertexColorSet,
						DWORD dwDCCIndex, DWORD dwVertexIndex);
	FbxVector4 ReadNormal(const FbxMesh* mesh,int controlPointIndex,int vertexCounter);

	int		GetSubMaterialIndex(int iPlygon,FbxLayerElementMaterial* pMtrl);
	bool	ParseMeshSkinning(FbxMesh* pFbxMesh, FbxModel* pObject);

public:
	TMatrix     DxConvertMatrix(TMatrix m);
	TMatrix     ConvertMatrix(FbxMatrix& m);
	TMatrix     ConvertAMatrix(FbxAMatrix& m);
	void		ParseAnimation();
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();
	virtual bool	CreateConstantBuffer(ID3D11Device* pDevice);
};

