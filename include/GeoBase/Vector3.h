#pragma once
#include <cmath>
#include "Common.h"
#include <ostream>

struct Vector3 {

	double x, y, z;

	Vector3() : x(0), y(0), z(0) {}

	Vector3(double x, double y, double z) : x(x), y(y), z(z) {}


	//向量加法
	Vector3 operator+(const Vector3& other) const {
		return Vector3(x + other.x, y + other.y, z + other.z);
	}
	

	//向量减法
	Vector3 operator-(const Vector3& other) const {
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	//标量乘法
	Vector3 operator*(double s) const {
		return Vector3(x * s, y * s, z * s);
	}

	//标量除法
	Vector3 operator/(double s) const {
		return Vector3(x / s, y / s, z / s);
	}

	//向量点积
	double dot(const Vector3& other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	//向量叉积
	Vector3 cross(const Vector3& other) const {
		return Vector3(
			y * other.z - other.y * z,
			other.x * z - x * other.z,
			x * other.y - other.x * y
		);
	}

	// 向量长度的平方，避免开方运算
	double lengthSquared() const {
		return x * x + y * y + z * z;
	}

	//向量长度（模）
	double length() const {
		return std::sqrt(lengthSquared());
	}

	//// 点在向量上的投影
	//Point3 projectOnto(const Vector3& other) const {
	//	double otherLenSq = other.dot(other);
	//	if (otherLenSq < EPS_ABS) return Vector3(); // 避免除以0
	//	double t = this->dot(other) / otherLenSq;
	//	return other * t;
	//}

	//归一化
	Vector3 normalized() const {
		double len = length();
		if (len < EPS_ABS)  return Vector3();
		return *this / len;
	}



};

// 正确：全局重载 <<
inline std::ostream& operator<<(std::ostream& os, const Vector3& v) {
	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}

using Point3 = Vector3;







