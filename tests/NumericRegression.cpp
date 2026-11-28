// [D01] GeoBase 数值回归网
// 断言的是「正确行为」：改造前大面积红，改造后必须全绿。
// 参考解一律走独立数学路径（恒等变换 / 中心差分），不用另一份实现自证。
// 这两行必须放在最最最上面！
#define _CRT_SECURE_NO_WARNINGS 
#pragma execution_character_set("utf-8")
#include <gtest/gtest.h>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../include/GeoBase/Common.h"
#include "../include/GeoBase/Vector3.h"
#include "../include/GeoBase/Matrix4.h"
#include "../include/GeoBase/BezierCurve.h"
#include <cstdlib>   // std::getenv
#include <limits>    // std::numeric_limits
#include <iomanip>
#include <iostream>
#include "../include/GeoBase/Triangle3.h"
#include "../include/GeoBase/Plane.h"
#include "../include/GeoBase/Matrix3.h"

//namespace {
//
//    // 参考值：这个三角形底是 1、高是 h，面积本来就等于 0.5 乘 h，不用算就知道。
//    double areaExact(double h) {
//        return 0.5 * h;
//    }
//
//    // 相对误差：算出来的和该有的差了多少，再除以该有的。// 用比例而不是绝对差，数值很小的时候也还能比得出好坏。
//    double relErr(double got, double exact) {
//        return std::abs(got - exact) / std::abs(exact);
//    }
//
//} // namespace
//
//TEST(Day01Float, TwoWaysToComputeOneArea) {
//    // 正常三角形：底 1、高 0.1，面积该是 0.05。
//    Triangle3 tri(Point3(0.0, 0.0, 0.0), Point3(1.0, 0.0, 0.0), Point3(0.5, 0.1, 0.0));
//    double exact = areaExact(0.1);
//    std::cout << std::setprecision(17);
//    std::cout << "heron=" << tri.getAreaByHeron()
//        << "  cross=" << tri.getArea()
//        << "  exact=" << exact << std::endl;
//    // 正常形状下两条路都该很准，误差小于 1e-12。
//    EXPECT_NEAR(tri.getAreaByHeron(), exact, 1e-12);
//    EXPECT_NEAR(tri.getArea(), exact, 1e-12);
//}
//
//TEST(Day01Float, ThinTriangleLosesDigits) {
//    // 高度每次缩小 10 倍，一共 12 档。// 三角形越扁，两条边长越接近半周长，相减丢精度越厉害。
//    std::cout << "h           heronRel     crossRel" << std::endl;
//    for (int k = 1; k <= 12; ++k) {
//        double h = std::pow(10.0, -k);
//        Triangle3 tri(Point3(0.0, 0.0, 0.0), Point3(1.0, 0.0, 0.0), Point3(0.5, h, 0.0));
//        double e1 = relErr(tri.getAreaByHeron(), areaExact(h));
//        double e2 = relErr(tri.getArea(), areaExact(h));
//        std::cout << std::scientific << std::setprecision(3)
//            << h << "    " << e1 << "    " << e2 << std::endl;
//    }
//    // 这一步只打印、不断言具体数字：数字以你这台机器跑出来的为准。
//    SUCCEED();
//}
//
//TEST(Day01Float, ThreeWaysCompared) {
//    // 三条路各算一遍，把相对误差并排打出来，都是从 Triangle3 上调的。
//    std::cout << "h           heronRel     stableRel    crossRel" << std::endl;
//    std::cout << std::scientific << std::setprecision(3);
//    for (int k = 1; k <= 12; ++k) {
//        double h = std::pow(10.0, -k);
//        Triangle3 tri(Point3(0.0, 0.0, 0.0), Point3(1.0, 0.0, 0.0), Point3(0.5, h, 0.0));
//        double e1 = relErr(tri.getAreaByHeron(), areaExact(h));
//        double e3 = relErr(tri.getAreaByHeronStable(), areaExact(h));
//        double e2 = relErr(tri.getArea(), areaExact(h));
//        std::cout << h << "   " << e1 << "   " << e3
//            << "   " << e2 << std::endl;
//    }
//    SUCCEED();
//}
//
//TEST(Day01Float, MachineEpsilon) {
//    // 机器精度：1 加上多少才刚好不再等于 1。// 双精度标准值约 2.22e-16，它的平方根约 1.49e-08。
//    double eps = std::numeric_limits<double>::epsilon();
//    std::cout << std::setprecision(10)
//        << "eps=" << eps
//        << "  sqrt(eps)=" << std::sqrt(eps) << std::endl;
//    EXPECT_LT(eps, 1e-14);
//    SUCCEED();
// //   double a = 2.0;
// ////   double b = 2.0 + (2*eps);
// // double b = 2.0000000000000001;
//	//bool equal = (a == b);
// //   SUCCEED();
//
//    
//}
//
//// 再往下压，看边长什么时候分不出差别 =====
//TEST(Day01Float, PushToSixteenScales) {
//    std::cout << "h           bMinusHalf   heronRel     crossRel" << std::endl;
//    std::cout << std::scientific << std::setprecision(3);
//    for (int k = 12; k <= 16; ++k) {
//        double h = std::pow(10.0, -k);
//        // 斜边 b：由三角形顶点直接算，用来看它离 0.5 还差多少。
//        double b = (Point3(0.5, h, 0.0) - Point3(1.0, 0.0, 0.0)).length();
//        Triangle3 tri(Point3(0.0, 0.0, 0.0), Point3(1.0, 0.0, 0.0), Point3(0.5, h, 0.0));
//        double e1 = relErr(tri.getAreaByHeron(), areaExact(h));
//        double e2 = relErr(tri.getArea(), areaExact(h));
//        std::cout << h << "   " << (b - 0.5) << "   "
//            << e1 << "   " << e2 << std::endl;
//    }
//    SUCCEED();
//}

