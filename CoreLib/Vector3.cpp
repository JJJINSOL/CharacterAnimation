#include "Vector3.h"
namespace MyMath
{
	Vector3::Vector3()
	{
		x = y = z = 0.0f;
	}
	Vector3::Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Vector3::Vector3(const Vector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	Vector3 Vector3:: operator + (const Vector3& v)
	{
		Vector3 vec;
		vec.x = this->x + v.x;
		vec.y = this->y + v.y;
		vec.z = this->z + v.z;
		return vec;
	}
	Vector3 Vector3:: operator - (const Vector3& v)
	{
		Vector3 vec;
		vec.x = this->x - v.x;
		vec.y = this->y - v.y;
		vec.z = this->z - v.z;
		return vec;
	}
	Vector3 Vector3:: operator * (float num)
	{
		Vector3 ret;
		ret.x = x * num;
		ret.y = y * num;
		ret.z = z * num;
		return ret;
	}
	Vector3 Vector3:: operator / (float num)
	{
		Vector3 vec;
		vec.x = this->x / num;
		vec.y = this->y / num;
		vec.z = this->z / num;
		return vec;
	}
	bool Vector3:: operator == (const Vector3& v)
	{
		if (fabs(x - v.x) < 0.0001f)
		{
			if (fabs(y - v.y) < 0.0001f)
			{
				if (fabs(z - v.z) < 0.0001f)
				{
					return true;
				}
			}
		}
		return false;
	}
	bool Vector3:: operator != (const Vector3& v)
	{
		if (fabs(x - v.x) < 0.0001f)
		{
			if (fabs(y - v.y) < 0.0001f)
			{
				if (fabs(z - v.z) < 0.0001f)
				{
					return false;
				}
			}
		}
		return true;
	}

	float Vector3::Length()
	{
		float len = x * x + y * y + z * z; // = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
		return sqrt(len);
	}

	Vector3 Vector3::Normal()
	{
		Vector3 vec;
		float d = 1.0f / Length();
		vec.x = x * d;
		vec.y = y * d;
		vec.z = z * d;
		return vec;
	}
	Vector3 Vector3::Normalize()
	{
		float d = 1.0f / Length();
		x = x * d;
		y = y * d;
		z = z * d;
		return *this;
	}
	Vector3 Normalize(Vector3& v)
	{
		Vector3 vec;
		float d = 1.0f / v.Length();
		vec.x = v.x * d;
		vec.y = v.y * d;
		vec.z = v.z * d;
		return vec;
	}
	//====================================================
	void Vector3::operator += (const Vector3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
	}
	void Vector3::operator -= (const Vector3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}
	float Vector3::operator | (Vector3 const& v)
	{
		return (x * v.x) + (y * v.y) + (z * v.z);
	}
	Vector3 Vector3::operator ^ (Vector3 const& v)
	{
		return Vector3((y * v.z - z * v.y),
			(z * v.x - x * v.z),
			(x * v.y - y * v.x));
	}
}