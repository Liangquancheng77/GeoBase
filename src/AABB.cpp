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