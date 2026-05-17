#include <gtest/gtest.h>
#include "../include/GeoBase/Ray.h"
#include "../include/GeoBase/Plane.h"
#include "../include/GeoBase/AABB.h"


// 构造函数零向量测试
TEST(RayTest, ZeroDirection) {
	EXPECT_DEATH(Ray(Point3(0, 0, 0), Vector3(0, 0, 0)), "Ray direction vector cannot be zero.");
}

// 射线起点 (0,0,0)，方向 (0,1,0)；平面过 (0,5,0)，法线 (0,-1,0)。验证 t=5，交点 (0,5,0)
TEST(RayTest, IntersectPlane) {
	Ray ray(Point3(0, 0, 0), Vector3(0, 1, 0));
	Plane plane(Vector3(0, -1, 0), Point3(0, 5, 0));
	double t;
	Point3 hitPoint;
	EXPECT_TRUE(plane.intersect(ray, t, hitPoint));
	EXPECT_DOUBLE_EQ(t, 5.0);
	EXPECT_DOUBLE_EQ(hitPoint.x, 0.0);
	EXPECT_DOUBLE_EQ(hitPoint.y, 5.0);
	EXPECT_DOUBLE_EQ(hitPoint.z, 0.0);
}

// 射线起点 (0,0,0)，方向 (1,0,0)；平面过 (0,5,0)，法线 (0,-1,0)。验证没有交点
TEST(RayTest, RayTest1) {
	Ray ray(Point3(0, 0, 0), Vector3(1, 0, 0));
	Plane plane(Vector3(0, -1, 0), Point3(0, 5, 0));
	double t;
	Point3 hitPoint;
	EXPECT_FALSE(plane.intersect(ray, t, hitPoint));
}

// 射线起点在平面上，验证 t≈0，交点就是起点
TEST(RayTest, RayTest2) {

	Ray ray(Point3(1, 5, 9), Vector3(1, 1, 0));
	Plane plane(Vector3(4, -1, 3), Point3(1, 5, 9));
	double t;
	Point3 hitPoint;
	plane.intersect(ray, t, hitPoint);
	EXPECT_NEAR(t, 0.0, EPS_ABS);

	EXPECT_DOUBLE_EQ(hitPoint.x, 1.0);
	EXPECT_DOUBLE_EQ(hitPoint.y, 5.0);
	EXPECT_DOUBLE_EQ(hitPoint.z, 9.0);

}

// 射线背向平面（方向与法线同向，起点在平面下方），验证返回 false（t<0）
TEST(RayTest, RayTest3) {

	Ray ray(Point3(0, 0, 0), Vector3(0, -1, 0));
	Plane plane(Vector3(0, -1, 0), Point3(0, 5, 0));
	double t;
	Point3 hitPoint;
	EXPECT_FALSE(plane.intersect(ray, t, hitPoint));
}

// 随机平面和射线，验证计算出的交点满足 plane.signedDistance(hitPoint) ≈ 0
TEST(RayTest, RayTest4) {
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	for (int i = 0; i < 100; ++i) {
		Point3 planeOrigin(
			std::rand() % 100 - 50,
			std::rand() % 100 - 50,
			std::rand() % 100 - 50
		);
		Vector3 planeNormal(
			std::rand() % 100 - 50,
			std::rand() % 100 - 50,
			std::rand() % 100 - 50
		);
		if (planeNormal.length() < EPS_ABS) {
			continue; // 跳过零向量法线
		}
		Plane plane(planeNormal, planeOrigin);
		Point3 rayOrigin(
			std::rand() % 100 - 50,
			std::rand() % 100 - 50,
			std::rand() % 100 - 50
		);
		Vector3 rayDirection(
			std::rand() % 100 - 50,
			std::rand() % 100 - 50,
			std::rand() % 100 - 50
		);
		if (rayDirection.length() < EPS_ABS) {
			continue; // 跳过零向量方向
		}
		Ray ray(rayOrigin, rayDirection);
		double t;
		Point3 hitPoint;
		if (plane.intersect(ray, t, hitPoint)) {
			double distance = plane.signedDistance(hitPoint);
			EXPECT_NEAR(distance, 0.0, EPS_ABS);
		}
	}
}

