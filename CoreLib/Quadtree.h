#pragma once
#include "Node.h"
#include "Map.h"
#include "Camera.h"
class Quadtree
{
public:
	Map*	m_pMap=nullptr;
	Camera* m_pCamera = nullptr;
	int		m_iWidth;
	int		m_iHeight;
	Node*  m_pRootNode;
	int		m_iLeafDepth=0;
	int		m_iMaxDepth = 0;
	int		m_iNumLOD = 1;
	int		m_iLeafLOD = 1;
	static  int g_iCount;
public:
	std::list<MapObject*> m_ObjectList;
	std::vector<Node*> g_pDrawLeafNodes;
	std::vector<Node*> g_pLeafNodes;
	std::queue<Node*> g_Queue;	
	std::vector<DWORD>   m_IndexList;
	ComPtr<ID3D11Buffer> m_pIndexBuffer = nullptr;
	int					 m_iNumFace;
	int g_iValue = 0;
public:
	void		Build(int iWidth, int iHeight, int iMaxDepth);
	Node*		CreateNode(Node* pParent, float x, float y, float w, float h);
public:
	void		Build(Map* pMap, int iMaxDepth);
	void		BuildTree(Node* pParent);
	Box			GenBoundingBox(Node* pNode);
	TVector2	GetHeightFromNode(DWORD dwTL, DWORD dwTR, DWORD dwBL, DWORD dwBR);
	bool		AddObject(MapObject* obj);
	bool		AddDynamicObject(MapObject* obj);

	void		DynamicDeleteObject(Node* pNode);
	Node*		FindNode(Node* pNode, Box& box);
	bool		CheckBox(Box& a, Box& b);
public:
	void		SetIndexData(Node* pNode, int iLodLevel);
	bool		CreateIndexBuffer(Node* pNode, int iLodLevel);
	void		Update(Camera* pCamera);
	bool		Render();
	void		RenderObject(Node* pNode);
	void		RenderTile(Node* pNode);
public:
	void		FindNeighborNode();
	Node*		CheckBoxtoPoint(T::TVector3 p);
	void		GetRatio(Node* pNode);
	int			GetLodType(Node* pNode);
	int			UpdateIndexList(Node* pNode, DWORD dwCurentIndex, DWORD dwNumLevel);
	int			SetLodIndexBuffer(Node* pNode,DWORD& dwCurentIndex,
				DWORD dwA, DWORD dwB, DWORD dwC,DWORD dwType);
#ifdef _DEBUG
public:
	BoxObj		m_BoxDebug;
	void		DrawDebugInit(ID3D11Device* pd3dDevice,
							  ID3D11DeviceContext* pContext);
	void		DrawDebugRender(Box* pBox);
#endif
public:
	void PrintObjectList(Node* pNode);


public:
	Quadtree() {};
	virtual ~Quadtree()
	{
#ifdef _DEBUG
		m_BoxDebug.Release();
#endif
		delete m_pRootNode;
	}
};
