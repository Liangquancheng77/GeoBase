#pragma once
#include "Vector3.h"
#include <limits>

struct HitInfo {
    double t;          // 射线参数：交点 = ray.origin + t * ray.direction
    Point3 point;      // 交点的世界坐标
    Vector3 normal;    // 交点处的几何体法线（已按射线方向修正朝向）

    // 默认构造：t初始化为无穷大，方便判断是否命中
    HitInfo()
        : t(std::numeric_limits<double>::infinity()) {
    }
};