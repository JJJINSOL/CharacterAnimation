#pragma once
#include "Object3D.h"
class BoxObj : public Object3D
{
public:
	Texture* m_pTexCube;
	virtual bool SetVertexData();
	virtual bool SetIndexData();
	bool	PostRender();
public:
	BoxObj();
	virtual ~BoxObj();
};