// ===== Day02 新增：量误差的小尺子 + 第一组检查题 =====// 换个名字是为了不和 Day01 那把尺子重名，两把尺子的算法一样。


//namespace {
//
//    // 相对误差：算出来的和该有的差了多少，再除以该有的。
//    double relErrD2(double got, double exact) {
//        return std::abs(got - exact) / std::abs(exact);
//    }
//
//    // 造一个 z=0 的平面：法向朝 z，平面过原点。
//    Plane flatPlane() {
//        return Plane(Vector3(0.0, 0.0, 1.0), Point3(0.0, 0.0, 0.0));
//    }
//
//} // namespace
//
//TEST(Day02Amplify, NormalLengthScalesTheAnswer) {
//    Plane plane = flatPlane();
//    Point3 p(0.0, 0.0, 1e-3);
//    // 项目自带的算法用的是单位法向，这个结果是对的。
//    double correct = plane.signedDistance(p);
//    std::cout << std::setprecision(10);
//    std::cout << "k        rawDistance   ratio" << std::endl;
//    int ks[] = { 1, 2, 5, 10, 100 };
//    for (int i = 0; i < 5; ++i) {
//        double k = static_cast<double>(ks[i]);
//        Vector3 raw(0.0, 0.0, k);
//        double d = plane.signedDistanceWithRawNormal(p, raw);
//        std::cout << k << "      " << d << "      " << d / correct << std::endl;
//        // 方向长 k 倍，距离就正好错 k 倍。
//        EXPECT_NEAR(d, k * correct, 1e-15);
//    }
//    EXPECT_NEAR(correct, 1e-3, 1e-15);
//}
//
//TEST(Day02Amplify, TinyTiltBlowsUp) {
//    Plane plane = flatPlane();
//    // 点离平面 0.001，法向只偏 1e-9 弧度。
//    double h = 1e-3;
//    double tilt = 1e-9;
//    std::cout << std::scientific << std::setprecision(3);
//    std::cout << "L         dTrue      dTilted     relErr     amplified" << std::endl;
//    int powers[] = { 0, 2, 4, 6, 8 };
//    for (int i = 0; i < 5; ++i) {
//        double L = std::pow(10.0, powers[i]);
//        Point3 p(L, 0.0, h);
//        double dTrue = plane.signedDistance(p);
//        double dTilt = plane.signedDistanceWithTiltedNormal(p, tilt);
//        double e = relErrD2(dTilt, dTrue);
//        std::cout << L << "   " << dTrue << "   " << dTilt
//            << "   " << e << "   " << e / tilt << std::endl;
//    }
//    // 方向没偏时，答案和点沿 x 挪多远无关。
//    EXPECT_NEAR(plane.signedDistance(Point3(1e6, 0.0, h)), h, 1e-15);
//    SUCCEED();
//}
//
//TEST(Day02Amplify, AmplificationMatchesTheory) {
//    Plane plane = flatPlane();
//    double h = 1e-3;
//    double tilt = 1e-9;
//    std::cout << std::scientific << std::setprecision(3);
//    std::cout << "L         measured     predicted" << std::endl;
//    int powers[] = { 0, 2, 4, 6, 8 };
//    for (int i = 0; i < 5; ++i) {
//        double L = std::pow(10.0, powers[i]);
//        Point3 p(L, 0.0, h);
//        double dTrue = plane.signedDistance(p);
//        double dTilt = plane.signedDistanceWithTiltedNormal(p, tilt);
//        double measured = relErrD2(dTilt, dTrue) / tilt;
//        double predicted = plane.distanceAmplification(p);
//        std::cout << L << "   " << measured << "   " << predicted << std::endl;
//    }
//    // 离基准点 1e6、离平面 1e-3，放大倍数约 1e9。
//    EXPECT_NEAR(plane.distanceAmplification(Point3(1e6, 0.0, h)), 1e9, 1e3);
//    SUCCEED();
//}
//
//TEST(Day02Amplify, BackwardErrorExplainsTheWrongAnswer) {
//    Plane plane = flatPlane();
//    Point3 p(0.0, 0.0, 1e-3);
//    double correct = plane.signedDistance(p);
//    // 方向没调成单位长，长度是 5，答案被放大 5 倍。
//    Vector3 raw(0.0, 0.0, 5.0);
//    double wrong = plane.signedDistanceWithRawNormal(p, raw);
//    // 前向误差：算出来的和该有的差了多少。
//    double forward = std::abs(wrong - correct);
//    // 后向误差：把错的当成对的，反推点本来该在哪儿。
//    Point3 p2 = plane.pointThatExplainsDistance(wrong, p);
//    double shift = (p2 - p).length();
//    std::cout << std::setprecision(10)
//        << "correct=" << correct << "  wrong=" << wrong
//        << "  forward=" << forward << "  shift=" << shift << std::endl;
//    // 用正确的公式去算那个稍微不同的点，正好得到错误距离。
//    EXPECT_NEAR(plane.signedDistance(p2), wrong, 1e-15);
//    EXPECT_NEAR(shift, forward, 1e-15);
//}
//
//TEST(Day02Amplify, AmplificationDoesNotDependOnTilt) {
//    Plane plane = flatPlane();
//    double h = 1e-3;
//    Point3 p(1e6, 0.0, h);
//    double dTrue = plane.signedDistance(p);
//    std::cout << std::scientific << std::setprecision(3);
//    std::cout << "tilt        relErr      amplified" << std::endl;
//    for (int k = 6; k <= 12; ++k) {
//        double tilt = std::pow(10.0, -k);
//        double dTilt = plane.signedDistanceWithTiltedNormal(p, tilt);
//        double e = relErrD2(dTilt, dTrue);
//        std::cout << tilt << "   " << e << "   " << e / tilt << std::endl;
//    }
//    SUCCEED();
//}
//
//TEST(Day02Amplify, BigCoordinatesSwallowTheGap) {
//    double base = 1e16;
//    Point3 farOrigin(base, 0.0, 0.0);
//    Point3 farPoint(base + 1.0, 0.0, 0.0);
//    Plane plane(Vector3(1.0, 0.0, 0.0), farOrigin);
//    double d = plane.signedDistance(farPoint);
//    std::cout << std::setprecision(17)
//        << "base+1 == base ? " << (farPoint.x == farOrigin.x)
//        << "   distance=" << d << "   shouldBe=1" << std::endl;
//    // 两个很大的数存不下那 1 的差别，相减之后直接是 0。
//    EXPECT_NEAR(d, 0.0, 1e-15);
//}
//
//// 新增：三把小尺子
//
//
//namespace d3 {
//
//    // 圆周率，造正多边形和算准确面积时要用。
//    const double PI_D3 = 3.14159265358979323846;
//
//    // 造一个圆心在指定位置的正 n 边形，顶点按逆时针排好。
//    std::vector<Point3> regularPolygon(int n, double radius, double offset) {
//        std::vector<Point3> pts;
//        for (int i = 0; i < n; ++i) {
//            double a = 2.0 * PI_D3 * i / n;
//            pts.push_back(Point3(offset + radius * std::cos(a), 
//                offset + radius * std::sin(a), 0.0));
//        }
//        return pts;
//    }
//
//    // 正 n 边形的准确面积：n 个等腰三角形，每块 0.5*r*r*sin(2π/n)。
//    double exactPolygonArea(int n, double radius) {
//        return 0.5 * n * radius * radius * std::sin(2.0 * PI_D3 / n);
//    }
//
//    // 相对误差：算出来的和该有的差了多少，再除以该有的。
//    double relErrD3(double got, double exact) {
//        return std::abs(got - exact) / std::abs(exact);
//    }
//
//} // namespace d3// 扫六档项数，把 N 个 0.1 加起来，看普通累加的误差怎么攒大、补偿累加为什么一直准。
//TEST(Day03Sum, ManyTinyTermsDriftAway) {
//    std::cout << std::scientific << std::setprecision(3);
//    std::cout << "N         plain        compensated   exact       errPlain    errComp" << std::endl;
//    int counts[] = { 10, 100, 1000, 10000, 100000, 1000000 };
//    for (int i = 0; i < 6; ++i) {
//        int n = counts[i];
//        std::vector<double> parts(n, 0.1);
//        double p = Triangle3::sumPlain(parts);
//        double k = Triangle3::sumCompensated(parts);
//        // 该有的答案用乘法一步算出来，和一项一项加是两条不同的路。
//        double exact = n * 0.1;
//        std::cout << n << "   " << p << "   " << k << "   " << exact
//            << "   " << std::abs(p - exact) << "   " << std::abs(k - exact) << std::endl;
//    }
//    std::vector<double> big(1000000, 0.1);
//    // 一百万项时普通累加差到 1e-6 量级，补偿累加仍和该有的答案一致。
//    EXPECT_NEAR(Triangle3::sumCompensated(big), 100000.0, 1e-9);
//    EXPECT_GT(std::abs(Triangle3::sumPlain(big) - 100000.0), 1e-7);
//}
//
//// 先放一个巨大数、塞进一万个很小的数、再减掉巨大数，看两种累加差多少。
//TEST(Day03Sum, BigPlusSmallMinusBig) {
//    int counts[] = { 10000, 100000, 1000000 };
//    std::cout << std::setprecision(10);
//    std::cout << "N         plain      compensated   exact" << std::endl;
//    for (int i = 0; i < 3; ++i) {
//        int n = counts[i];
//        std::vector<double> parts;
//        parts.push_back(1e16);
//        for (int j = 0; j < n; ++j) parts.push_back(0.001);
//        parts.push_back(-1e16);
//        double p = Triangle3::sumPlain(parts);
//        double k = Triangle3::sumCompensated(parts);
//        double exact = n * 0.001;
//        std::cout << n << "   " << p << "   " << k << "   " << exact << std::endl;
//    }
//    std::vector<double> one(100000, 0.001);
//    one.insert(one.begin(), 1e16);
//    one.push_back(-1e16);
//    EXPECT_NEAR(Triangle3::sumPlain(one), 0.0, 1e-12);
//    EXPECT_NEAR(Triangle3::sumCompensated(one), 100.0, 1e-9);
//}
//
//// 把正多边形拆成一串三角形求总面积，和课本公式对上，并比较两种累加。
//TEST(Day03Polygon, AreaMatchesExactFormula) {
//    int counts[] = { 6, 12, 36, 100, 1000, 10000 };
//    std::cout << std::scientific << std::setprecision(6);
//    std::cout << "N         plain        compensated   exact        errPlain" << std::endl;
//    for (int i = 0; i < 6; ++i) {
//        int n = counts[i];
//        std::vector<Point3> poly = d3::regularPolygon(n, 1.0, 0.0);
//        double p = Triangle3::polygonArea(poly, false);
//        double k = Triangle3::polygonArea(poly, true);
//        double exact = d3::exactPolygonArea(n, 1.0);
//        std::cout << n << "   " << p << "   " << k << "   " << exact
//            << "   " << std::abs(p - exact) << std::endl;
//        EXPECT_NEAR(p, exact, 1e-10);
//        EXPECT_NEAR(k, exact, 1e-10);
//    }
//}
//
//// 按面积加权算正多边形的质心，圆心在哪儿质心就该在哪儿，看两种累加偏了多少。
//TEST(Day03Polygon, CentroidStaysAtOrigin) {
//    int counts[] = { 6, 12, 36, 100, 1000, 10000 };
//    std::cout << std::scientific << std::setprecision(3);
//    std::cout << "N         offsetPlain   offsetComp" << std::endl;
//    for (int i = 0; i < 6; ++i) {
//        int n = counts[i];
//        std::vector<Point3> poly = d3::regularPolygon(n, 1.0, 0.0);
//        Point3 c1 = Triangle3::polygonCentroid(poly, false);
//        Point3 c2 = Triangle3::polygonCentroid(poly, true);
//        double e1 = std::sqrt(c1.x * c1.x + c1.y * c1.y);
//        double e2 = std::sqrt(c2.x * c2.x + c2.y * c2.y);
//        std::cout << n << "   " << e1 << "   " << e2 << std::endl;
//        EXPECT_LT(e1, 1e-10);
//        EXPECT_LT(e2, 1e-10);
//    }
//}
//
//// 把 0.1 加 N 次再减掉 N-1 次，答案本该正好是 0.1，看普通累加攒下多少误差。
//TEST(Day03Sum, MixedSignsStillBenefit) {
//    int counts[] = { 10, 100, 1000, 10000, 100000, 1000000 };
//    std::cout << std::scientific << std::setprecision(3);
//    std::cout << "N         plain        compensated   errPlain" << std::endl;
//    for (int i = 0; i < 6; ++i) {
//        int n = counts[i];
//        std::vector<double> parts;
//        for (int j = 0; j < n; ++j) parts.push_back(0.1);
//        for (int j = 0; j < n - 1; ++j) parts.push_back(-0.1);
//        double p = Triangle3::sumPlain(parts);
//        double k = Triangle3::sumCompensated(parts);
//        std::cout << n << "   " << p << "   " << k << "   " << std::abs(p - 0.1) << std::endl;
//        EXPECT_NEAR(k, 0.1, 1e-14);
//    }
//}
//
//// 把多边形整体挪到很远的地方，看补偿累加能不能救回每一块面积自己算歪的部分。
//TEST(Day03Polygon, BigOffsetCannotBeFixedBySummation) {
//    double offs[] = { 0.0, 1e6, 1e9 };
//    std::cout << std::setprecision(12);
//    std::cout << "off        plain        compensated   exact        diffComp" << std::endl;
//    for (int i = 0; i < 3; ++i) {
//        std::vector<Point3> poly = d3::regularPolygon(36, 1.0, offs[i]);
//        double p = Triangle3::polygonArea(poly, false);
//        double k = Triangle3::polygonArea(poly, true);
//        double exact = d3::exactPolygonArea(36, 1.0);
//        std::cout << offs[i] << "   " << p << "   " << k << "   " << exact
//            << "   " << std::abs(k - exact) << std::endl;
//    }
//    SUCCEED();
//}



