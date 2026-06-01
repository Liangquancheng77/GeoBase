#include<gtest/gtest.h>
#include "../include/GeoBase/QEM.h"

// 从平面(0,1,0)过原点构建Q矩阵，验证evaluate(Point3(0,1,0))接近1.0，evaluate(Point3(0,0,0))接近0
TEST(QEMTest, QEMTest1) {
	Quadric q(Vector3(0, 1, 0), Point3(0, 0, 0));
	EXPECT_EQ(q.evaluate(Point3(0, 1, 0)), 1.0);
	EXPECT_EQ(q.evaluate(Point3(0, 0, 0)), 0);
}

// 两个Q矩阵相加，验证矩阵元素正确
TEST(QEMTest, QEMTest2) {
	Quadric q1(Vector3(0, 1, 0), Point3(0, 0, 0));
	Quadric q2(Vector3(1, 0, 0), Point3(0, 0, 0));
	Quadric q3 = q1 + q2;
	EXPECT_EQ(q3.m[0], 1.0);
	EXPECT_EQ(q3.m[1], 0);
	EXPECT_EQ(q3.m[4], 1.0);
	EXPECT_EQ(q3.m[5], 0);
	EXPECT_EQ(q3.m[7], 0);
	EXPECT_EQ(q3.m[9], 0);
}

// 对一组三个互相垂直的平面构建Q，验证最优位置在三个平面的交点
TEST(QEMTest, QEMTest3) {
	Quadric q1(Vector3(0, 1, 0), Point3(0, 1, 0));
	Quadric q2(Vector3(1, 0, 0), Point3(1, 0, 0));
	Quadric q3(Vector3(0, 0, 1), Point3(0, 0, 1));

	Quadric q4 = q1 + q2 + q3;

	Point3 p;
	EXPECT_TRUE(q4.solveOptimal(p));
	EXPECT_EQ(p.x, 1.0);
	EXPECT_EQ(p.y, 1.0);
	EXPECT_EQ(p.z, 1.0);
}

// 对随机平面构建Q，随机采样点，验证evaluate结果≥0（二次型半正定性）
TEST(QEMTest, QEMTest4) {
	for (int i = 0; i < 100; i++)
	{
		Quadric q(Vector3(randomDouble(0, 1), randomDouble(0, 1), randomDouble(0, 1)).normalized(), Point3(randomDouble(0, 1), randomDouble(0, 1), randomDouble(0, 1)));
		const Point3 p(randomDouble(0, 1), randomDouble(0, 1), randomDouble(0, 1));
		EXPECT_TRUE(q.evaluate(p) > 0);
	}
}

// 构造一个奇异的Q矩阵（如一个平面），验证solveOptimal返回false
TEST(QEMTest, QEMTest5) {
	Quadric q(Vector3(0, 1, 0), Point3(0, 0, 0));
	Point3 result;
	EXPECT_FALSE(q.solveOptimal(result));
}

// 零Q矩阵（所有元素为0），验证evaluate对任意点返回0
TEST(QEMTest, QEMTest6) {
	Quadric q;
	Point3 p1(1, 2, 3);
	Point3 p2(1, 5, 8);
	Point3 p3(2, 12, 13);
	Point3 p4(0, 0, 0);
	EXPECT_EQ(q.evaluate(p1), 0);
	EXPECT_EQ(q.evaluate(p2), 0);
	EXPECT_EQ(q.evaluate(p3), 0);
	EXPECT_EQ(q.evaluate(p4), 0);
}

// 测试：极大/极小数值平面，浮点数值退化场景
TEST(QEMTest, QEMTest7) {
	// 极大值平面
	double large = 1e6;
	Quadric q_large(Vector3(1, 0, 0), Point3(large, 0, 0));
	// 极小值平面
	double small = 1e-6;
	Quadric q_small(Vector3(0, 1, 0), Point3(0, small, 0));

	// 验证：不崩溃、误差计算正常
	EXPECT_NO_THROW(q_large.evaluate(Point3(large, 0, 0)));
	Point3 dummy;
	EXPECT_NO_THROW(q_small.solveOptimal(dummy));
}

