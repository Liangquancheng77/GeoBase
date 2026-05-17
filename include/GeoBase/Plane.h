#pragma once
#include "Vector3.h"


enum class PlaneSide
{
    ON_PLANE,
    FRONT,
    BACK
};


struct Plane
{
    // 平面单位法向量
    Vector3 normal;
    // 平面上任意一点
    Point3 origin;

	// 构造函数
    Plane(const Vector3& normal, const Point3& origin);

	// 三点确定一个平面
	static Plane fromPoints(const Point3& p0, const Point3& p1, const Point3& p2);

    // 归一化法线
    void normalize();

    // 计算点到平面的有符号距离
    double signedDistance(const Point3& point) const;

    // 计算点在平面上的投影点
    Point3 projectPoint(const Point3& point) const;

    // 判断点是否在平面上（误差范围内）
    bool contains(const Point3& point, double eps = EPS_ABS) const;

    // 判断点相对于平面的位置（正面/背面/在平面上）
    PlaneSide getSide(const Point3& point, double eps = EPS_ABS) const;

	// 构造函数
	//Plane(const Vector3& normal, const Point3& origin) {
	//	// 断言法向量不能是零向量
	//	assert(normal.length() > 1e-9 && "Plane normal vector cannot be zero.");
	//	this->normal = normal.normalized();  // 确保法向量是单位向量
	//	this->origin = origin;
	//}

	//// 归一化法线
	//void normalize() {
	//	this->normal = this->normal.normalized();
	//}

	//// 计算点到平面的有符号距离
	//double signedDistance(const Point3& point) const {
	//	// 点到平面的距离 = (点 - 平面上任意一点) · 法向量
	//	Vector3 vecToPoint = point - origin;
	//	return vecToPoint.dot(normal);
	//}

	//// 计算点在平面上的投影点
	//Point3 projectPoint(const Point3& point) const {
	//	double distance = signedDistance(point);
	//	// 投影点 = 原点 - 距离 * 法向量
	//	return point - normal * distance;
	//}

	//// 判断点是否在平面上（误差范围内）
	//bool contains(const Point3& point, double eps) const {
	//	return std::fabs(signedDistance(point)) < eps;
	//}

	//// 判断点相对于平面的位置（正面/背面/在平面上）
	//PlaneSide getSide(const Point3& point, double eps) const {
	//	double distance = signedDistance(point);
	//	if (distance > eps) {
	//		return PlaneSide::FRONT;
	//	}
	//	if (distance < -eps) {
	//		return PlaneSide::BACK;
	//	}
	//	return PlaneSide::ON_PLANE;
	//}



};