// ===== Day04 新增：包含语句 + 四把小尺子 + 第一组检查题 =====


namespace d4 {

    // 独立数学路径：这个病态矩阵「最多放大多少倍」的准确公式，不靠求逆自证。
    double exactCondition(double eps) {
        return (3.0 + eps) * (4.0 + eps) / eps;
    }

    // 独立数学路径：只改右端第一个数时，解被放大的准确倍数。
    double exactAmplification(double eps) {
        return 2.0 + 4.0 / eps;
    }

    // 三个分量里绝对值最大的那个，用来量一个解有多大。
    double maxAbsComponent(const Vector3& v) {
        double r = std::fabs(v.x);
        if (std::fabs(v.y) > r) r = std::fabs(v.y);
        if (std::fabs(v.z) > r) r = std::fabs(v.z);
        return r;
    }

    // 两个解差了多少：先量差向量有多大，再除以原来的解有多大。
    double relativeDiff(const Vector3& got, const Vector3& base) {
        double scale = maxAbsComponent(base);
        if (scale == 0.0) return 0.0;
        Vector3 d = got - base;
        return maxAbsComponent(d) / scale;
    }

} // namespace d4// 扫三档 eps，看事先量出的放大上界和课本公式对不对得上。
TEST(Day04Condition, RowSumNormMatchesExactFormula) {
    double epss[] = { 1e-2, 1e-4, 1e-6 };
    std::cout << std::scientific << std::setprecision(3);
    std::cout << "eps       condGot      condExact    relErr" << std::endl;
    for (int i = 0; i < 3; ++i) {
        double eps = epss[i];
        Matrix3 a = Matrix3::nearlySingular(eps);
        double got = Matrix3::conditionNumberByRowSum(a);
        double exact = d4::exactCondition(eps);
        double rel = std::fabs(got - exact) / exact;
        std::cout << eps << "   " << got << "   " << exact << "   " << rel << std::endl;
        EXPECT_NEAR(got, exact, exact * 1e-3);
    }
}

