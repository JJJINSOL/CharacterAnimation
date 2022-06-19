#pragma once
#include "Core.h"
#include "TFbxObj.h"
class Sample : public Core
{
	std::vector<Fbx> m_FbxObj;
public:
	virtual bool Init()  override;
	virtual bool Frame()  override;
	virtual bool Render()  override;
	virtual bool Release()  override;	

	virtual void CreateResizeDevice(UINT iWidth, UINT iHeight) override;
	virtual void DeleteResizeDevice(UINT iWidth, UINT iHeight) override;

public:
	Sample();
	virtual ~Sample();
};

