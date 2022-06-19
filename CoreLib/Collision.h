#pragma once
#include "TMath.h"
enum Type { RECT_OUT = 0, RECT_IN = 1, RECT_OVERLAP = 2 };
// È­¸éÁÂÇ¥°è+¿ÞÂÊ»ó´ÜÀÌ ¿øÁ¡
struct Rect2D
{
	T::TVector2 vMin;
	T::TVector2 vMax;
	T::TVector2 vCenter;
	T::TVector2 vSize;
	bool operator == (const Rect2D& v)
	{
		if (fabs((vMin - v.vMin).Length()) < 0.0001f)
		{
			if (fabs((vMax - v.vMax).Length()) < 0.0001f)
			{
				return true;
			}
		}
		return false;
	}
	Rect2D() {};
	Rect2D(T::TVector2 vMin, T::TVector2 vMax)
	{
		this->vMin = vMin;
		this->vMax = vMax;
		vCenter = (vMax + vMin);
		vCenter /= 2.0f;
		vSize.x = vMax.x - vMin.x;
		vSize.y = vMax.y - vMin.y;
	}
	Rect2D(T::TVector2 v, float w, float h)
	{
		this->vMin = v;
		this->vMax = vMin + T::TVector2(w, h);
		vCenter = (vMax + vMin);
		vCenter /= 2.0f;
		this->vSize.x = w;
		this->vSize.y = h;
	}
};
// È­¸éÁÂÇ¥°è+Áß¾ÓÀÌ ¿øÁ¡
struct Rect
{
	T::TVector2 vMin;
	T::TVector2 vMax;
	T::TVector2 vCenter;
	T::TVector2 vSize;
	bool operator == (const Rect& v)
	{
		if (fabs((vMin - v.vMin).Length()) < 0.0001f)
		{
			if (fabs((vMax - v.vMax).Length()) < 0.0001f)
			{
				return true;
			}
		}
		return false;
	}
	Rect() {};
	Rect(T::TVector2 vMin, T::TVector2 vMax)
	{
		this->vMin = vMin;
		this->vMax = vMax;
		vCenter = (vMax + vMin);
		vCenter /= 2.0f;
		vSize.x = vMax.x - vMin.x;
		vSize.y = vMax.y - vMin.y;
	}
	Rect(T::TVector2 pos, float w, float h)
	{
		vCenter = pos;
		this->vMin = vCenter - T::TVector2(w / 2.0f, h / 2.0f);
		this->vMax = vCenter + T::TVector2(w / 2.0f, h / 2.0f);
		this->vSize.x = w;
		this->vSize.y = h;
	}
};

class Box
{
public:
	T::TVector3 vList[8];
	T::TVector3 vMin;
	T::TVector3 vMax;
	T::TVector3 vCenter;
	T::TVector3 vAxis[3];
	T::TVector3 vSize;

	bool operator == (const Box& v)
	{
		if (fabs((vMin - v.vMin).Length()) < 0.0001f)
		{
			if (fabs((vMax - v.vMax).Length()) < 0.0001f)
			{
				return true;
			}
		}
		return false;
	}
	Box() {};
	Box(T::TVector3 Min, T::TVector3 Max)
	{
		vMin = Min;
		vMax = Max;
		vCenter = (vMax + vMin);
		vCenter /= 2.0f;
		vSize.x = Max.x - Min.x;
		vSize.y = Max.y - Min.y;
		vSize.z = Max.z - Min.z;
	};
	Box(T::TVector3 v, float w, float h, float q)
	{
		vMin = v;
		vMax = vMin + T::TVector3(w, h, q);
		vCenter = (vMax + vMin);
		vCenter /= 2.0f;
		vSize.x = w;
		vSize.y = h;
		vSize.z = q;
	}
};
struct Sphere
{
	T::TVector3 vCenter;
	float		fRadius;
	Sphere()
	{
		fRadius = 3.0f;
	}
};
class Collision
{
public:
	static bool SphereToPoint(Sphere rt, float x, float y, float z);
	static bool SphereToPoint(Sphere rt, T::TVector3 v);

	static bool RectToPoint(Rect rt, int x, int y);
	static bool RectToPoint(Rect rt, T::TVector2 v);
	static Type RectToRect(Rect rt1, Rect rt2);

	static Type ToRect(Rect rt1, Rect rt2);

	static Rect UnionRect(Rect rt1, Rect rt2);
	static bool IntersectRect(Rect rt1, Rect rt2, Rect* rt);


	static bool BoxToPoint(Box bx, int x, int y, int z);
	static bool BoxToPoint(Box bx, T::TVector3 v);
	static Type BoxToBox(Box box1, Box box2);
	static Box UnionBox(Box bx1, Box bx2);
	static bool IntersectBox(Box bx1, Box bx2, Box* bx);
};

