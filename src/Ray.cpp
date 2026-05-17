#include "../include/GeoBase/Ray.h"
#include <cassert>

// 构造函数
Ray::Ray(const Point3& origin, const Vector3& direction) : origin(origin), direction(direction.normalized()) {
	// 断言方向向量不能是零向量
	assert(direction.length() > 1e-9 && "Ray direction vector cannot be zero.");
}

// 计算射线在参数t处的点
Point3 Ray::pointAt(double t) const {
	return origin + direction * t;
}

// 射线与平面求交：判断射线是否与平面相交
//bool Ray::intersect(const Plane& plane) const {
//	double  denom = direction.dot(plane.normal);
//	if (std::abs(denom) < EPS_ABS) {
//		return false; // 射线与平面平行或者在平面上，无有效交点
//	}
//
//	double t = (plane.origin - origin).dot(plane.normal) / denom;
//	return t - EPS_ABS >= 0; // 只考虑射线前方的交点
//
//}

