#pragma once
#include "Vector3.h"
#include "AABB.h"

struct Triangle3
{
	Point3 v0;
	Point3 v1;
	Point3 v2;

	// 构造函数
	Triangle3(const Point3& v0, const Point3& v1, const Point3& v2);

	// 访问三角形的顶点
	Point3 getVertex(int index) const;

	// 获取三角形的单位法线向量
	Vector3 getNormal() const;

	// 计算三角形的面积
	double getArea() const;

	//// 计算三角形的有向面积
	//double getSignedArea() const;

	// 计算点p在三角形内的重心坐标(u, v, w)
	Vector3 getBarycentric(const Point3& p) const;

	// 判断点p是否在三角形内
	bool contains(const Point3& p) const;

	// 获取三角形的轴对齐包围盒（AABB）
	AABB getBoundingBox() const;


};

