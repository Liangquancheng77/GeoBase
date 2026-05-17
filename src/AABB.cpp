#include "../include/GeoBase/AABB.h"
#include <cassert>
#include <algorithm>
#include <limits>

// 构造函数
AABB::AABB(const Point3& min, const Point3& max) : min(min), max(max) {
	// 断言 min 的每个分量都小于等于 max 的对应分量
	assert(min.x <= max.x && min.y <= max.y && min.z <= max.z && "Invalid AABB dimensions");
}

// 获取最小点
Point3 AABB::getMin() const {
	return min;
}

// 获取最大点
Point3 AABB::getMax() const {
	return max;
}

// 获取包围盒的中心点
Point3 AABB::getCenter() const {
	return (min + max) * 0.5;
}

// 获取包围盒的半边长（从中心到各轴边界的距离）
Vector3 AABB::getExtent() const {
	return (max - min) * 0.5;
}

// 扩展AABB，使其包含一个新的点
void AABB::expand(const Point3& point) {
	min.x = std::min(min.x, point.x);
	min.y = std::min(min.y, point.y);
	min.z = std::min(min.z, point.z);
	max.x = std::max(max.x, point.x);
	max.y = std::max(max.y, point.y);
	max.z = std::max(max.z, point.z);
}

// 合并另一个AABB
void AABB::merge(const AABB& other) {
	min.x = std::min(min.x, other.min.x);
	min.y = std::min(min.y, other.min.y);
	min.z = std::min(min.z, other.min.z);
	max.x = std::max(max.x, other.max.x);
	max.y = std::max(max.y, other.max.y);
	max.z = std::max(max.z, other.max.z);
}

// 判断一个点是否在AABB内部
bool AABB::contains(const Point3& point) const {
	return (point.x >= min.x - EPS_ABS && point.x <= max.x + EPS_ABS &&
			point.y >= min.y - EPS_ABS && point.y <= max.y + EPS_ABS &&
			point.z >= min.z - EPS_ABS && point.z <= max.z + EPS_ABS);
}


// 判断与另一个AABB是否相交
bool AABB::intersects(const AABB& other) const {
	return (max.x + EPS_ABS >= other.min.x && min.x - EPS_ABS <= other.max.x &&
		max.y + EPS_ABS >= other.min.y && min.y - EPS_ABS <= other.max.y &&
		max.z + EPS_ABS >= other.min.z && min.z - EPS_ABS <= other.max.z);
}

// 用矩阵变换AABB，返回新的AABB
AABB AABB::transform(const Matrix4& mat) const {
	Vector3 corners[8] = {
		{ min.x, min.y, min.z },
		{ max.x, min.y, min.z },
		{ min.x, max.y, min.z },
		{ max.x, max.y, min.z },
		{ min.x, min.y, max.z },
		{ max.x, min.y, max.z },
		{ min.x, max.y, max.z },
		{ max.x, max.y, max.z }
	};
	double minX = std::numeric_limits<double>::max();
	double minY = std::numeric_limits<double>::max();
	double minZ = std::numeric_limits<double>::max();
	double maxX = std::numeric_limits<double>::lowest();
	double maxY = std::numeric_limits<double>::lowest();
	double maxZ = std::numeric_limits<double>::lowest();
	for (size_t i = 0; i < 8; i++)
	{
		Point3 newCorner = mat.transformPoint(corners[i]);
		minX = std::min(minX, newCorner.x);
		minY = std::min(minY, newCorner.y);
		minZ = std::min(minZ, newCorner.z);
		maxX = std::max(maxX, newCorner.x);
		maxY = std::max(maxY, newCorner.y);
		maxZ = std::max(maxZ, newCorner.z);
	}
	return AABB(Point3(minX, minY, minZ), Point3(maxX, maxY, maxZ));
}

// 射线与AABB求交，返回是否相交以及交点参数tMinOut和tMaxOut
bool AABB::intersect(const Ray& ray, double& tMinOut, double& tMaxOut) const {
	double tix, tiy, tiz, tox, toy, toz;

	if (std::abs(ray.direction.x) < EPS_ABS)
	{
		if (ray.origin.x < min.x || ray.origin.x > max.x)
		{
			return false;
		}
		tix = std::numeric_limits<double>::lowest();
		tox = std::numeric_limits<double>::max();
	}
	else {
		double tx1 = (min.x - ray.origin.x) / ray.direction.x;
		double tx2 = (max.x - ray.origin.x) / ray.direction.x;
		if (ray.direction.x > EPS_ABS)
		{
			tix = tx1;
			tox = tx2;
		} 
		else {
			tix = tx2;
			tox = tx1;
		}
	}

	if (std::abs(ray.direction.y) < EPS_ABS)
	{
		if (ray.origin.y < min.y || ray.origin.y > max.y)
		{
			return false;
		}
		tiy = std::numeric_limits<double>::lowest();
		toy = std::numeric_limits<double>::max();
	}
	else
	{
		double ty1 = (min.y - ray.origin.y) / ray.direction.y;
		double ty2 = (max.y - ray.origin.y) / ray.direction.y;
		if (ray.direction.y > EPS_ABS)
		{
			tiy = ty1;
			toy = ty2;
		}
		else
		{
			tiy = ty2;
			toy = ty1;
		}
	}

	if (std::abs(ray.direction.z) < EPS_ABS)
	{
		if (ray.origin.z < min.z || ray.origin.z > max.z)
		{
			return false;
		}
		tiz = std::numeric_limits<double>::lowest();
		toz = std::numeric_limits<double>::max();
	}
	else
	{
		double tz1 = (min.z - ray.origin.z) / ray.direction.z;
		double tz2 = (max.z - ray.origin.z) / ray.direction.z;
		if (ray.direction.z > EPS_ABS)
		{
			tiz = tz1;
			toz = tz2;
		}
		else
		{
			tiz = tz2;
			toz = tz1;
		}
	}

	double tEnter = std::max({ tix, tiy, tiz });
	double tExit = std::min({ tox, toy, toz });
	if (tEnter > tExit || tExit < 0)
	{
		return false;
	}
	tMinOut = tEnter;
	tMaxOut = tExit;
	return true;
}





// 射线与AABB求交，返回是否相交以及交点参数tMinOut和tMaxOut
//bool AABB::intersect(const Ray& ray, double& tMinOut, double& tMaxOut) const {
//	double tMin = (min.x - ray.origin.x) / ray.direction.x;
//	double tMax = (max.x - ray.origin.x) / ray.direction.x;
//	if (tMin > tMax) std::swap(tMin, tMax);
//	double tyMin = (min.y - ray.origin.y) / ray.direction.y;
//	double tyMax = (max.y - ray.origin.y) / ray.direction.y;
//	if (tyMin > tyMax) std::swap(tyMin, tyMax);
//	if ((tMin > tyMax) || (tyMin > tMax))
//	return false;
//	if (tyMin > tMin)
//	tMin = tyMin;
//	if (tyMax < tMax)
//		tMax = tyMax;
//	double tzMin = (min.z - ray.origin.z) / ray.direction.z;
//	double tzMax = (max.z - ray.origin.z) / ray.direction.z;
//	if (tzMin > tzMax) std::swap(tzMin, tzMax);
//	if ((tMin > tzMax) || (tzMin > tMax))
//	return false;
//	if (tzMin > tMin)
//	tMin = tzMin;
//	if (tzMax < tMax)
//		tMax = tzMax;
//	tMinOut = tMin;
//	tMaxOut = tMax;
//	return true;
//}