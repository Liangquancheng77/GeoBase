#pragma once
#include "Vector3.h"
#include "AABB.h"
#include "GeometryCommon.h"

struct Triangle3
{
	Point3 v0;
	Point3 v1;
	Point3 v2;

	// 构造函数
	Triangle3(const Point3& v0, const Point3& v1, const Point3& v2);

	Triangle3() : v0(0, 0, 0), v1(0, 0, 0), v2(0, 0, 0) {}

	// 访问三角形的顶点
	Point3 getVertex(int index) const;

	// 获取三角形的单位法线向量
	Vector3 getNormal() const;

	// 计算三角形的面积
	double getArea() const;

	// 用海伦公式算面积：先由三个顶点算出三条边长，再套公式。// 和上面的 getArea()（叉积法）算的是同一个面积，但走的是完全不同的路。// 这条路在三角形很扁时会丢精度，正好拿来和 getArea() 对比。
	double getAreaByHeron() const;

	// 更稳的海伦写法：先把边长从大到小排好，再换一种括号组合。// 能撑到更扁的三角形，但后面会看到它也救不彻底。
	double getAreaByHeronStable() const;

	//// 计算三角形的有向面积
	//double getSignedArea() const;

	// 计算点p在三角形内的重心坐标(u, v, w)
	Vector3 getBarycentric(const Point3& p) const;

	// 判断点p是否在三角形内
	bool contains(const Point3& p) const;

	// 获取三角形的轴对齐包围盒（AABB）
	AABB getBoundingBox() const;

	// 射线-三角形求交（Möller-Trumbore）
	bool intersect(const Ray& ray, double& t, double& u, double& v, bool cullBackface = true) const;

	// 带HitInfo的重载版本（通用接口）
	bool intersect(const Ray& ray, HitInfo& info, bool cullBackface = false) const;

	// 判断与另一个三角形是否相交（使用分离轴SAT定理）
	bool intersect_sat(const Triangle3& other) const;

	//// 判断与另一个三角形是否相交（效率更高的实现）
	//bool triTriIntersect(const Triangle3& other) const;

	//// 判断与另一个三角形是否相交（使用分离轴SAT定理）并返回两个交点（如果有）
	//bool intersect_sat(const Triangle3& other, Point3& intersectionPoint1, Point3& intersectionPoint2) const;

};

