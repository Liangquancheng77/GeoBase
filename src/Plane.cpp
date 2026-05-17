#include "../include/GeoBase/Plane.h"
#include "../include/GeoBase/common.h"
#include <cassert>

// 构造函数
Plane::Plane(const Vector3& normal, const Point3& origin) {
	// 断言法向量不能是零向量
	assert(normal.length() > 1e-9 && "Plane normal vector cannot be zero.");
	this->normal = normal.normalized();  // 确保法向量是单位向量
	this->origin = origin;
}

// 三点确定一个平面
Plane Plane::fromPoints(const Point3& p0, const Point3& p1, const Point3& p2) {
	Vector3 edge1 = p1 - p0;
	Vector3 edge2 = p2 - p0;
	Vector3 normal = edge1.cross(edge2).normalized();
	return Plane(normal, p0);
}

// 归一化法线
void Plane::normalize() {
	this->normal = this->normal.normalized();
}

// 计算点到平面的有符号距离
double Plane::signedDistance(const Point3& point) const {
	// 点到平面的距离 = (点 - 平面上任意一点) · 法向量
	Vector3 vecToPoint = point - origin;
	return vecToPoint.dot(normal);
}

// 计算点在平面上的投影点
Point3 Plane::projectPoint(const Point3& point) const {
	double distance = signedDistance(point);
	// 投影点 = 原点 - 距离 * 法向量
	return point - normal * distance;
}

// 判断点是否在平面上（误差范围内）
bool Plane::contains(const Point3& point, double eps) const {
	return std::fabs(signedDistance(point)) < eps;
}

// 判断点相对于平面的位置（正面/背面/在平面上）
PlaneSide Plane::getSide(const Point3& point, double eps) const {
	double distance = signedDistance(point);
	if (distance > eps) {
		return PlaneSide::FRONT;
	}
	if (distance < -eps) {
		return PlaneSide::BACK;
	}
	return PlaneSide::ON_PLANE;
}

// 判断射线是否与平面相交，返回交点参数t和交点坐标
bool Plane::intersect(const Ray& ray, double& t, Point3& hitPoint) const {
	double  denom = ray.direction.dot(normal);
	if (std::abs(denom) < EPS_ABS) {
		return false; // 射线与平面平行或者在平面上，无有效交点
	}
	t = (origin - ray.origin).dot(normal) / denom;
	if (t + EPS_ABS < 0)
	{
		return false;
	}
	hitPoint = ray.pointAt(t);
	return true;
}