// 法线未单位化（用长度2的法线构造平面），验证求交仍然正确
TEST(RayTest, RayTest5) {
	Plane plane(Vector3(0, -2, 0), Point3(0, 5, 0)); // 法线长度为2
	Ray ray(Point3(0, 0, 0), Vector3(0, 1, 0));
	double t;
	Point3 hitPoint;
	EXPECT_TRUE(plane.intersect(ray, t, hitPoint));
	EXPECT_DOUBLE_EQ(t, 5.0);
	EXPECT_DOUBLE_EQ(hitPoint.x, 0.0);
	EXPECT_DOUBLE_EQ(hitPoint.y, 5.0);
	EXPECT_DOUBLE_EQ(hitPoint.z, 0.0);
}

// 射线与AABB求交测试:射线起点 (10,2,2)，方向 (-1,0,0),AABB (-2, -2, -2)-(5, 5, 5)，验证 tMin=5，tMax=12
TEST(RayTest, RayAABBIntersect) {
	Ray ray(Point3(10, 2, 2), Vector3(-1, 0, 0));
	AABB box(Point3(-2, -2, -2), Point3(5, 5, 5));
	double tMin, tMax;
	EXPECT_TRUE(box.intersect(ray, tMin, tMax));
	EXPECT_DOUBLE_EQ(tMin, 5.0);
	EXPECT_DOUBLE_EQ(tMax, 12.0);
}

// 射线与AABB不求交测试:射线起点 (10,6,2)，方向 (-1,0,0),AABB (-2, -2, -2)-(5, 5, 5)
TEST(RayTest, RayTest6) {
	Ray ray(Point3(10, 6, 2), Vector3(-1, 0, 0));
	AABB box(Point3(-2, -2, -2), Point3(5, 5, 5));
	double tMin(0.0), tMax(0.0);
	EXPECT_FALSE(box.intersect(ray, tMin, tMax));
	EXPECT_DOUBLE_EQ(tMin, 0.0); // tMin和tMax不应被修改
	EXPECT_DOUBLE_EQ(tMax, 0.0);
}

// 射线在AABB内部测试:射线起点 (0,0,0)，方向 (1,0,0),AABB (-2, -2, -2)-(5, 5, 5)，验证 tMin=-2，tMax=5
TEST(RayTest, RayTest7) {
	Ray ray(Point3(0, 0, 0), Vector3(1, 0, 0));
	AABB box(Point3(-2, -2, -2), Point3(5, 5, 5));
	double tMin, tMax;
	EXPECT_TRUE(box.intersect(ray, tMin, tMax));
	EXPECT_DOUBLE_EQ(tMin, -2.0);
	EXPECT_DOUBLE_EQ(tMax, 5.0);
}

// 射线起点刚好在AABB表面测试:射线起点 (5,0,0)，方向 (1,0,0),AABB (-2, -2, -2)-(5, 5, 5)，验证 tMin=0，tMax=-7
TEST(RayTest, RayTest8) {
	Ray ray(Point3(5, 0, 0), Vector3(1, 0, 0));
	AABB box(Point3(-2, -2, -2), Point3(5, 5, 5));
	double tMin, tMax;
	EXPECT_TRUE(box.intersect(ray, tMin, tMax));
	EXPECT_DOUBLE_EQ(tMin, -7.0);
	EXPECT_DOUBLE_EQ(tMax, 0.0);
}

// 零尺寸AABB（min==max），验证求交不崩溃
TEST(RayTest, RayTest9) {
	Ray ray(Point3(5, 5, 0), Vector3(0, 0, 1));
	AABB box(Point3(5, 5, 5), Point3(5, 5, 5));
	double tMin, tMax;
	EXPECT_TRUE(box.intersect(ray, tMin, tMax));
}

// 射线方向分量中有两个接近0（几乎平行于某个面），验证鲁棒性
TEST(RayTest, RayTest10) {
	Ray ray(Point3(0, 0, 0), Vector3(1e-10, 1, 1e-10));
	AABB box(Point3(-2, -2, -2), Point3(5, 5, 5));
	double tMin, tMax;
	EXPECT_TRUE(box.intersect(ray, tMin, tMax));
}