// 右端只改十亿分之一，看解被放大多少倍；和课本公式对照，并确认没超过上界。
TEST(Day04Amplify, TinyChangeInRightHandBlowsUpSolution) {
    double epss[] = { 1e-2, 1e-4, 1e-6 };
    double relB = 1e-9;
    std::cout << std::scientific << std::setprecision(3);
    std::cout << "eps       relX         ampGot       ampExact     cond" << std::endl;
    for (int i = 0; i < 3; ++i) {
        double eps = epss[i];
        Matrix3 a = Matrix3::nearlySingular(eps);
        Vector3 trueX(1.0, 2.0, 3.0);
        Vector3 b = a * trueX;
        Vector3 b2 = b;
        b2.x = b2.x * (1.0 + relB);
        Vector3 x0 = Matrix3::solveByInverse(a, b);
        Vector3 x1 = Matrix3::solveByInverse(a, b2);
        double relX = d4::relativeDiff(x1, x0);
        double amp = relX / relB;
        double exact = d4::exactAmplification(eps);
        double cond = Matrix3::conditionNumberByRowSum(a);
        std::cout << eps << "   " << relX << "   " << amp << "   "
                  << exact << "   " << cond << std::endl;
        EXPECT_NEAR(amp, exact, exact * 1e-3);
        EXPECT_LT(amp, cond);
    }
}

