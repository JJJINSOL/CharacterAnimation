#pragma once
#include "Vector3.h"
namespace MyMath
{
	struct float4x4
	{
		union {
			struct {
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float m[4][4];
		};
	};
	class Matrix : public float4x4
	{
	public:
		Matrix operator * (Matrix const& matrix)
		{
			Matrix OutMatrix;
			for (int iColumn = 0; iColumn < 4; iColumn++)
			{
				for (int iRow = 0; iRow < 4; iRow++)
				{
					OutMatrix.m[iRow][iColumn] = m[iRow][0] * matrix.m[0][iColumn] +
						m[iRow][1] * matrix.m[1][iColumn] +
						m[iRow][2] * matrix.m[2][iColumn] +
						m[iRow][3] * matrix.m[3][iColumn];
				}
			}
			return OutMatrix;
		}
		void    XRotate(float fRadian)
		{
			float fCos = cos(fRadian);
			float fSin = sin(fRadian);
			_22 = fCos;
			_23 = fSin;
			_32 = -fSin;
			_33 = fCos;
		}
		void    YRotate(float fRadian)
		{
			float fCos = cos(fRadian);
			float fSin = sin(fRadian);
			_11 = fCos;
			_13 = -fSin;
			_31 = fSin;
			_33 = fCos;
		}
		void    ZRotate(float fRadian)
		{
			float fCos = cos(fRadian);
			float fSin = sin(fRadian);
			_11 = fCos;
			_12 = fSin;
			_21 = -fSin;
			_22 = fCos;
		}
		void Translation(const Vector3& v)
		{
			_41 = v.x;
			_42 = v.y;
			_43 = v.z;
		}
		void Translation(float x, float y, float z)
		{
			_41 = x;
			_42 = y;
			_43 = z;
		}
		void Scale(const Vector3& vector)
		{
			_11 = vector.x;
			_22 = vector.y;
			_33 = vector.z;
		}
		void Scale(float x, float y, float z)
		{
			_11 = x;
			_22 = y;
			_33 = z;
		}
		//단위
		void   Identity()
		{
			_11 = _12 = _13 = _14 = 0.0f;
			_21 = _22 = _23 = _24 = 0.0f;
			_31 = _32 = _33 = _34 = 0.0f;
			_41 = _42 = _43 = _44 = 0.0f;
			_11 = _22 = _33 = _44 = 1.0f;
		}
		//전치
		Matrix Transpose()
		{
			Matrix matrix;
			matrix._11 = _11; matrix._12 = _21; matrix._13 = _31; matrix._14 = _41;
			matrix._21 = _12; matrix._22 = _22; matrix._23 = _32; matrix._24 = _42;
			matrix._31 = _13; matrix._32 = _23; matrix._33 = _33; matrix._34 = _43;
			matrix._41 = _14; matrix._42 = _24; matrix._43 = _34; matrix._44 = _44;
			return matrix;
		}
		// 외적을 통한 뷰 행렬 계산
		Matrix ViewLookAt(Vector3& vPosition, Vector3& vTarget, Vector3& vUp)
		{
			Matrix matrix;
			Vector3 vDirection = (vTarget - vPosition).Normal();		// Z Axis
			Vector3 vRightVector = (vUp ^ vDirection).Normal();			// X Axis
			Vector3 vUpVector = (vDirection ^ vRightVector).Normal();	// Y Axis

			_11 = vRightVector.x;	_12 = vUpVector.x;	_13 = vDirection.x;
			_21 = vRightVector.y;	_22 = vUpVector.y;	_23 = vDirection.y;
			_31 = vRightVector.z;	_32 = vUpVector.z;	_33 = vDirection.z;

			_41 = -(vPosition.x * _11 + vPosition.y * _21 + vPosition.z * _31);
			_42 = -(vPosition.x * _12 + vPosition.y * _22 + vPosition.z * _32);
			_43 = -(vPosition.x * _13 + vPosition.y * _23 + vPosition.z * _33);
			memcpy((void*)&matrix, this, 16 * sizeof(float));
			return matrix;
		}
		// 내적을 통한 뷰 행렬 계산
		Matrix CreateViewLook(Vector3& vPosition, Vector3& vTarget, Vector3& vUp)
		{
			Matrix matrix;
			Vector3 vDirection = vTarget - vPosition;
			vDirection = vDirection.Normal();//z
			float fDot = vUp | vDirection;
			// 직교의 근사화 작업
			Vector3 vUpVector = vUp - (vDirection * fDot);
			vUpVector = vUpVector.Normal();
			Vector3 vRightVector = vUpVector ^ vDirection;

			_11 = vRightVector.x;	_12 = vUpVector.x;	_13 = vDirection.x;
			_21 = vRightVector.y;	_22 = vUpVector.y;	_23 = vDirection.y;
			_31 = vRightVector.z;	_32 = vUpVector.z;	_33 = vDirection.z;
			_41 = -(vPosition.x * _11 + vPosition.y * _21 + vPosition.z * _31);
			_42 = -(vPosition.x * _12 + vPosition.y * _22 + vPosition.z * _32);
			_43 = -(vPosition.x * _13 + vPosition.y * _23 + vPosition.z * _33);
			memcpy((void*)&matrix, this, 16 * sizeof(float));
			return matrix;
		}
		// 원근 투영 행렬 계산
		Matrix PerspectiveFovLH(float fNearPlane, float fFarPlane, float fovy, float Aspect) // width / heght
		{
			float    h, w, Q;

			h = 1 / tan(fovy * 0.5f);  // 1/tans(x) = cot(x)
			w = h / Aspect;

			Q = fFarPlane / (fFarPlane - fNearPlane);

			Matrix ret;
			memset(this, 0, sizeof(Matrix));

			_11 = w;
			_22 = h;
			_33 = Q;
			_43 = -Q * fNearPlane;
			_34 = 1;

			memcpy((void*)&ret, this, 16 * sizeof(float));
			return ret;
		}
		static Matrix RotationYawPitchRoll(float yaw, float pitch, float roll);
	public:
		Matrix()
		{
			Identity();
		};
	};
}
