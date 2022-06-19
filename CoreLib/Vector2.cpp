#include "Vector2.h"
namespace MyMath
{
	Vector2::Vector2()
	{
		x = y = 0.0f;
	}
	Vector2::Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	Vector2::Vector2(const Vector2& v)
	{
		x = v.x;
		y = v.y;
	}

	Vector2 Vector2:: operator + (const Vector2& v)
	{
		Vector2 vec;
		vec.x = this->x + v.x;
		vec.y = this->y + v.y;
		return vec;
	}
	Vector2 Vector2:: operator - (const Vector2& v)
	{
		Vector2 vec;
		vec.x = this->x - v.x;
		vec.y = this->y - v.y;
		return vec;
	}
	Vector2 Vector2:: operator * (float num)
	{
		Vector2 vec;
		vec.x = x * num;
		vec.y = y * num;
		return vec;
	}
	Vector2 Vector2:: operator / (float num)
	{
		Vector2 vec;
		vec.x = x / num;
		vec.y = y / num;
		return vec;
	}
	Vector2 Vector2::operator += (const Vector2& v)
	{
		this->x = this->x + v.x;
		this->y = this->y + v.y;
		return *this;
	}
	Vector2 Vector2::operator -= (const Vector2& v)
	{
		this->x = this->x - v.x;
		this->y = this->y - v.y;
		return *this;
	}
	bool Vector2:: operator == (const Vector2& v)
	{
		if (fabs(x - v.x) < 0.0001f)
		{
			if (fabs(y - v.y) < 0.0001f)
			{
				return true;
			}
		}
		return false;
	}
	bool Vector2:: operator != (const Vector2& v)
	{
		if (fabs(x - v.x) < 0.0001f)
		{
			if (fabs(y - v.y) < 0.0001f)
			{
				return false;
			}
		}
		return true;
	}

	float Vector2::Length()
	{
		float len = sqrt(pow(x, 2) + pow(y, 2));
		return len;
	}

	Vector2 Vector2::Normal()
	{
		Vector2 vec;
		float d = 1.0f / Length();
		vec.x = x * d;
		vec.y = y * d;
		return vec;
	}
	Vector2 Vector2::Normalize()
	{
		float d = 1.0f / Length();
		x = x * d;
		y = y * d;
		return *this;
	}
	Vector2 Normalize(Vector2& v)
	{
		Vector2 vec;
		float d = 1.0f / v.Length();
		vec.x = v.x * d;
		vec.y = v.y * d;
		return vec;
	}
	// 두벡터의 거리 반환
	float Vector2::Distance(Vector2& v)
	{
		float fDist = (*this - v).Length();
		return fDist;
	}
}