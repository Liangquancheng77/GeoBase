#include "Matrix4.h"
#include <cstring>
#include <cmath>


Matrix4::Matrix4() {
	setIdentity();
}

void Matrix4::setIdentity() {
	memset(m, 0, sizeof(m));

	m[0][0] = 1.0;
	m[1][1] = 1.0;
	m[2][2] = 1.0;
	m[3][3] = 1.0;
}

Matrix4 Matrix4::createTranslation(double tx, double ty, double tz) {
	Matrix4 mat;
	mat.m[0][3] = tx;
	mat.m[1][3] = ty;
	mat.m[2][3] = tz;
	return mat;
}

Matrix4 Matrix4::createRotationX(double rad) {
	Matrix4 mat;
	mat.m[1][1] = cos(rad);
	mat.m[1][2] = -sin(rad);
	mat.m[2][1] = sin(rad);
	mat.m[2][2] = cos(rad);
	return mat;
}

Matrix4 Matrix4::createRotationY(double rad) {
	Matrix4 mat;
	mat.m[0][0] = cos(rad);
	mat.m[0][2] = sin(rad);
	mat.m[2][0] = -sin(rad);
	mat.m[2][2] = cos(rad);
	return mat;
}

Matrix4 Matrix4::createRotationZ(double rad) {
	Matrix4 mat;
	mat.m[0][0] = cos(rad);
	mat.m[0][1] = -sin(rad);
	mat.m[1][0] = sin(rad);
	mat.m[1][1] = cos(rad);
	return mat;
}

Matrix4 Matrix4::createScaling(double sx, double sy, double sz) {
	Matrix4 mat;
	mat.m[0][0] = sx;
	mat.m[1][1] = sy;
	mat.m[2][2] = sz;
	return mat;
}

void Matrix4::setTranslation(double tx, double ty, double tz) {
	setIdentity();
	m[0][3] = tx;
	m[1][3] = ty;
	m[2][3] = tz;
}

void Matrix4::setRotationX(double rad) {
	setIdentity();
	m[1][1] = cos(rad);
	m[1][2] = -sin(rad);
	m[2][1] = sin(rad);
	m[2][2] = cos(rad);
}

void Matrix4::setRotationY(double rad) {
	setIdentity();
	m[0][0] = cos(rad);
	m[0][2] = sin(rad);
	m[2][0] = -sin(rad);
	m[2][2] = cos(rad);
}

void Matrix4::setRotationZ(double rad) {
	setIdentity();
	m[0][0] = cos(rad);
	m[0][1] = -sin(rad);
	m[1][0] = sin(rad);
	m[1][1] = cos(rad);
}

void Matrix4::setScaling(double sx, double sy, double sz) {
	setIdentity();
	m[0][0] = sx;
	m[1][1] = sy;
	m[2][2] = sz;
}

Matrix4 Matrix4::multiply(const Matrix4& other) const {
	Matrix4 mat;
	for (size_t i = 0; i < sizeof(mat.m) / sizeof(mat.m[0]); i++)
	{
		for (size_t j = 0; j < sizeof(mat.m[0]) / sizeof(double); j++)
		{
			mat.m[i][j] = m[i][0] * other.m[0][j] + m[i][1] * other.m[1][j] + m[i][2] * other.m[2][j] + m[i][3] * other.m[3][j];
		}
	}
	return mat;
}

Vector3 Matrix4::transformPoint(const Vector3& vec) const {
	Vector3 newVec;
	newVec.x = m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z + m[0][3];
	newVec.y = m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z + m[1][3];
	newVec.z = m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z + m[2][3];
	return newVec;
}

Vector3 Matrix4::transformVector(const Vector3& vec) const {
	Vector3 newVec;
	newVec.x = m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z;
	newVec.y = m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z;
	newVec.z = m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z;
	return newVec;
}

Matrix4 Matrix4::transpose() const
{
	Matrix4 mat;

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			mat.m[i][j] = m[j][i];
		}
	}

	return mat;
}

bool Matrix4::isEqualApprox(const Matrix4& other, double eps) const {
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			if (fabs(m[i][j] - other.m[i][j]) >= eps) {
				return false;
			}
		}
	}
	return true;
}

bool Matrix4::operator==(const Matrix4& other) const {
	return isEqualApprox(other);
}




