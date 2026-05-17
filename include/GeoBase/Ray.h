#pragma once
#include "Vector3.h"
//#include "Plane.h"

struct Ray
{
	    Point3 origin;
		Vector3 direction; // 单位向量

		// 构造函数
		Ray(const Point3& origin, const Vector3& direction);

		// 计算射线在参数t处的点
		Point3 pointAt(double t) const;

		//// 射线与平面求交：判断射线是否与平面相交
		//bool intersect(const Plane& plane) const;

};
