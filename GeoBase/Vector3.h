#pragma once
#include <cmath>


struct Vector3 {

	double x, y, z;

	Vector3() : x(0),y(0),z(0){}

	Vector3(double x, double y, double z) : x(x),y(y),z(z) {}


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

	//向量长度（模）
	double length() const {
		return std::sqrt(x * x + y * y + z * z);
	}

	//单位化
	Vector3 normalized() {
		double len = length();
		if (len < 1e-9)  return Vector3();
		return *this/len;
	}



};











