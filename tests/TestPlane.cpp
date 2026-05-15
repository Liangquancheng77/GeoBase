#include <gtest/gtest.h>
#include "../include/GeoBase/Plane.h"

// 测试点到平面的有符号距离
TEST(PlaneTest, SignedDistance) {
	Point3 target(-1, -1, -1);
	Point3 planeOrigin(0, 0, 0);
	Vector3 planeNormal(0, 1, 0); // 平面法向量指向y轴正方向
	Plane plane(planeNormal, planeOrigin);
	double distance = plane.signedDistance(target);
	EXPECT_DOUBLE_EQ(distance, -1.0);
}

// 测试点在平面上的投影
TEST(PlaneTest, ProjectPoint) {
	Plane plane({ 0, 1, 0 }, { 0, 0, 0 }); // 平面：y=0
	Point3 p(4, 5, 6);
	Point3 projected = plane.projectPoint(p);
	EXPECT_DOUBLE_EQ(projected.x, 4.0);
	EXPECT_DOUBLE_EQ(projected.y, 0.0);
	EXPECT_DOUBLE_EQ(projected.z, 6.0);
}

// 测试任意一点的投影到任意平面的距离接近0
TEST(PlaneTest, PlaneTest1) {
	Plane plane({ 1, 3, 7 }, { 1, 1, 0 }); // 平面：y=0
	Point3 point(2, 3, 4);
	Point3 projected = plane.projectPoint(point);
	double distance = plane.signedDistance(projected);
	EXPECT_NEAR(distance, 0.0, EPS_ABS);
}

// 测试点在平面上的包含关系
TEST(PlaneTest, ContainsPoint) {
	Plane plane({ 0, 1, 0 }, { 0, 0, 0 }); // 平面：y=0
	Point3 pointOnPlane(1, 0, 1);
	Point3 pointAbove(0, 2, 0);
	Point3 pointBelow(0, -3, 0);
	EXPECT_TRUE(plane.contains(pointOnPlane));
	EXPECT_FALSE(plane.contains(pointAbove));
	EXPECT_FALSE(plane.contains(pointBelow));
}

// 测试零向量法线的平面构造函数应该断言失败
TEST(PlaneTest, ZeroNormalVector) {
	EXPECT_DEATH(Plane({ 0, 0, 0 }, { 0, 0, 0 }), "Plane normal vector cannot be zero.");
}

// 测试大数据量的点到平面的距离计算
TEST(PlaneTest, LargeDataSignedDistance) {
	Plane plane({ 0, 1, 0 }, { 0, 0, 0 }); // 平面：y=0
	const int numPoints = 10000;
	std::vector<Point3> points(numPoints);
	for (int i = 0; i < numPoints; ++i) {
		points[i] = Point3(i % 100, i % 10, i % 5);
	}
	for (const auto& point : points) {
		double distance = plane.signedDistance(point);
		EXPECT_NEAR(distance, point.y, EPS_ABS);
	}
}

// 测试极大小值的点到平面的距离计算
TEST(PlaneTest, ExtremeValuesSignedDistance) {
	Plane plane({ 0, 1, 0 }, { 0, 0, 0 }); // 平面：y=0
	Point3 pointMax(std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
	Point3 pointMin(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest());
	EXPECT_NEAR(plane.signedDistance(pointMax), pointMax.y, EPS_ABS);
	EXPECT_NEAR(plane.signedDistance(pointMin), pointMin.y, EPS_ABS);
}

// 随机生成平面和一个外部点，验证 (p - projectPoint(p)).normalized() 与平面法线平行（点积绝对值接近 1）。
TEST(PlaneTest, RandomPlaneProjection) {
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	for (int i = 0; i < 100; ++i) {
		Vector3 normal(std::rand() % 100 - 50, std::rand() % 100 - 50, std::rand() % 100 - 50);
		if (normal.length() < 1e-9) continue; // 避免零向量
		Point3 origin(std::rand() % 100 - 50, std::rand() % 100 - 50, std::rand() % 100 - 50);
		Plane plane(normal, origin);
		Point3 point(std::rand() % 100 - 50, std::rand() % 100 - 50, std::rand() % 100 - 50);
		Point3 projected = plane.projectPoint(point);
		Vector3 toProjected = (point - projected).normalized();
		double dotProduct = toProjected.dot(plane.normal);
		EXPECT_NEAR(std::abs(dotProduct), 1.0, EPS_ABS);
	}
}