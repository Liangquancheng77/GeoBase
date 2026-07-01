#include <gtest/gtest.h>
#include "../include/GeoBase/NURBSCurve.h"
#include "../include/GeoBase/BSplineCurve.h"

// 所有权重=1时，NURBS退化为B样条，验证与BSplineCurve求值一致
TEST(NURBSCurveTest, NURBSCurveTest1) {
	std::vector<Point3> controlPoints({ Point3(0,0,0), Point3(1,2,3),Point3(2,3,4),Point3(3,4,5) });
	std::vector<double> knots({ 0,0,0,0.5,1,1,1 });
	int degree = 2;
	std::vector<double> weights({ 1,1,1,1 });
	NURBSCurve nc(controlPoints, knots, degree, weights);
	BSplineCurve sc(controlPoints, knots, degree);
	EXPECT_EQ(nc.evaluate(0.65), sc.evaluate(0.65));

}

// 某个权重=0：控制点对曲线无影响
TEST(NURBSCurveTest, NURBSCurveTest2) {
	std::vector<Point3> controlPoints1({ Point3(0,0,0), Point3(1,2,3),Point3(2,3,4),Point3(3,4,5) });
	std::vector<Point3> controlPoints2({ Point3(0,0,0), Point3(4,5,7),Point3(2,3,4),Point3(3,4,5) });
	std::vector<double> knots({ 0,0,0,0.5,1,1,1 });
	int degree = 2;
	std::vector<double> weights({ 1,0,1,1 });
	NURBSCurve nc1(controlPoints1, knots, degree, weights);
	NURBSCurve nc2(controlPoints2, knots, degree, weights);
	EXPECT_EQ(nc1.evaluate(0.15), nc2.evaluate(0.15));
}

// 极大权重（如1000）：曲线几乎经过该控制点
TEST(NURBSCurveTest, NURBSCurveTest3) {
	std::vector<Point3> controlPoints({ Point3(0,0,0), Point3(1,2,3),Point3(2,3,4),Point3(3,4,5) });
	std::vector<double> knots({ 0,0,0,0.5,1,1,1 });
	int degree = 2;
	std::vector<double> weights({ 1,1,1000,1 });
	NURBSCurve nc(controlPoints, knots, degree, weights);
	// 选取参数 t = 0.5（位于节点区间中部）
	Point3 p = nc.evaluate(0.5);

	// 权重为 1000 的控制点坐标为 (2,3,4)
	Point3 target(2, 3, 4);
	double distance = (p - target).length();

	// 验证曲线在 t=0.5 处非常接近该控制点（距离小于 0.01）
	EXPECT_LT(distance, 0.01);
}

// 所有权重之和接近0：透视除法不稳定，验证防御
TEST(NURBSCurveTest, NURBSCurveTest4) {
	std::vector<Point3> controlPoints({ Point3(0,0,0), Point3(1,2,3),Point3(2,3,4),Point3(3,4,5) });
	std::vector<double> knots({ 0,0,0,0.5,1,1,1 });
	int degree = 2;
	std::vector<double> weights({ 0,0,0,0 });
	NURBSCurve nc(controlPoints, knots, degree, weights);
	Point3 p = nc.evaluate(0.5);
	int a;
}