// 换成单位阵（最好算的方程组），同样改右端一点点，看解是不是也只差一点点。
TEST(Day04Amplify, WellBehavedMatrixDoesNotBlowUp) {
    Matrix3 a = Matrix3::identity();
    double cond = Matrix3::conditionNumberByRowSum(a);
    Vector3 trueX(1.0, 2.0, 3.0);
    Vector3 b = a * trueX;
    Vector3 b2 = b;
    double relB = 1e-9; 
    b2.x = b2.x * (1.0 + relB);
    Vector3 x0 = Matrix3::solveByInverse(a, b);
    Vector3 x1 = Matrix3::solveByInverse(a, b2);
    double relX = d4::relativeDiff(x1, x0);
    std::cout << std::scientific << std::setprecision(3);
    std::cout << "identity  cond=" << cond << "  relB=" << relB
        << "  relX=" << relX << "  amp=" << relX / relB << std::endl;
    EXPECT_NEAR(cond, 1.0, 1e-12);
    EXPECT_LT(relX, 1e-8);
}

// 分别改右端三个分量，看三个方向的放大倍数都压在事先量出的上界里。
TEST(Day04Amplify, AllThreeDirectionsStayUnderCondition) {
    double eps = 1e-4;
    Matrix3 a = Matrix3::nearlySingular(eps);
    double cond = Matrix3::conditionNumberByRowSum(a);
    Vector3 trueX(1.0, 2.0, 3.0);
    Vector3 b = a * trueX;
    double relB = 1e-9;
    std::cout << std::scientific << std::setprecision(3);
    std::cout << "which     amp          cond" << std::endl;
    for (int k = 0; k < 3; ++k) {
        Vector3 b2 = b;
        if (k == 0) b2.x = b2.x * (1.0 + relB);
        if (k == 1) b2.y = b2.y * (1.0 + relB);
        if (k == 2) b2.z = b2.z * (1.0 + relB);
        Vector3 x0 = Matrix3::solveByInverse(a, b);
        Vector3 x1 = Matrix3::solveByInverse(a, b2);
        double amp = d4::relativeDiff(x1, x0) / relB;
        std::cout << k << "   " << amp << "   " << cond << std::endl;
        EXPECT_LT(amp, cond);
    }
}

