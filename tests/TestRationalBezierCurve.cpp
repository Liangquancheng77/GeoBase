#include <gtest/gtest.h>
#include "../include/GeoBase/RationalBezierCurve.h"

// 用7个控制点和特定权重构造一个精确圆（权重：中心控制点权重√2/2，其他为1），验证 evaluate(0.5) 的精度
TEST(RationalBezierCurveTest, RationalBezierCurveTest1) {
    // ========== 1. 构造7个控制点（对称单位圆配置，z=0 平面） ==========
    // 按左右对称排布，目标为圆心在原点、半径=1的精确圆弧
    std::vector<Point3> controlPoints = {
        Point3(1.0,  0.0,   0.0),   // P0 右端点（在圆上）
        Point3(1.0,  0.6,   0.0),   // P1
        Point3(0.5,  1.2,   0.0),   // P2
        Point3(0.0,  1.4142, 0.0),  // P3 中心控制点（对应圆弧最高点）
        Point3(-0.5,  1.2,   0.0),   // P4 与P2对称
        Point3(-1.0,  0.6,   0.0),   // P5 与P1对称
        Point3(-1.0,  0.0,   0.0)    // P6 左端点（在圆上）
    };

    // ========== 2. 设置权重：中心控制点权重 √2/2，其余为1 ==========
    double sqrt2_half = std::sqrt(2.0) / 2.0; // ≈ 0.70710678
    std::vector<double> weights = {
        1.0,
        1.0,
        1.0,
        sqrt2_half, // 中心控制点 P3 的权重
        1.0,
        1.0,
        1.0
    };

    // ========== 3. 创建有理贝塞尔曲线 ==========
    RationalBezierCurve curve(controlPoints, weights);

    // ========== 4. 计算 t=0.5 处的曲线上的点 ==========
    Point3 pt = curve.evaluate(0.5);

    // ========== 5. 精度验证：计算该点到圆心的距离，与理论半径1对比 ==========
    double distance = std::sqrt(pt.x * pt.x + pt.y * pt.y + pt.z * pt.z);
    double theoryRadius = 1.0;
    double error = std::abs(distance - theoryRadius);

    // ========== 6. 输出结果 ==========
    std::cout << "=== 有理贝塞尔圆精度测试 (t=0.5) ===" << std::endl;
    std::cout << "曲线上点坐标: (" << pt.x << ", " << pt.y << ", " << pt.z << ")" << std::endl;
    std::cout << "到圆心距离:   " << distance << std::endl;
    std::cout << "理论半径:     " << theoryRadius << std::endl;
    std::cout << "绝对误差:     " << error << std::endl;

    // 精度断言（可接入 GTest/单元测试框架）
    if (error < EPS_ABS) {
        std::cout << "right" << std::endl;
    }
    else {
        std::cout << "error" << std::endl;
    }

}

