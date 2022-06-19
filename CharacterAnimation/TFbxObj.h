#pragma once
#include "TFbxImporter.h"
#include "Input.h"
class Fbx :public Object3D
{
public:
	TFbxImporter* m_pMeshImp;
	TFbxImporter* m_pAnimImporter;
	float m_fDir = 1.0f;
	float m_fTime = 0.0f;
	float m_fSpeed = 1.0f;

	bool  all = true;
	bool  idel = false;
	bool  walk = false;
	bool  run = false;
	bool  jump = false;
	bool  attack = false;

	BoneWorld			m_matBoneArray;
	std::vector<Fbx>	m_DrawList;
public:
	virtual bool	Init()override;
	virtual bool	Frame()override;
	virtual bool	Render()override;
	virtual bool    Release() override;
public:
	//애니메이션 보간
	T::TMatrix Interplate(TFbxImporter* pAnimImp,FbxModel* pModel, float fFrame);
};