// 把右端改的量从大到小扫一遍，看放大倍数是不是一直不变。
TEST(Day04Amplify, AmplificationDoesNotDependOnPerturbationSize) {
    double eps = 1e-4;
    Matrix3 a = Matrix3::nearlySingular(eps);
    Vector3 trueX(1.0, 2.0, 3.0);
    Vector3 b = a * trueX;
    double relBs[] = { 1e-5, 1e-7, 1e-9, 1e-11 };
    double exact = d4::exactAmplification(eps);
    std::cout << std::scientific << std::setprecision(3);
    std::cout << "relB      relX         amp          exact" << std::endl;
    for (int i = 0; i < 4; ++i) {
        double relB = relBs[i];
        Vector3 b2 = b;
        b2.x = b2.x * (1.0 + relB);
        Vector3 x0 = Matrix3::solveByInverse(a, b);
        Vector3 x1 = Matrix3::solveByInverse(a, b2);
        double relX = d4::relativeDiff(x1, x0);
        std::cout << relB << "   " << relX << "   " << relX / relB
            << "   " << exact << std::endl;
    }
}

// 把 eps 越推越小，看现成的行列式什么时候算成 0；这一步只调行列式，不求逆。
TEST(Day04Condition, DeterminantCollapsesAtTinyEps) {
    double epss[] = { 1e-4, 1e-6, 1e-7, 1e-8 };
    std::cout << std::scientific << std::setprecision(3);
    std::cout << "eps       detGot       detExact     relErr" << std::endl;
    for (int i = 0; i < 4; ++i) {
        double eps = epss[i];
        Matrix3 a = Matrix3::nearlySingular(eps);
        double got = a.determinant();
        double exact = eps * eps;
        std::cout << eps << "   " << got << "   " << exact
            << "   " << std::fabs(got - exact) / exact << std::endl;
    }
    SUCCEED();
}

//TEST(Day04Condition, RowSumNormMatchesExactFormula2) {
//    double a = 0.1;
//	double sum = 0.0;
//    std::cout << std::scientific << std::setprecision(17);
//    for (size_t i = 0; i < 20; i++)
//    {
//		sum += a;
//        // i是循环下标，i+1代表累加了多少次a
//        std::cout << "count=" << i + 1 << " , sum = " << sum << std::endl;
//    }
//    
//}

//TEST(Day04Condition, RowSumNormMatchesExactFormula3) {
//    double eps = std::numeric_limits<double>::epsilon();
//    double eps2 = 1.12e-16;
//    double a = 1.0;
//    double e = a + eps2;
//    //double b = 10000000000000000001;
//    //double d = b - 10000000000000000001;
//    double c = 0.1;
//}









//TEST(Day03Sum, ManyTinyTermsDriftAway2) {
//
//    double a = 1000.00000000000000;
//	double b = 0.00000000000001;
//	double c = a + b;
//    double d = c-a-b;
//    c = c+d;
//    double e = 0.1;
//
//}

