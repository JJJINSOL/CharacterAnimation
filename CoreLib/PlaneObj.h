#pragma once
#include "Object3D.h"
class PlaneObj :   public Object3D
{
public:
	virtual bool		SetVertexData();
	virtual bool		SetIndexData();
public:
	PlaneObj();
	virtual ~PlaneObj();
};

