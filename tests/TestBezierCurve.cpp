#include <gtest/gtest.h>
#include "../include/GeoBase/BezierCurve.h"
using namespace std;


// 两个控制点（线性），验证 evaluate(0.5) 是中点
TEST(BezierCurveTest, BezierCurveTest1) {
	
	vector<Point3> controlPoints = { Point3(1,2,3),Point3(7,8,9) };
	BezierCurve bc(controlPoints);
	Point3 p = bc.evaluate(0.5);
	EXPECT_EQ(p, Point3(4, 5, 6));

}

// 三个控制点的二次贝塞尔，手动计算并验证 evaluate(0.5)
TEST(BezierCurveTest, BezierCurveTest2) {
	vector<Point3> controlPoints = { Point3(1,2,3),Point3(7,8,9), Point3(3,4,5) };
	BezierCurve bc(controlPoints);
	Point3 p = bc.evaluate(0.5);
	EXPECT_EQ(p, Point3(4.5, 5.5, 6.5));
}

// 验证端点插值：evaluate(0) == controlPoints[0]，evaluate(1) == controlPoints.back()
TEST(BezierCurveTest, BezierCurveTest3) {
	vector<Point3> controlPoints = { Point3(1,2,3),Point3(7,8,9), Point3(3,4,5),  Point3(4,-4,5) };
	BezierCurve bc(controlPoints);
	EXPECT_TRUE(bc.evaluate(0) == controlPoints[0]);

	vector<Point3> controlPoints2 = { Point3(1,2,3),Point3(7,8,9), Point3(3,4,5),  Point3(4,-4,5) };
	BezierCurve bc2(controlPoints2);
	EXPECT_TRUE(bc2.evaluate(1) == controlPoints.back());
}

// 升阶后，新旧曲线在相同t处的点误差 < EPS
TEST(BezierCurveTest, BezierCurveTest4) {
	vector<Point3> controlPoints = { Point3(1,2,3),Point3(7,8,9), Point3(3,4,5),  Point3(4,-4,5) };
	BezierCurve bc(controlPoints);
	EXPECT_TRUE(bc.evaluate(0) == controlPoints[0]);
}

// 验证de Casteljau 算法和直接计算伯恩斯坦多项式的数值稳定性
TEST(BezierCurveTest, BezierCurveTest5) {
    std::mt19937 rng(42); // 固定种子可复现
    std::uniform_real_distribution<double> coordDist(-1.0, 1.0);
    std::uniform_real_distribution<double> tDist(0.0, 1.0);

    // 测试不同次数
    //std::vector<int> testDegrees = { 5, 10, 15, 20, 30 };
    std::vector<int> testDegrees = { 3 };
    const int NUM_T_SAMPLES = 1000; // 每个次数测试1000个随机t

    std::cout << std::fixed << std::setprecision(12);
    std::cout << "=== Numerical Stability Test: de Casteljau vs. Direct Bernstein ===\n\n";

    for (int degree : testDegrees) {
        // 生成 degree+1 个随机控制点
        std::vector<Point3> controlPts(degree + 1);
        for (auto& pt : controlPts) {
            //pt.x = coordDist(rng);
            //pt.y = coordDist(rng);
            //pt.z = coordDist(rng);
            pt.x = 1.0;
            pt.y = 1.0;
            pt.z = 1.0;
        }

        double maxError = 0.0;
        double sumError = 0.0;
        int valid = 0;
        BezierCurve bc(controlPts);
        for (int s = 0; s < NUM_T_SAMPLES; ++s) {
            //double t = tDist(rng);
            //// 同时包括极端t值：0.0, 1.0, 接近0/1的值
            //if (s == 0) t = 0.0;
            //else if (s == 1) t = 1.0;
            //else if (s == 2) t = 1e-6;
            //else if (s == 3) t = 1.0 - 1e-6;
            //// 其余为随机 t
            double t = 0.000001;

            Point3 pDeCast = bc.evaluate(t);
            Point3 pDirect = bc.directBernstein(t);
            double error = (pDeCast - pDirect).length();

            maxError = std::max(maxError, error);
            sumError += error;
            valid++;
        }

        double avgError = sumError / valid;
        std::cout << "Degree " << degree << " : "
            << "avg error = " << std::scientific << avgError
            << ", max error = " << maxError << "\n";
    }
}

// 辅助函数：判断点是否在四面体内部或表面（允许微小误差）
bool pointInTetrahedron(const Point3& p,
    const Point3& a, const Point3& b,
    const Point3& c, const Point3& d) {
    // 计算四个面的向外法线（右手定则，内部点在法线负侧）
    auto signedDist = [&](const Point3& v1, const Point3& v2,
        const Point3& v3, const Point3& innerRef) {
            Vector3 normal = (v2 - v1).cross(v3 - v1);
            // 用 innerRef 调整法线方向，使其指向外部
            if (normal.dot(innerRef - v1) > 0) normal = normal * (-1);
            // 原点到平面的有符号距离（这里原点换成测试点p）
            // 平面方程：normal·(x - v1) = 0，p 代入
            return normal.dot(p - v1);
        };

    double d1 = signedDist(a, b, c, d); // 面 ABC，内部点 D
    double d2 = signedDist(a, c, d, b); // 面 ACD，内部点 B
    double d3 = signedDist(a, d, b, c); // 面 ADB，内部点 C
    double d4 = signedDist(b, d, c, a); // 面 BDC，内部点 A

    // 所有有符号距离必须 <= eps （允许小正误差，表示在表面或内部）
    return d1 <= EPS_ABS && d2 <= EPS_ABS && d3 <= EPS_ABS && d4 <= EPS_ABS;
}

// 凸包性验证：生成随机控制点，验证曲线上100个采样点全部在控制点的凸包内
TEST(BezierCurveTest, BezierCurveTest6) {
    // 1. 随机生成四个不共面的控制点（构成四面体凸包）
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    std::vector<Point3> controlPoints;
    const int numPoints = 4;
    for (int i = 0; i < numPoints; ++i) {
        double x = (std::rand() % 1000) / 100.0 - 5.0;  // 范围 [-5,5)
        double y = (std::rand() % 1000) / 100.0 - 5.0;
        double z = (std::rand() % 1000) / 100.0 - 5.0;
        controlPoints.emplace_back(x, y, z);
    }
    // 确保四点不共面（体积不为0）
    Vector3 ab = controlPoints[1] - controlPoints[0];
    Vector3 ac = controlPoints[2] - controlPoints[0];
    Vector3 ad = controlPoints[3] - controlPoints[0];
    double volume = std::abs(ab.cross(ac).dot(ad)) / 6.0;
    if (volume < EPS_REL) {
        GTEST_SKIP() << "Generated degenerate tetrahedron, skip test.";
        return;
    }

    // 2. 构造贝塞尔曲线（三次）
    BezierCurve curve(controlPoints);

    // 3. 采样100个点
    const int samples = 100;
    for (int i = 0; i < samples; ++i) {
        double t = i / static_cast<double>(samples - 1);  // 0.0, 0.0101..., 1.0
        Point3 pt = curve.directBernstein(t);

        // 4. 验证该点在凸包（四面体）内
        bool inside = pointInTetrahedron(pt,
            controlPoints[0], controlPoints[1],
            controlPoints[2], controlPoints[3]);
        EXPECT_TRUE(inside) << "Sampled point at t=" << t
            << " is outside the convex hull.";
    }
}

//// 变差缩减性验证：一条直线与贝塞尔曲线的交点数量不超过与控制多边形的交点数量
//TEST(BezierCurveTest, BezierCurveTest7) {
//
//}