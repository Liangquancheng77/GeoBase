#pragma once
#include <cmath>

// 高精度圆周率 足够几何算法所有场景
constexpr double PI = 3.14159265358979323846;

//eps，全局统一管理
constexpr double EPS_ABS = 1e-9;
constexpr double EPS_REL = 1e-6;

// 角度转弧度
inline double DegToRad(double deg)
{
    return deg * PI / 180.0;
}

// 弧度转角度
inline double RadToDeg(double rad)
{
    return rad * 180.0 / PI;
}