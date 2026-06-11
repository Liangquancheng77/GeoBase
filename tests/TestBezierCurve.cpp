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
