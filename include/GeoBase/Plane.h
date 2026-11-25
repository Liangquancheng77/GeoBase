#pragma once
#include "Vector3.h"
#include "Ray.h"
#include "GeometryCommon.h"

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



    // 故意不把方向调成单位长，直接算距离。// 方向有多长，算出来的距离就被放大多少倍，用来看清调成单位长的作用。
    double signedDistanceWithRawNormal(const Point3& point, const Vector3& rawNormal) const;

    // 把法向量绕 y 轴转一个很小的角度（弧度），再算距离。// 转完仍是单位长，所以这里纯粹是「方向偏了一点」，不是长度问题。
    double signedDistanceWithTiltedNormal(const Point3& point, double tilt) const;

    // 输入差一丁点，输出最多会被放大多少倍。// 等于「点到平面基准点的距离」除以「点到平面的距离」。
    double distanceAmplification(const Point3& point) const;

    // 把算错的距离当成对的，反推这个点本来该在哪儿。
    Point3 pointThatExplainsDistance(double wrongDistance, const Point3& point) const;




    // 计算点在平面上的投影点
    Point3 projectPoint(const Point3& point) const;

    // 判断点是否在平面上（误差范围内）
    bool contains(const Point3& point, double eps = EPS_ABS) const;

    // 判断点相对于平面的位置（正面/背面/在平面上）
    PlaneSide getSide(const Point3& point, double eps = EPS_ABS) const;

	// 判断射线是否与平面相交，返回交点参数t和交点坐标
    bool intersect(const Ray& ray, double& t, Point3& hitPoint) const;

    // intersect重载
    bool intersect(const Ray& ray, HitInfo& info) const;

};
