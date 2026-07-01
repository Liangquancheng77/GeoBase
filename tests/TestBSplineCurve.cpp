#include <gtest/gtest.h>
#include "../include/GeoBase/BSplineCurve.h"
#include "../include/GeoBase/BezierCurve.h"

// 节点向量[0,0,0,1,1,1]（p=2），三个控制点，验证等价于二次贝塞尔
TEST(BSplineCurveTest, BSplineCurveTest1) {
	std::vector<Point3> controlPoints({ Point3(0,0,0), Point3(1, 2, 3), Point3(2, 3, 4) });
	std::vector<double> knots({ 0,0,0,1,1,1 });
	int degree = 2;
	BSplineCurve bsc(controlPoints, knots, degree);
	Point3 result1 = bsc.evaluate(0.25);

	BezierCurve bc(controlPoints);
	Point3 result2 = bc.evaluate(0.25);
	EXPECT_EQ(result1, result2);
}

// 验证端点插值：evaluate(knots[degree]) == controlPoints[0]，evaluate(knots.back()) == controlPoints.back()
TEST(BSplineCurveTest, BSplineCurveTest2) {
	std::vector<Point3> controlPoints({ Point3(0,0,0), Point3(1, 2, 3), Point3(2, 3, 4), Point3(7, 8, 9) });
	std::vector<double> knots({ 0,0,0,0.5,1,1,1 });
	int degree = 2;
	BSplineCurve bsc(controlPoints, knots, degree);
	EXPECT_EQ(bsc.evaluate(knots[degree]), controlPoints[0]);
	EXPECT_EQ(bsc.evaluate(1.0), controlPoints.back());
}

// 移动一个控制点，验证只有局部曲线发生变化（局部性验证）
TEST(BSplineCurveTest, BSplineCurveTest3) {
	std::vector<Point3> controlPoints1({ Point3(0,0,0), Point3(1, 2, 3), Point3(2, 3, 4), Point3(7, 8, 9) });
	std::vector<double> knots({ 0,0,0,0.5,1,1,1 });
	int degree = 2;
	BSplineCurve bsc1(controlPoints1, knots, degree);

	std::vector<Point3> controlPoints2 = controlPoints1;
	// 移动第一个控制点
	controlPoints2[0] = Point3(20, 20, 20);

	BSplineCurve bsc2(controlPoints2, knots, degree);

	EXPECT_FALSE(bsc1.evaluate(0.25) == bsc2.evaluate(0.25));
	//Point3 r1 = bsc1.evaluate(0.75);
	//Point3 r2 = bsc2.evaluate(0.75);
	EXPECT_TRUE(bsc1.evaluate(0.75) == bsc2.evaluate(0.75));

}

// 均匀节点向量vs非均匀节点向量的求值对比
TEST(BSplineCurveTest, BSplineCurveTest4) {
	// 公共配置：二次B样条，4个控制点
	std::vector<Point3> ctrlPts = {
		Point3(0, 0, 0),
		Point3(1, 2, 0),
		Point3(3, 2, 0),
		Point3(7, 8, 0)
	};
	int degree = 2;

	// 1. 均匀节点向量：内部节点等距，两段区间长度都是 0.5
	std::vector<double> uniformKnots = { 0, 0, 0, 0.5, 1, 1, 1 };
	BSplineCurve uniformCurve(ctrlPts, uniformKnots, degree);

	// 2. 非均匀节点向量：内部节点左移，第一段长 0.2，第二段长 0.8，间距不等
	std::vector<double> nonUniformKnots = { 0, 0, 0, 0.2, 1, 1, 1 };
	BSplineCurve nonUniformCurve(ctrlPts, nonUniformKnots, degree);

	// 验证1：端点处两者完全一致（夹紧特性，都精确经过首尾控制点）
	EXPECT_TRUE(uniformCurve.evaluate(0.0) == nonUniformCurve.evaluate(0.0));
	EXPECT_TRUE(uniformCurve.evaluate(1.0) == nonUniformCurve.evaluate(1.0));

	// 验证2：中间相同参数 t，求值结果不同（节点分布改变了参数与曲线的对应关系）
	// t=0.3：均匀曲线里还在第一段区间，非均匀曲线里已经进入第二段区间
	double tMid = 0.3;
	Point3 ptUniform = uniformCurve.evaluate(tMid);
	Point3 ptNonUniform = nonUniformCurve.evaluate(tMid);
	EXPECT_FALSE(ptUniform == ptNonUniform);
}


// ========================节点插入========================

// 插入一个节点后，新旧曲线在相同t处的点误差 < EPS
TEST(BSplineCurveTest, BSplineCurveTest5) {
	std::vector<Point3> controlPoints({ Point3(0,0,0), Point3(1, 2, 3), Point3(2, 3, 4), Point3(7, 8, 9) });
	std::vector<double> knots({ 0,0,0,0.5,1,1,1 });
	int degree = 2;
	BSplineCurve bsc(controlPoints, knots, degree);

	Point3 result1 = bsc.evaluate(0.25);

	bsc.insertKnot(0.7);

	Point3 result2 = bsc.evaluate(0.25);

	EXPECT_TRUE(result1 == result2);

}

// 连续插入多个节点，控制点数量增加，曲线不变
TEST(BSplineCurveTest, BSplineCurveTest6) {
	std::vector<Point3> controlPoints({ Point3(0,0,0), Point3(1, 2, 3), Point3(2, 3, 4), Point3(7, 8, 9) });
	std::vector<double> knots({ 0,0,0,0.5,1,1,1 });
	int degree = 2;
	BSplineCurve bsc(controlPoints, knots, degree);

	Point3 result1 = bsc.evaluate(0.25);

	bsc.insertKnot(0.5);
	bsc.insertKnot(0.7);
	bsc.insertKnot(0.7);
	bsc.insertKnot(0.17);
	bsc.insertKnot(0.97);

	Point3 result2 = bsc.evaluate(0.25);

	EXPECT_TRUE(result1 == result2);
}

// 重节点插入：在已有节点处再插入，验证多重性增加，但曲线仍不变
TEST(BSplineCurveTest, BSplineCurveTest7) {
	std::vector<Point3> controlPoints({ Point3(0,0,0), Point3(1, 2, 3), Point3(2, 3, 4), Point3(7, 8, 9) });
	std::vector<double> knots({ 0,0,0,0.5,1,1,1 });
	int degree = 2;
	BSplineCurve bsc(controlPoints, knots, degree);

	Point3 result1 = bsc.evaluate(0.25);

	bsc.insertKnot(0.5);
	bsc.insertKnot(0.5);
	bsc.insertKnot(0.5);
	bsc.insertKnot(0.5);
	bsc.insertKnot(0.5);
	bsc.insertKnot(0.5);

	Point3 result2 = bsc.evaluate(0.25);

	EXPECT_TRUE(result1 == result2);
}




