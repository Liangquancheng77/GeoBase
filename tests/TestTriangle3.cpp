#include <gtest/gtest.h>
#include "../include/GeoBase/Triangle3.h"
#include "../include/GeoBase/Plane.h"


// 直角三角形 (0,0,0), (1,0,0), (0,1,0)，验证面积0.5，重心在(1/3, 1/3, 1/3)
TEST(Triangle3Test, Triangle3Test1) {
	Triangle3 tri(Point3(0, 0, 0), Point3(1, 0, 0), Point3(0, 1, 0));
	EXPECT_DOUBLE_EQ(tri.getArea(), 0.5);
	tri.getBarycentric(Point3(0.5, 0.5, 0)); // 三角形中心点

    Point3 center(1.0 / 3, 1.0 / 3, 0.0);

    Vector3 bary = tri.getBarycentric(center);
    double u = bary.x;
    double v = bary.y;
    double w = bary.z;

    EXPECT_NEAR(std::abs(u - 1.0 / 3), 0.0, EPS_ABS);
    EXPECT_NEAR(std::abs(v - 1.0 / 3), 0.0, EPS_ABS);
    EXPECT_NEAR(std::abs(w - 1.0 / 3), 0.0, EPS_ABS);


    EXPECT_TRUE(tri.contains(center));

}

// 三个顶点分别验证 contains 为 true
TEST(Triangle3Test, ContainsVertices) {
	Triangle3 tri(Point3(0, 0, 0), Point3(1, 0, 0), Point3(0, 1, 0));
	EXPECT_TRUE(tri.contains(Point3(0, 0, 0)));
	EXPECT_TRUE(tri.contains(Point3(1, 0, 0)));
	EXPECT_TRUE(tri.contains(Point3(0, 1, 0)));
}

// 随机三角形和点，用 getBarycentric 算出的坐标加权三个顶点，验证结果与原点重合
TEST(Triangle3Test, BarycentricReconstruction) {

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    for (int i = 0; i < 100; ++i) {
        Point3 v0, v1, v2;
        do {
            v0 = Point3(
                std::rand() % 100 - 50,
                std::rand() % 100 - 50,
                std::rand() % 100 - 50
            );
            v1 = Point3(
                std::rand() % 100 - 50,
                std::rand() % 100 - 50,
                std::rand() % 100 - 50
            );
            v2 = Point3(
                std::rand() % 100 - 50,
                std::rand() % 100 - 50,
                std::rand() % 100 - 50
            );
        } while (Triangle3(v0, v1, v2).getArea() < EPS_ABS);

        Triangle3 tri(v0, v1, v2);
        Plane plane = Plane::fromPoints(v0, v1, v2);
        // 平面内随机点
		Point3 origin = plane.origin;
        Vector3 vec = tri.getBarycentric(plane.origin);
        Vector3 newPoint = v0 * vec.x + v1 * vec.y + v2 * vec.z;
        EXPECT_NEAR(origin.x, newPoint.x, EPS_ABS);
        EXPECT_NEAR(origin.y, newPoint.y, EPS_ABS);
        EXPECT_NEAR(origin.z, newPoint.z, EPS_ABS);

    }
}

// 三点共线的退化三角形，验证面积接近0
//TEST(Triangle3Test, Triangle3Tes2) {
//    Point3 v0(0, 0, 0);
//    Point3 v1(1, 1, 1);
//    Point3 v2(2, 2, 2); // 共线
//    Triangle3 tri(v0, v1, v2);
//    EXPECT_NEAR(tri.getArea(), 0.0, EPS_ABS);
//}

// 测试三点共线，退化三角形，验证 contains 对任何点都返回 false
//TEST(Triangle3Test, ContainsDegenerate) {
//    Point3 v0(0, 0, 0);
//    Point3 v1(1, 1, 1);
//    Point3 v2(2, 2, 2); // 共线
//    Triangle3 tri(v0, v1, v2);
//	EXPECT_FALSE(tri.contains(Point3(0, 0, 0)));
//	EXPECT_FALSE(tri.contains(Point3(1, 0, 0)));
//	EXPECT_FALSE(tri.contains(Point3(0, 2, 0)));
//	EXPECT_FALSE(tri.contains(Point3(2, 2, 2)));
//}



