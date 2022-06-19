#include "Collision.h"
bool Collision::SphereToPoint(Sphere sp, float x, float y, float z)
{
	float fDistance = (sp.vCenter - T::TVector3(x, y, z)).Length();
	if (fDistance <= sp.fRadius)
	{
		return true;
	}
	return false;
}
bool Collision::SphereToPoint(Sphere sp, T::TVector3 v)
{
	float fDistance = (sp.vCenter - v).Length();
	if (fDistance <= sp.fRadius)
	{
		return true;
	}
	return false;
}
bool Collision::RectToPoint(Rect rt, int x, int y)
{
	if (rt.vMin.x <= x && rt.vMax.x >= x &&
		rt.vMin.y <= y && rt.vMax.y >= y )
	{
		return true;
	}
	return false;
}
bool Collision::RectToPoint(Rect rt, T::TVector2 v)
{
	if (rt.vMin.x <= v.x && rt.vMax.x >= v.x &&
		rt.vMin.y <= v.y && rt.vMax.y >= v.y )
	{
		return true;
	}
	return false;
}

Rect Collision::UnionRect(Rect rt1, Rect rt2)
{
	Rect unirect;
	unirect.vMin.x = rt1.vMin.x < rt2.vMin.x ? rt1.vMin.x : rt2.vMin.x;
	unirect.vMin.y = rt1.vMin.y < rt2.vMin.y ? rt1.vMin.y : rt2.vMin.y;

	unirect.vMax.x = rt1.vMax.x < rt2.vMax.x ? rt2.vMax.x : rt1.vMax.x;
	unirect.vMax.y = rt1.vMax.y < rt2.vMax.y ? rt2.vMax.y : rt1.vMax.y;

	unirect.vSize = unirect.vMax - unirect.vMin;
	unirect.vCenter = (unirect.vMin + unirect.vMax);
	unirect.vCenter /= 2.0f;
	return unirect;
}
bool Collision::IntersectRect(Rect rt1, Rect rt2, Rect* rt)
{
	Rect unirect = UnionRect(rt1, rt2);
	if (unirect.vSize.x <= (rt1.vSize.x + rt2.vSize.x) &&
		unirect.vSize.y <= (rt1.vSize.y + rt2.vSize.y) )
	{
		if (rt != nullptr)
		{
			rt->vMin.x = rt1.vMin.x < rt2.vMin.x ? rt2.vMin.x : rt1.vMin.x;
			rt->vMin.y = rt1.vMin.y < rt2.vMin.y ? rt2.vMin.y : rt1.vMin.y;
			
			rt->vMax.x = rt1.vMax.x < rt2.vMax.x ? rt1.vMax.x : rt2.vMax.x;
			rt->vMax.y = rt1.vMax.y < rt2.vMax.y ? rt1.vMax.y : rt2.vMax.y;

			rt->vSize = rt->vMax - rt->vMin;
			rt->vCenter = (rt->vMax + rt->vMin);
			rt->vCenter /= 2.0f;
		}
		return true;
	}
	return false;
}

Type Collision::RectToRect(Rect rt1, Rect rt2)
{
	Rect rt;
	int ret = IntersectRect(rt1, rt2, &rt);
	if (ret <= 0)
	{
		return RECT_OUT;
	}
	if (rt == rt2) return RECT_OVERLAP;
	return RECT_IN;
}
Type Collision::ToRect(Rect rt1, Rect rt2)
{
	// 거리 판정
	float fDistanceX;
	float fDistanceY;
	fDistanceX = fabs(rt1.vCenter.x - rt2.vCenter.x);
	fDistanceY = fabs(rt1.vCenter.y - rt2.vCenter.y);
	float fToX = rt1.vSize.x / 2.0f + rt2.vSize.x / 2.0f;
	float fToY = rt1.vSize.y / 2.0f + rt2.vSize.y / 2.0f;
	if (fDistanceX < fToX && fDistanceY < fToY)
	{
		return RECT_OVERLAP;
	}
	return RECT_OUT;
}
//==========================================================================================================
bool Collision::BoxToPoint(Box rt, int x, int y, int z)
{
	if (rt.vMin.x <= x && rt.vMax.x >= x &&
		rt.vMin.y <= y && rt.vMax.y >= y &&
		rt.vMin.z <= z && rt.vMax.z >= z)
	{
		return true;
	}
	return false;
}
bool Collision::BoxToPoint(Box rt, T::TVector3 v)
{
	if (rt.vMin.x <= v.x && rt.vMax.x >= v.x &&
		rt.vMin.y <= v.y && rt.vMax.y >= v.y &&
		rt.vMin.z <= v.z && rt.vMax.z >= v.z)
	{
		return true;
	}
	return false;
}

Box Collision::UnionBox(Box rt1, Box rt2)
{
	Box unibox;
	unibox.vMin.x = rt1.vMin.x < rt2.vMin.x ? rt1.vMin.x : rt2.vMin.x;
	unibox.vMin.y = rt1.vMin.y < rt2.vMin.y ? rt1.vMin.y : rt2.vMin.y;
	unibox.vMin.z = rt1.vMin.z < rt2.vMin.z ? rt1.vMin.z : rt2.vMin.z;

	unibox.vMax.x = rt1.vMax.x < rt2.vMax.x ? rt2.vMax.x : rt1.vMax.x;
	unibox.vMax.y = rt1.vMax.y < rt2.vMax.y ? rt2.vMax.y : rt1.vMax.y;
	unibox.vMax.z = rt1.vMax.z < rt2.vMax.z ? rt2.vMax.z : rt1.vMax.z;

	unibox.vSize = unibox.vMax - unibox.vMin;
	unibox.vCenter = (unibox.vMin + unibox.vMax);
	unibox.vCenter /= 2.0f;
	return unibox;
}
bool Collision::IntersectBox(Box rt1, Box rt2, Box* rt)
{
	Box unibox = UnionBox(rt1, rt2);
	if (unibox.vSize.x <= (rt1.vSize.x + rt2.vSize.x) &&
		unibox.vSize.y <= (rt1.vSize.y + rt2.vSize.y) &&
		unibox.vSize.z <= (rt1.vSize.z + rt2.vSize.z))
	{
		if (rt != nullptr)
		{
			rt->vMin.x = rt1.vMin.x < rt2.vMin.x ? rt2.vMin.x : rt1.vMin.x;
			rt->vMin.y = rt1.vMin.y < rt2.vMin.y ? rt2.vMin.y : rt1.vMin.y;
			rt->vMin.z = rt1.vMin.z < rt2.vMin.z ? rt2.vMin.z : rt1.vMin.z;

			rt->vMax.x = rt1.vMax.x < rt2.vMax.x ? rt1.vMax.x : rt2.vMax.x;
			rt->vMax.y = rt1.vMax.y < rt2.vMax.y ? rt1.vMax.y : rt2.vMax.y;
			rt->vMax.z = rt1.vMax.z < rt2.vMax.z ? rt1.vMax.z : rt2.vMax.z;

			rt->vSize = rt->vMax - rt->vMin;
			rt->vCenter = (rt->vMax + rt->vMin);
			rt->vCenter /= 2.0f;
		}
		return true;
	}
	return false;
}

Type Collision::BoxToBox(Box box1, Box box2)
{
	Box bx;
	int ret = IntersectBox(box1, box2, &bx);
	if (ret <= 0)
	{
		return RECT_OUT;
	}
	if (bx == box2) return RECT_OVERLAP;
	return RECT_IN;
}