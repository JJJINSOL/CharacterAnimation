#include "Matrix.h"
namespace MyMath
{
	Matrix Matrix::RotationYawPitchRoll(float yaw, float pitch, float roll)
	{
		Matrix matY;
		matY.YRotate(yaw);
		Matrix matX;
		matX.XRotate(pitch);
		Matrix matZ;
		matZ.ZRotate(roll);
		Matrix ret = matZ * matX * matY;
		return ret;
	}
}