#pragma once
#include "Vector3.h"
#include "Matrix4.h"
#include "Ray.h"
#include "GeometryCommon.h"

struct Triangle3;

struct AABB
{
	Point3 min; // 包围盒最小点
	Point3 max; // 包围盒最大点

	// 默认构造：创建一个无效的AABB（min初始化为最大值，max初始化为最小值）
	AABB() : min(std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max()),
		max(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest()) {
	}

	// 构造函数
	AABB(const Point3& min, const Point3& max);

	// 获取最小点
	Point3 getMin() const;

	// 获取最大点
	Point3 getMax() const;

	// 获取包围盒的中心点
	Point3 getCenter() const;

	// 获取包围盒的半边长（从中心到各轴边界的距离）
	Vector3 getExtent() const;

	// 扩展AABB，使其包含一个新的点
    // 对每个轴取当前边界和新点坐标的最小值/最大值
	void expand(const Point3& point);

	// 合并另一个AABB
    // 将当前AABB扩展为包含自身和另一个AABB的并集
	void merge(const AABB& other);

	static AABB merge(const AABB& one, const AABB& other);

	// 判断一个点是否在AABB内部
	bool contains(const Point3& point) const;

	// 判断与另一个AABB是否相交
	bool intersects(const AABB& other) const;

	// 用矩阵变换AABB，返回新的AABB
	AABB transform(const Matrix4& mat) const;

	// 射线与AABB求交，返回是否相交以及交点参数tMinOut和tMaxOut
	bool intersect(const Ray& ray, double& tMinOut, double& tMaxOut) const;

	// 重载：取最近的交点（tmin）
	bool intersect(const Ray& ray, HitInfo& info) const;

	// 判断AABB是否与三角形相交（使用分离轴定理）
	bool intersectTriangleAABB(const Triangle3& tri) const;

	// AABB与球体相交测试
	static bool boxIntersectsSphere(const AABB& box, const Point3& center, double radius);

	// AABB与点的距离平方（如果点在AABB内部则为0）
	static double distSqToPoint(const AABB& box, const Point3& point);

};