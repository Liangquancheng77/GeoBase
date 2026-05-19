#include "../include/GeoBase/AABB.h"
#include <cassert>
#include <algorithm>
#include <limits>
#include "../include/GeoBase/Triangle3.h"
#include "vector"
using namespace std;

// 构造函数
AABB::AABB(const Point3& min, const Point3& max) : min(min), max(max) {
	// 断言 min 的每个分量都小于等于 max 的对应分量
	//assert(min.x <= max.x && min.y <= max.y && min.z <= max.z && "Invalid AABB dimensions");
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

// 重载：取最近的交点（tmin）
bool AABB::intersect(const Ray& ray, HitInfo& info) const {
	double tMinOut, tMaxOut;
	if (!intersect(ray, tMinOut, tMaxOut)) return false;

	info.t = tMinOut;
	info.point = ray.pointAt(tMinOut);

	Vector3 center = getCenter();
	Vector3 extent = max - center;

	if (extent.x < EPS_ABS && extent.y < EPS_ABS && extent.z < EPS_ABS)
	{
		// 退化AABB（点/线/面）无有效法线，赋安全值直接返回
		info.normal = Vector3(0, 1, 0);
		return true;
	}

	Vector3 delta = info.point - center;
	bool onX = std::abs(std::abs(delta.x) - extent.x) < EPS_ABS;
	bool onY = std::abs(std::abs(delta.y) - extent.y) < EPS_ABS;
	bool onZ = std::abs(std::abs(delta.z) - extent.z) < EPS_ABS;

	if (onX && !onY && !onZ) {
		info.normal = Vector3(delta.x > 0 ? 1 : -1, 0, 0);
	}
	else if (!onX && onY && !onZ) {
		info.normal = Vector3(0, delta.y > 0 ? 1 : -1, 0);
	}
	else if (!onX && !onY && onZ) {
		info.normal = Vector3(0, 0, delta.z > 0 ? 1 : -1);
	}
	// 处理棱/顶点上的点（多个面的交点，按x→y→z优先级取法线）
	else {
		if (onX) {
			info.normal = Vector3(delta.x > 0 ? 1 : -1, 0, 0);
		}
		else if (onY) {
			info.normal = Vector3(0, delta.y > 0 ? 1 : -1, 0);
		}
		else {
			info.normal = Vector3(0, 0, delta.z > 0 ? 1 : -1);
		}
	}
	return true;

}
//
//info.t = tMinOut;
//info.point = ray.pointAt(tMinOut);
//// 计算交点处的法线
//Point3 hitPoint = info.point;
//if (std::abs(hitPoint.x - min.x) < EPS_ABS) {
//	info.normal = Vector3(-1, 0, 0);
//}
//else if (std::abs(hitPoint.x - max.x) < EPS_ABS) {
//	info.normal = Vector3(1, 0, 0);
//}
//else if (std::abs(hitPoint.y - min.y) < EPS_ABS) {
//	info.normal = Vector3(0, -1, 0);
//}
//else if (std::abs(hitPoint.y - max.y) < EPS_ABS) {
//	info.normal = Vector3(0, 1, 0);
//}
//else if (std::abs(hitPoint.z - min.z) < EPS_ABS) {
//	info.normal = Vector3(0, 0, -1);
//}
//else if (std::abs(hitPoint.z - max.z) < EPS_ABS) {
//	info.normal = Vector3(0, 0, 1);
//}
//else
//{
//	info.normal = Vector3(0, 0, 0); // 理论上不应该发生，除非数值误差导致交点不在表面上
//}
//return true;


// 判断AABB是否与三角形相交（使用分离轴定理）
bool AABB::intersectTriangleAABB(const Triangle3& tri) const {
	// 先判断三角形的AABB与当前AABB是否相交，如果不相交则直接返回false
	AABB triAABB = tri.getBoundingBox();
	if (!intersects(triAABB))
	{
		return false;
	}

	// 局部坐标系转换：将AABB中心作为原点，三角形顶点转换到局部坐标系
	Vector3 center = getCenter();
	Vector3 extent = getExtent();
	Vector3 localV0 = tri.v0 - center;
	Vector3 localV1 = tri.v1 - center;
	Vector3 localV2 = tri.v2 - center;

	// 三角形的3条边向量
	Vector3 e[3] = { localV1 - localV0, localV2 - localV1, localV0 - localV2 };

	// 分离轴定理：检查13个潜在的分离轴
	// 1. AABB的三个轴（x、y、z）
	// x轴
	double minTriX = std::min({ localV0.x, localV1.x, localV2.x });
	double maxTriX = std::max({ localV0.x, localV1.x, localV2.x });
	if (extent.x < minTriX - EPS_ABS || maxTriX < -extent.x - EPS_ABS) return false;
	// y轴
	double minTriY = std::min({ localV0.y, localV1.y, localV2.y });
	double maxTriY = std::max({ localV0.y, localV1.y, localV2.y });
	if (extent.y < minTriY - EPS_ABS || maxTriY < -extent.y - EPS_ABS) return false;
	// z轴
	double minTriZ = std::min({ localV0.z, localV1.z, localV2.z });
	double maxTriZ = std::max({ localV0.z, localV1.z, localV2.z });
	if (extent.z < minTriZ - EPS_ABS || maxTriZ < -extent.z - EPS_ABS) return false;


	// 2. 三角形的法线
	Vector3 triNormal = tri.getNormal();
	// 处理退化三角形，跳过无效轴
	if (triNormal.length() > EPS_ABS)
	{
		double radius = extent.x * std::abs(triNormal.x) + extent.y * std::abs(triNormal.y) + extent.z * std::abs(triNormal.z);
		double minTri = std::min({ localV0.dot(triNormal), localV1.dot(triNormal), localV2.dot(triNormal) });
		double maxTri = std::max({ localV0.dot(triNormal), localV1.dot(triNormal), localV2.dot(triNormal) });
		if (radius < minTri - EPS_ABS || maxTri < -radius - EPS_ABS) return false;
	}

	// 3. 三角形的三条边与AABB的三个轴的叉积（9个轴）
	Vector3 axis;
	// 3.1 AABB的X轴 × 三角形的边
	for (size_t i = 0; i < 3; i++)
	{
		axis = Vector3(1, 0, 0).cross(e[i]);
		if (axis.length() > EPS_ABS)
		{
			axis = axis.normalized();
			double r = extent.y * std::abs(axis.y) + extent.z * std::abs(axis.z);
			double p0 = localV0.dot(axis); double p1 = localV1.dot(axis); double p2 = localV2.dot(axis);
			double minTri = std::min({ p0, p1, p2 });
			double maxTri = std::max({ p0, p1, p2 });
			if (r < minTri - EPS_ABS || maxTri < -r - EPS_ABS) return false;
		}
	}
	// 3.2 AABB的Y轴 × 三角形的边
	for (size_t i = 0; i < 3; i++)
	{
		axis = Vector3(0, 1, 0).cross(e[i]);
		if (axis.length() > EPS_ABS)
		{
			axis = axis.normalized();
			double r = extent.x * std::abs(axis.x) + extent.z * std::abs(axis.z);
			double p0 = localV0.dot(axis); double p1 = localV1.dot(axis); double p2 = localV2.dot(axis);
			double minTri = std::min({ p0, p1, p2 });
			double maxTri = std::max({ p0, p1, p2 });
			if (r < minTri - EPS_ABS || maxTri < -r - EPS_ABS) return false;
		}
	}
	// 3.3 AABB的Z轴 × 三角形的边
	for (size_t i = 0; i < 3; i++)
	{
		axis = Vector3(0, 0, 1).cross(e[i]);
		if (axis.length() > EPS_ABS)
		{
			axis = axis.normalized();
			double r = extent.x * std::abs(axis.x) + extent.y * std::abs(axis.y);
			double p0 = localV0.dot(axis); double p1 = localV1.dot(axis); double p2 = localV2.dot(axis);
			double minTri = std::min({ p0, p1, p2 });
			double maxTri = std::max({ p0, p1, p2 });
			if (r < minTri - EPS_ABS || maxTri < -r - EPS_ABS) return false;
		}
	}

	return true; // 没有找到分离轴，说明相交

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