//namespace {
//
//    // 【类型别名】定义 4x4 的二维数组，用于在测试中方便地硬编码输入数据
//    using Arr4 = double[4][4];
//
//    // 【辅助函数】将一个 C 风格二维数组转换为自定义的 Matrix4 对象
//    Matrix4 makeMat(const Arr4& v) {
//        Matrix4 m;
//        for (int i = 0; i < 4; ++i)
//            for (int j = 0; j < 4; ++j)
//                m.m[i][j] = v[i][j];
//        return m;
//    }
//
//    // 【辅助函数】快速构造一个标准的 4x4 仿射变换矩阵（包含沿坐标轴的缩放和平移）
//    Matrix4 makeAffine(double s0, double s1, double s2,
//        double tx, double ty, double tz) {
//        const Arr4 v = {
//            { s0, 0.0, 0.0, tx },
//            { 0.0, s1, 0.0, ty },
//            { 0.0, 0.0, s2, tz },
//            { 0.0, 0.0, 0.0, 1.0 }
//        };
//        return makeMat(v);
//    }
//
//    // 【辅助函数】计算两个矩阵乘积 (A*B) 与单位矩阵 (I) 之间的重构误差
//    // 采用逐元素比较，返回绝对值最大的那个误差（即无穷范数意义下的误差）
//    double reconError(const Matrix4& a, const Matrix4& b) {
//        Matrix4 prod = a.multiply(b);
//        double e = 0.0;
//        for (int i = 0; i < 4; ++i)
//            for (int j = 0; j < 4; ++j)
//                e = std::max(e, std::abs(prod.m[i][j] - (i == j ? 1.0 : 0.0)));
//        return e;
//    }
//
//    // 【辅助函数】计算两个三维向量在各个坐标分量上的最大绝对差值
//    double maxAbsDiff(const Vector3& a, const Vector3& b) {
//        return std::max({ std::abs(a.x - b.x),
//                          std::abs(a.y - b.y),
//                          std::abs(a.z - b.z) });
//    }
//
//    // 【辅助函数】使用“中心差分法”数值计算曲线在参数 t 处的导数
//    // 这是一个独立于库中解析导数的参考解，用于防止“用错误代码测试错误代码”
//    Vector3 centralDiff(const BezierCurve& c, double t, double h = 1e-5) {
//        Point3 lo = c.evaluate(t - h);
//        Point3 hi = c.evaluate(t + h);
//        return (hi - lo) / (2.0 * h);
//    }
//
//    // 【辅助函数】返回一组固定坐标的三次贝塞尔曲线控制点，用于统一测试基准
//    std::vector<Point3> bezierControlPoints() {
//        return { Point3(0, 0, 0), Point3(1, 2, 0),
//                 Point3(3, 0, 1), Point3(4, 1, 1) };
//    }
//
//    // 【辅助函数】将测试运行得到的各项指标（如误差值）记录到 CSV 文件
//    // 仅在环境变量 GEABASE_NUMERIC_CSV 被设置时生效，用于生成对比报告
//    void logMetric(const std::string& caseId, const std::string& scale,
//        const std::string& metric, double value, bool pass,
//        const std::string& note = "") {
//        const char* path = std::getenv("GEABASE_NUMERIC_CSV");
//        if (!path || !*path) return;
//        static bool headerWritten = false;
//        std::ofstream f(path, std::ios::app);
//        if (!f) return;
//        if (!headerWritten) {
//            f << "case_id,scale,metric_name,metric_value,pass,note\n";
//            headerWritten = true;
//        }
//        f << caseId << "," << scale << "," << metric << ","
//            << value << "," << (pass ? 1 : 0) << "," << note << "\n";
//    }
//
//} // namespace
//
//// ==================================================================
//// A. 矩阵求逆
//// ==================================================================
//
//// 【测试用例】单位矩阵求逆往返测试：验证 I * I^-1 是否无限接近 I
//TEST(Matrix4Inverse, IdentityRoundTrip) {                 // M4_INV_IDENTITY_01
//    Matrix4 I;
//    Matrix4 inv = I.inverse();
//    double e = reconError(I, inv);
//    logMetric("M4_INV_IDENTITY_01", "1", "recon_error", e, e < 1e-12);
//    EXPECT_LT(e, 1e-12);
//}
//
//// 【测试用例】良态仿射矩阵求逆往返测试：验证正常缩放和平移矩阵求逆的精度
//TEST(Matrix4Inverse, WellConditionedRoundTrip) {         // M4_INV_WELL_01
//    // A = diag(2,3,4,1) + 平移(1,2,3)，det = 24，cond 小
//    Matrix4 A = makeAffine(2.0, 3.0, 4.0, 1.0, 2.0, 3.0);
//    ASSERT_NO_THROW({
//        Matrix4 inv = A.inverse();
//        double e = reconError(A, inv);
//        logMetric("M4_INV_WELL_01", "1", "recon_error", e, e < 1e-12);
//        EXPECT_LT(e, 1e-12);
//        // 手算逆矩阵第3列应为 (-0.5, -2/3, -0.75, 1)
//        EXPECT_NEAR(inv.m[0][3], -0.5, 1e-12);
//        EXPECT_NEAR(inv.m[2][3], -0.75, 1e-12);
//        });
//}
//
//// 【测试用例】奇异矩阵求逆异常测试1：验证输入含全零行的矩阵时能正确抛出异常
//TEST(Matrix4Inverse, SingularZeroRowThrows) {            // M4_SINGULAR_01
//    // 第 2 行全零 → 严格奇异，det 精确为 0
//    const Arr4 v = {
//        { 1.0, 2.0, 0.0, 1.0 },
//        { 0.0, 1.0, 3.0, 2.0 },
//        { 0.0, 0.0, 0.0, 0.0 },
//        { 0.0, 0.0, 0.0, 1.0 }
//    };
//    Matrix4 A = makeMat(v);
//    EXPECT_THROW({ (void)A.inverse(); }, std::runtime_error);
//}
//
//// 【测试用例】奇异矩阵求逆异常测试2：验证输入含重复行的矩阵时能正确抛出异常
//TEST(Matrix4Inverse, SingularDuplicateRowsThrows) {     // M4_SINGULAR_02
//    // 第 0、1 行相同 → 行线性相关，det 的浮点结果约 1e-16 量级
//    const Arr4 v = {
//        { 1.0, 2.0, 3.0, 4.0 },
//        { 1.0, 2.0, 3.0, 4.0 },
//        { 2.0, 1.0, 0.0, 1.0 },
//        { 0.0, 0.0, 0.0, 1.0 }
//    };
//    Matrix4 A = makeMat(v);
//    EXPECT_THROW({ (void)A.inverse(); }, std::runtime_error);
//}
//
//// 【测试用例】奇异矩阵求逆异常测试3：验证输入含线性相关行（A+B=C）的矩阵时能正确抛出异常
//TEST(Matrix4Inverse, SingularDependentRowThrows) {      // M4_SINGULAR_03
//    // 第 2 行 = 第 0 行 + 第 1 行 → 线性相关
//    const Arr4 v = {
//        { 1.0, 0.0, 2.0, 1.0 },
//        { 0.0, 1.0, 1.0, 2.0 },
//        { 1.0, 1.0, 3.0, 3.0 },
//        { 0.0, 0.0, 0.0, 1.0 }
//    };
//    Matrix4 A = makeMat(v);
//    EXPECT_THROW({ (void)A.inverse(); }, std::runtime_error);
//}
//
//// 【测试用例】非法输入异常测试：验证矩阵包含 NaN 等非有限值时能拦截并抛出异常
//TEST(Matrix4Inverse, NonFiniteThrows) {                 // M4_SINGULAR_04
//    // NaN 输入：std::abs(NaN) < 1e-12 为 false，改造前会继续算 1/NaN
//    Matrix4 A = makeAffine(1.0, 1.0, 1.0, 0.0, 0.0, 0.0);
//    A.m[1][1] = std::numeric_limits<double>::quiet_NaN();
//    EXPECT_THROW({ (void)A.inverse(); }, std::runtime_error);
//}
//
//// ==================================================================
//// B. Bezier 升阶不变性
//// ==================================================================
//
//// 【测试用例】二次贝塞尔升阶不变性：验证 3 个控制点升为 4 个后，曲线形状和端点不变
//TEST(BezierElevate, QuadraticInvariance) {              // BEZ_ELEV_01
//    std::vector<Point3> pts = {
//        Point3(0, 0, 0), Point3(1, 2, 0), Point3(3, 0, 1)
//    };
//    BezierCurve before(pts);
//    BezierCurve after(pts);
//    after.elevateDegree();
//
//    EXPECT_EQ(after.controlPoints().size(), 4u);       // n=2 → 3 个变 4 个
//    for (double t : { 0.0, 0.25, 0.5, 0.75, 1.0 }) {
//        double e = maxAbsDiff(before.evaluate(t), after.evaluate(t));
//        logMetric("BEZ_ELEV_01", "1", "max_abs_err", e, e < 1e-12);
//        EXPECT_LT(e, 1e-12) << "t = " << t;
//    }
//    // t=0.5 的闭式解，前后都应为 (1.25, 1.0, 0.25)
//
//    //Point3 mid1 = before.evaluate(0.5);
//    //EXPECT_NEAR(mid1.x, 1.25, 1e-12);
//    //EXPECT_NEAR(mid1.y, 1.0, 1e-12);
//    //EXPECT_NEAR(mid1.z, 0.25, 1e-12);
//
//    Point3 mid2 = after.evaluate(0.5);
//    EXPECT_NEAR(mid2.x, 1.25, 1e-12);
//    EXPECT_NEAR(mid2.y, 1.0, 1e-12);
//    EXPECT_NEAR(mid2.z, 0.25, 1e-12);
//}
//
//// 【测试用例】三次贝塞尔连续两次升阶不变性：验证 4 个控制点升为 6 个后，曲线形状依然不变
//TEST(BezierElevate, CubicDoubleElevationInvariance) {  // BEZ_ELEV_02
//    std::vector<Point3> pts = bezierControlPoints();
//    BezierCurve before(pts);
//    BezierCurve after(pts);
//    after.elevateDegree();
//    after.elevateDegree();
//
//    EXPECT_EQ(after.controlPoints().size(), 6u);       // 4 → 5 → 6
//    for (double t : { 0.0, 0.2, 0.5, 0.8, 1.0 }) {
//        double e = maxAbsDiff(before.evaluate(t), after.evaluate(t));
//        logMetric("BEZ_ELEV_02", "1", "max_abs_err", e, e < 1e-12);
//        EXPECT_LT(e, 1e-12) << "t = " << t;
//    }
//}
//
//// ==================================================================
//// C. Bezier 导数
//// ==================================================================
//
//// 【测试用例】导数数值比对：验证解析求导的结果与中心差分法算出的数值解一致
//TEST(BezierDerivative, MatchesCentralDifference) {      // BEZ_DERIV_01
//    BezierCurve c(bezierControlPoints());
//    const double t = 0.3;
//    Vector3 analytic = c.derivative(t);
//    Vector3 numeric = centralDiff(c, t);
//    double e = maxAbsDiff(analytic, numeric);
//    logMetric("BEZ_DERIV_01", "1", "max_abs_err", e, e < 1e-6);
//    EXPECT_LT(e, 1e-6);
//    // 解析值：3 * [0.49(P1-P0) + 0.42(P2-P1) + 0.09(P3-P2)] = (4.26, 0.69, 1.26)
//    EXPECT_NEAR(analytic.x, 4.26, 1e-9);
//    EXPECT_NEAR(analytic.y, 0.69, 1e-9);
//    EXPECT_NEAR(analytic.z, 1.26, 1e-9);
//}
//
//// 【测试用例】端点导数验证：直接验证贝塞尔曲线在 t=0 和 t=1 处的切线（导数）是否正确
//TEST(BezierDerivative, EndpointValues) {                // BEZ_DERIV_02
//    BezierCurve c(bezierControlPoints());
//    // C'(0) = 3(P1-P0) = (3,6,0)；C'(1) = 3(P3-P2) = (3,3,0)
//    EXPECT_LT(maxAbsDiff(c.derivative(0.0), Vector3(3, 6, 0)), 1e-12);
//    EXPECT_LT(maxAbsDiff(c.derivative(1.0), Vector3(3, 3, 0)), 1e-12);
//}
//
//// 【测试用例】线性曲线导数测试：一次贝塞尔曲线（直线）的导数处处相等，应为一个常量
//TEST(BezierDerivative, LinearCurveConstant) {           // BEZ_DERIV_03
//    // n=1 时导数应为常向量，且导数曲线恰好 1 个控制点
//    std::vector<Point3> pts = { Point3(1, 1, 1), Point3(4, 5, 6) };
//    BezierCurve c(pts);
//    for (double t : { 0.0, 0.3, 0.7, 1.0 }) {
//        double e = maxAbsDiff(c.derivative(t), Vector3(3, 4, 5));
//        logMetric("BEZ_DERIV_03", "1", "max_abs_err", e, e < 1e-12);
//        EXPECT_LT(e, 1e-12) << "t = " << t;
//    }
//}