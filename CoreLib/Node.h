#pragma once
#include <iostream>
#include <queue>
#include <stack>
#include <vector> // 배열(재할당 가능한)
#include <list>
#include "Object3D.h"
using namespace std;

struct MapObject
{
	std::wstring  name;
	T::TMatrix matWorld;
	T::TVector3 vPos;
	Box       box;
	Object3D* pObject;
	T::TVector3 m_vLight;
	T::TVector3 m_vUp;
	T::TVector3 m_vLook;
	void		UpdateData()
	{
		m_vLight.x = matWorld._11;
		m_vLight.y = matWorld._12;
		m_vLight.z = matWorld._13;
		m_vUp.x = matWorld._21;
		m_vUp.y = matWorld._22;
		m_vUp.z = matWorld._23;
		m_vLook.x = matWorld._31;
		m_vLook.y = matWorld._32;
		m_vLook.z = matWorld._33;

		T::D3DXVec3Normalize(&m_vLight, &m_vLight);
		T::D3DXVec3Normalize(&m_vUp, &m_vUp);
		T::D3DXVec3Normalize(&m_vLook, &m_vLook);
	}
	void UpdateCollision()
	{
		box.vAxis[0] = m_vLight;
		box.vAxis[1] = m_vUp;
		box.vAxis[2] = m_vLook;

		// GenAABB();
		box.vMin = T::TVector3(100000, 100000, 100000);
		box.vMax = T::TVector3(-100000, -100000, -100000);
		for (int iV = 0; iV < 8; iV++)
		{
			T::TVector3 pos;
			T::D3DXVec3TransformCoord(&pos, &box.vList[iV], &matWorld);
			if (box.vMin.x > pos.x)
			{
				box.vMin.x = pos.x;
			}
			if (box.vMin.y > pos.y)
			{
				box.vMin.y = pos.y;
			}
			if (box.vMin.z > pos.z)
			{
				box.vMin.z = pos.z;
			}

			if (box.vMax.x < pos.x)
			{
				box.vMax.x = pos.x;
			}
			if (box.vMax.y < pos.y)
			{
				box.vMax.y = pos.y;
			}
			if (box.vMax.z < pos.z)
			{
				box.vMax.z = pos.z;
			}
		}

		T:TVector3 vHalf = box.vMax - box.vCenter;
		box.vSize.x = fabs(T::D3DXVec3Dot(&box.vAxis[0], &vHalf));
		box.vSize.y = fabs(T::D3DXVec3Dot(&box.vAxis[1], &vHalf));
		box.vSize.z = fabs(T::D3DXVec3Dot(&box.vAxis[2], &vHalf));
		box.vCenter = (box.vMin + box.vMax);
		box.vCenter /= 2.0f;
	}
};

class Node
{
public:
	int		m_iIndex;
	Node*	m_pParent;
	Box	m_Box;
	int		m_iDepth;
	int     m_iCurrentLod=0;
	int		m_dwLodType; //0 ~ 16
	bool	m_bLeaf = false;
	std::vector<Node*> m_pChild;
	std::vector<Node*> m_pNeighborList; // 북(0)남(1)서(2)동(3)
	std::vector<int>   m_CornerList; // 정점인덱스
	std::list<MapObject*> m_StaticObjectList;
	std::list<MapObject*> m_DynamicObjectList;
	void   AddObject(MapObject* obj);
	void   AddDynamicObject(MapObject* obj);
	void   DelDynamicObject(MapObject* obj);
	
public:
	using INDEXLIST = std::vector<DWORD>;// m_IndexList;
	using INDEXBUFFER = ComPtr<ID3D11Buffer>;// m_pIndexBuffer0;
	std::vector<INDEXLIST> m_IndexList;
	std::vector<INDEXBUFFER> m_pIndexBuffer;	
public:
	Node();
	Node(float x, float y, float w, float h);
	~Node();
};
