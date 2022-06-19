#pragma once
#include "Vector2.h"
namespace MyMath
{
	struct float3
	{
		union
		{
			struct { float x, y, z; };
			float v[3];
		};
	};
	class Vector3 : public float3
	{
	public:
		Vector3();
		Vector3(float x, float y, float z);
		Vector3(const Vector3& v);

		void  operator += (const Vector3& v);
		void  operator -= (const Vector3& v);
		float operator | (Vector3 const& v);
		Vector3 operator ^ (Vector3 const& v);

		Vector3 operator + (const Vector3& v);
		Vector3 operator - (const Vector3& v);
		Vector3 operator * (float num);
		Vector3 operator / (float num);
		bool operator == (const Vector3& v);
		bool operator != (const Vector3& v);

		float Length();

		Vector3 Normal();
		Vector3 Normalize();
		friend Vector3 Normalize(Vector3& v);
	};
}