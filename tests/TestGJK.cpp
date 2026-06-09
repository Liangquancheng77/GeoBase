#include <gtest/gtest.h>
#include "../include/GeoBase/GJK.h"
#include "../include/GeoBase/MeshObject.h"
#include <chrono>
#include <fstream>
#include <random>
#include <iomanip>
using namespace std;
//
////----------------------------------------2D------------------------------------------
//
//// 两个正方形相交、分离、刚好接触
//TEST(GJKTest, GJKTest1) {
//	std::vector<Point3> shapeA;
//	shapeA.push_back(Point3(1, 1, 0));
//	shapeA.push_back(Point3(0, 2, 0));
//	shapeA.push_back(Point3(2, 2, 0));
//	shapeA.push_back(Point3(1, 3, 0));
//	std::vector<Point3> shapeB;
//	shapeB.push_back(Point3(2.9, 1, 0));
//	shapeB.push_back(Point3(1.9, 2, 0));
//	shapeB.push_back(Point3(3.9, 2, 0));
//	shapeB.push_back(Point3(2.9, 3, 0));
//	std::vector<Point3> shapeC;
//	shapeC.push_back(Point3(3, 1, 0));
//	shapeC.push_back(Point3(2, 2, 0));
//	shapeC.push_back(Point3(4, 2, 0));
//	shapeC.push_back(Point3(3, 3, 0));
//	std::vector<Point3> shapeD;
//	shapeD.push_back(Point3(3.001, 1, 0));
//	shapeD.push_back(Point3(2.001, 2, 0));
//	shapeD.push_back(Point3(4.001, 2, 0));
//	shapeD.push_back(Point3(3.001, 3, 0));
//
//	EXPECT_TRUE(GJK::intersect2D(shapeA, shapeB));
//	EXPECT_TRUE(GJK::intersect2D(shapeA, shapeC));
//	EXPECT_FALSE(GJK::intersect2D(shapeA, shapeD));
//
//}
//
//// 一个形状完全包含另一个
//TEST(GJKTest, GJKTest2) {
//	std::vector<Point3> shapeA;
//	shapeA.push_back(Point3(0, 0, 0));
//	shapeA.push_back(Point3(0, 2, 0));
//	shapeA.push_back(Point3(2, 0, 0));
//	std::vector<Point3> shapeB;
//	shapeB.push_back(Point3(0.1, 0, 0));
//	shapeB.push_back(Point3(0, 1, 0));
//	shapeB.push_back(Point3(1, 0, 0));
//	EXPECT_TRUE(GJK::intersect2D(shapeA, shapeB));
//}
//
//// 退化：形状是点、点
//TEST(GJKTest, GJKTest3) {
//	std::vector<Point3> shapeA;
//	shapeA.push_back(Point3(1, 2, 0));
//	std::vector<Point3> shapeB;
//	shapeB.push_back(Point3(1, 2, 0));
//	EXPECT_TRUE(GJK::intersect2D(shapeA, shapeB));
//}
//
//// 退化：形状是点、线段
//TEST(GJKTest, GJKTest4) {
//	std::vector<Point3> shapeA;
//	shapeA.push_back(Point3(1, 2, 0));
//	std::vector<Point3> shapeB;
//	shapeB.push_back(Point3(1, 2, 0));
//	shapeB.push_back(Point3(2, 4, 0));
//	EXPECT_TRUE(GJK::intersect2D(shapeA, shapeB));
//}
//
//// 退化：形状是线段、线段
//TEST(GJKTest, GJKTest5) {
//	std::vector<Point3> shapeA;
//	shapeA.push_back(Point3(0, 0, 0));
//	shapeA.push_back(Point3(2, 2, 0));
//	std::vector<Point3> shapeB;
//	shapeB.push_back(Point3(0, 2, 0));
//	shapeB.push_back(Point3(2, 0, 0));
//	EXPECT_TRUE(GJK::intersect2D(shapeA, shapeB));
//}
//
//TEST(GJKTest, GJKTest6) {
//	std::vector<Point3> shapeA;
//	shapeA.push_back(Point3(1, 1, 0));
//	shapeA.push_back(Point3(4, 0, 0));
//	shapeA.push_back(Point3(0, 2, 0));
//	std::vector<Point3> shapeB;
//	shapeB.push_back(Point3(0, 0, 0));
//	EXPECT_FALSE(GJK::intersect2D(shapeA, shapeB));
//}
//
//TEST(GJKTest, GJKTest7) {
//	std::vector<Point3> shapeA;
//	shapeA.push_back(Point3(0, 2, 0)); // 只改了顶点顺序！
//	shapeA.push_back(Point3(1, 1, 0));
//	shapeA.push_back(Point3(4, 0, 0));
//	std::vector<Point3> shapeB;
//	shapeB.push_back(Point3(0, 0, 0));
//	EXPECT_FALSE(GJK::intersect2D(shapeA, shapeB)); // 正确结果：不碰撞
//}
//
////----------------------------------------3D------------------------------------------
//
//// 两个正方形相交、分离、刚好接触
//TEST(GJKTest, GJKTest8) {
//	std::vector<Point3> shapeA;
//	shapeA.push_back(Point3(1, 1, 0));
//	shapeA.push_back(Point3(0, 2, 0));
//	shapeA.push_back(Point3(2, 2, 0));
//	shapeA.push_back(Point3(1, 3, 0));
//	std::vector<Point3> shapeB;
//	shapeB.push_back(Point3(2.9, 1, 0));
//	shapeB.push_back(Point3(1.9, 2, 0));
//	shapeB.push_back(Point3(3.9, 2, 0));
//	shapeB.push_back(Point3(2.9, 3, 0));
//	std::vector<Point3> shapeC;
//	shapeC.push_back(Point3(3, 1, 0));
//	shapeC.push_back(Point3(2, 2, 0));
//	shapeC.push_back(Point3(4, 2, 0));
//	shapeC.push_back(Point3(3, 3, 0));
//	std::vector<Point3> shapeD;
//	shapeD.push_back(Point3(3.001, 1, 0));
//	shapeD.push_back(Point3(2.001, 2, 0));
//	shapeD.push_back(Point3(4.001, 2, 0));
//	shapeD.push_back(Point3(3.001, 3, 0));
//	std::vector<Point3> simplex;
//	EXPECT_TRUE(GJK::intersect3D(shapeA, shapeB, simplex));
//	EXPECT_TRUE(GJK::intersect3D(shapeA, shapeC, simplex));
//	EXPECT_FALSE(GJK::intersect3D(shapeA, shapeD, simplex));
//
//}
//
//// 一个形状完全包含另一个
//TEST(GJKTest, GJKTest9) {
//	std::vector<Point3> shapeA;
//	shapeA.push_back(Point3(0, 0, 0));
//	shapeA.push_back(Point3(0, 2, 0));
//	shapeA.push_back(Point3(2, 0, 0));
//	std::vector<Point3> shapeB;
//	shapeB.push_back(Point3(0.1, 0, 0));
//	shapeB.push_back(Point3(0, 1, 0));
//	shapeB.push_back(Point3(1, 0, 0));
//	std::vector<Point3> simplex;
//	EXPECT_TRUE(GJK::intersect3D(shapeA, shapeB, simplex));
//}
//
//// 退化：形状是点、点
//TEST(GJKTest, GJKTest10) {
//	std::vector<Point3> shapeA;
//	shapeA.push_back(Point3(1, 2, 0));
//	std::vector<Point3> shapeB;
//	shapeB.push_back(Point3(1, 2, 0));
//	std::vector<Point3> simplex;
//	EXPECT_TRUE(GJK::intersect3D(shapeA, shapeB, simplex));
//}
//
//// 退化：形状是点、线段
//TEST(GJKTest, GJKTest11) {
//	std::vector<Point3> shapeA;
//	shapeA.push_back(Point3(1, 2, 0));
//	std::vector<Point3> shapeB;
//	shapeB.push_back(Point3(1, 2, 0));
//	shapeB.push_back(Point3(2, 4, 0));
//	std::vector<Point3> simplex;
//	EXPECT_TRUE(GJK::intersect3D(shapeA, shapeB, simplex));
//}
//
//// 退化：形状是线段、线段
//TEST(GJKTest, GJKTest12) {
//	std::vector<Point3> shapeA;
//	shapeA.push_back(Point3(0, 0, 0));
//	shapeA.push_back(Point3(2, 2, 0));
//	std::vector<Point3> shapeB;
//	shapeB.push_back(Point3(0, 2, 0));
//	shapeB.push_back(Point3(2, 0, 0));
//	std::vector<Point3> simplex;
//	EXPECT_TRUE(GJK::intersect3D(shapeA, shapeB, simplex));
//}
//
//TEST(GJKTest, GJKTest13) {
//	std::vector<Point3> shapeA;
//	shapeA.push_back(Point3(1, 1, 0));
//	shapeA.push_back(Point3(4, 0, 0));
//	shapeA.push_back(Point3(0, 2, 0));
//	std::vector<Point3> shapeB;
//	shapeB.push_back(Point3(0, 0, 0));
//	std::vector<Point3> simplex;
//	EXPECT_FALSE(GJK::intersect3D(shapeA, shapeB, simplex));
//}
//
//TEST(GJKTest, GJKTest14) {
//	std::vector<Point3> shapeA;
//	shapeA.push_back(Point3(0, 2, 0)); // 只改了顶点顺序！
//	shapeA.push_back(Point3(1, 1, 0));
//	shapeA.push_back(Point3(4, 0, 0));
//	std::vector<Point3> shapeB;
//	shapeB.push_back(Point3(0, 0, 0));
//	std::vector<Point3> simplex;
//	EXPECT_FALSE(GJK::intersect3D(shapeA, shapeB, simplex)); // 正确结果：不碰撞
//}
//
// 两个正方体相交、分离、刚好接触
TEST(GJKTest, GJKTest15) {
	std::vector<Point3> shapeA;
	shapeA.push_back(Point3(-1, -1, -1));
	shapeA.push_back(Point3(1, -1, -1));
	shapeA.push_back(Point3(1, 1, -1));
	shapeA.push_back(Point3(-1, 1, -1));
	shapeA.push_back(Point3(-1, -1, 1));
	shapeA.push_back(Point3(1, -1, 1));
	shapeA.push_back(Point3(1, 1, 1));
	shapeA.push_back(Point3(-1, 1, 1));

	std::vector<Point3> shapeB;
	// 所有x坐标都增加1.9，确保两个立方体相交但不完全重叠
	shapeB.push_back(Point3(0.9, -1, -1));
	shapeB.push_back(Point3(2.9, -1, -1));
	shapeB.push_back(Point3(2.9, 1, -1));
	shapeB.push_back(Point3(0.9, 1, -1));
	shapeB.push_back(Point3(0.9, -1, 1));
	shapeB.push_back(Point3(2.9, -1, 1));
	shapeB.push_back(Point3(2.9, 1, 1));
	shapeB.push_back(Point3(0.9, 1, 1));

	std::vector<Point3> shapeC;
	// 所有x坐标都增加2.1，确保两个立方体分离
	shapeC.push_back(Point3(1.1, -1, -1));
	shapeC.push_back(Point3(3.1, -1, -1));
	shapeC.push_back(Point3(3.1, 1, -1));
	shapeC.push_back(Point3(1.1, 1, -1));
	shapeC.push_back(Point3(1.1, -1, 1));
	shapeC.push_back(Point3(3.1, -1, 1));
	shapeC.push_back(Point3(3.1, 1, 1));
	shapeC.push_back(Point3(1.1, 1, 1));

	std::vector<Point3> shapeD;
	// 所有x坐标都增加2.0,确保两个立方体刚好接触
	shapeD.push_back(Point3(1.0, -1, -1));
	shapeD.push_back(Point3(3.0, -1, -1));
	shapeD.push_back(Point3(3.0, 1, -1));
	shapeD.push_back(Point3(1.0, 1, -1));
	shapeD.push_back(Point3(1.0, -1, 1));
	shapeD.push_back(Point3(3.0, -1, 1));
	shapeD.push_back(Point3(3.0, 1, 1));
	shapeD.push_back(Point3(1.0, 1, 1));

	std::vector<Point3> shapeE;
	// 所有x坐标都增加1.0,y坐标都增加1.0,z坐标都增加1.0,正常相交
	shapeE.push_back(Point3(0.0, 0, 0));
	shapeE.push_back(Point3(2.0, 0, 0));
	shapeE.push_back(Point3(2.0, 2, 0));
	shapeE.push_back(Point3(0.0, 2, 0));
	shapeE.push_back(Point3(0.0, 0, 2));
	shapeE.push_back(Point3(2.0, 0, 2));
	shapeE.push_back(Point3(2.0, 2, 2));
	shapeE.push_back(Point3(0.0, 2, 2));

	std::vector<GJK::Vertex> simplex;

	//EXPECT_TRUE(GJK::intersect3D(shapeA, shapeB, simplex));
	//EXPECT_FALSE(GJK::intersect3D(shapeA, shapeC, simplex));
	EXPECT_TRUE(GJK::intersect3D(shapeA, shapeD, simplex));
	//EXPECT_TRUE(GJK::intersect3D(shapeA, shapeE, simplex));
	GJK::expandToTetrahedron(simplex, shapeA, shapeD);
	//GJK::expandToTetrahedron(simplex, shapeA, shapeB);
	GJK::PenetrationInfo info;
	GJK::computePenetration(shapeA, shapeD, simplex, info);
	//GJK::computePenetration(shapeA, shapeE, simplex, info);
	cout << "--------------------------------------------" << endl;
}

// 退化：形状是点、正方体
TEST(GJKTest, GJKTest16) {
	std::vector<Point3> shapeA;
	shapeA.push_back(Point3(-1, -1, -1));
	shapeA.push_back(Point3(1, -1, -1));
	shapeA.push_back(Point3(1, 1, -1));
	shapeA.push_back(Point3(-1, 1, -1));
	shapeA.push_back(Point3(-1, -1, 1));
	shapeA.push_back(Point3(1, -1, 1));
	shapeA.push_back(Point3(1, 1, 1));
	shapeA.push_back(Point3(-1, 1, 1));
	std::vector<Point3> shapeB;
	shapeB.push_back(Point3(1.01, 1.01, -1.01));
	std::vector<GJK::Vertex> simplex;
	//EXPECT_TRUE(GJK::intersect3D(shapeA, shapeB, simplex));
	EXPECT_FALSE(GJK::intersect3D(shapeA, shapeB, simplex));
	GJK::expandToTetrahedron(simplex, shapeA, shapeB);
	cout << "--------------------------------------------" << endl;
}
//
//TEST(GJKTest, LineSegmentOverlapCausesInfiniteLoop) {
//	// 两个共线且重叠的线段
//	std::vector<Point3> shapeA = { Point3(0,0,0), Point3(2,0,0) };
//	std::vector<Point3> shapeB = { Point3(1,0,0), Point3(3,0,0) };
//
//	// 它们明显相交，GJK 应该返回 true，但实际会陷入死循环或崩溃
//	EXPECT_TRUE(GJK::intersect2D(shapeA, shapeB));
//}
//
//TEST(GJKTest, GJK_Fails_TriangleOutside) {
//	// shapeA 是原点
//	std::vector<Point3> shapeA = { Point3(0, 0, 0) };
//	// shapeB 是一个三角形，它的 Minkowski 差（即 -shapeB）是一个三角形，
//	// 原点在该三角形外部，且位于未检查的 BC 边外侧。
//	std::vector<Point3> shapeB = {
//		Point3(0, 3, 0),   // 对应 Minkowski 差中的 A(0, -3)
//		Point3(2, 1, 0),   // B(-2, -1)
//		Point3(-2, 1, 0)   // C(2, -1)
//	};
//	// 此时 Minkowski 差三角形顶点（以 shapeA - shapeB）为：
//	// A(0, -3), B(-2, -1), C(2, -1)
//	// 原点 (0,0) 明显在 BC 边的上方，位于三角形外部。
//	// 期望结果：不相交
//	EXPECT_FALSE(GJK::intersect2D(shapeA, shapeB));
//}
//
//TEST(GJKTest, Fails_BC_Outside) {
//	// shapeA 是一个三角形，shapeB 是原点 (0,0)
//	// Minkowski 差 = shapeA 本身（因为 shapeB 为零点）
//	std::vector<Point3> shapeA;
//	shapeA.push_back(Point3(0, 4, 0));   // A
//	shapeA.push_back(Point3(-3, 1, 0));  // B
//	shapeA.push_back(Point3(3, 0.5, 0)); // C
//
//	std::vector<Point3> shapeB;
//	shapeB.push_back(Point3(0, 0, 0));
//	  
//	// 原点 (0,0) 在三角形外部，预期不相交
//	EXPECT_FALSE(GJK::intersect2D(shapeA, shapeB));
//}

// ================================== 退化测试 =========================================

// ---------- 辅助函数：生成单位球面上的点 ----------
std::vector<Point3> generateSpherePoints(double radius, int subdivisions = 2) {
	// 从一个基础二十面体开始，然后细分，返回所有顶点（去重）
	// 为简化，这里直接返回二十面体顶点（精度已经足够测试）
	const double X = 0.525731112119133606;
	const double Z = 0.850650808352039932;
	std::vector<Point3> base = {
		{-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
		{0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
		{Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
	};
	for (auto& p : base) p = p * radius;
	return base;
}

// 两个相同的球体，验证GJK检测到相交，EPA返回合理穿透深度
TEST(GJKTest, GJKTest17) {
	const double radius = 1.0;
	const double expectedPenetration = 2.0 * radius; // 完全重叠时穿透深度

	// 生成两个相同的球体点云（中心都在原点）
	std::vector<Point3> sphereA = generateSpherePoints(radius);
	std::vector<Point3> sphereB = sphereA; // 完全相同，中心重合
	std::vector<GJK::Vertex> simplex;
	bool gjkResult = GJK::intersect3D(sphereA, sphereB, simplex);
	ASSERT_TRUE(gjkResult) << "GJK should detect intersection for overlapping spheres.";
	GJK::expandToTetrahedron(simplex, sphereA, sphereB);
	GJK::PenetrationInfo info;
	GJK::computePenetration(sphereA, sphereB, simplex, info);
	cout << "--------------------------------------------" << endl;
}

// 两个相同的正方体，验证GJK检测到相交，EPA返回合理穿透深度
TEST(GJKTest, GJKTest18) {
	std::vector<Point3> shapeA;
	shapeA.push_back(Point3(-1, -1, -1));
	shapeA.push_back(Point3(1, -1, -1));
	shapeA.push_back(Point3(1, 1, -1));
	shapeA.push_back(Point3(-1, 1, -1));
	shapeA.push_back(Point3(-1, -1, 1));
	shapeA.push_back(Point3(1, -1, 1));
	shapeA.push_back(Point3(1, 1, 1));
	shapeA.push_back(Point3(-1, 1, 1));
	std::vector<Point3> shapeB = shapeA;
	std::vector<GJK::Vertex> simplex;
	GJK::intersect3D(shapeA, shapeB, simplex);
	GJK::expandToTetrahedron(simplex, shapeA, shapeB);
	GJK::PenetrationInfo info;
	GJK::computePenetration(shapeA, shapeB, simplex, info);
	cout << "--------------------------------------------" << endl;
}

// 刚好接触：两个共享一个顶点的立方体，验证边界情况处理
TEST(GJKTest, GJKTest19) {
	std::vector<Point3> shapeA;
	shapeA.push_back(Point3(-1, -1, -1));
	shapeA.push_back(Point3(1, -1, -1));
	shapeA.push_back(Point3(1, 1, -1));
	shapeA.push_back(Point3(-1, 1, -1));
	shapeA.push_back(Point3(-1, -1, 1));
	shapeA.push_back(Point3(1, -1, 1));
	shapeA.push_back(Point3(1, 1, 1));
	shapeA.push_back(Point3(-1, 1, 1));

	std::vector<Point3> shapeB;
	shapeB.push_back(Point3(1, 1, 1));
	shapeB.push_back(Point3(3, 1, 1));
	shapeB.push_back(Point3(3, 3, 1));
	shapeB.push_back(Point3(1, 3, 1));
	shapeB.push_back(Point3(1, 1, 3));
	shapeB.push_back(Point3(3, 1, 3));
	shapeB.push_back(Point3(3, 3, 3));
	shapeB.push_back(Point3(1, 3, 3));
	std::vector<GJK::Vertex> simplex;
	EXPECT_TRUE(GJK::intersect3D(shapeA, shapeB, simplex));
	GJK::expandToTetrahedron(simplex, shapeA, shapeB);
	GJK::PenetrationInfo info;
	GJK::computePenetration(shapeA, shapeB, simplex, info);
	cout << "--------------------------------------------" << endl;

}

// 极近距离分离：两个分离的正方体，距离为1e-6，验证GJK正确判断不相交
TEST(GJKTest, GJKTest20) {
	std::vector<Point3> shapeA;
	shapeA.push_back(Point3(-1, -1, -1));
	shapeA.push_back(Point3(1, -1, -1));
	shapeA.push_back(Point3(1, 1, -1));
	shapeA.push_back(Point3(-1, 1, -1));
	shapeA.push_back(Point3(-1, -1, 1));
	shapeA.push_back(Point3(1, -1, 1));
	shapeA.push_back(Point3(1, 1, 1));
	shapeA.push_back(Point3(-1, 1, 1));

	std::vector<Point3> shapeB;
	shapeB.push_back(Point3(1+ EPS_REL, -1, -1));
	shapeB.push_back(Point3(3+ EPS_REL, -1, -1));
	shapeB.push_back(Point3(3+ EPS_REL, 1, -1));
	shapeB.push_back(Point3(1+ EPS_REL, 1, -1));
	shapeB.push_back(Point3(1+ EPS_REL, -1, 1));
	shapeB.push_back(Point3(3+ EPS_REL, -1, 1));
	shapeB.push_back(Point3(3+ EPS_REL, 1, 1));
	shapeB.push_back(Point3(1+ EPS_REL, 1, 1));
	std::vector<GJK::Vertex> simplex;
	EXPECT_FALSE(GJK::intersect3D(shapeA, shapeB, simplex));
	cout << "--------------------------------------------" << endl;

}

// 接近0的距离分离：两个分离的正方体，距离为1e-10，验证GJK正确判断相交
TEST(GJKTest, GJKTest21) {
	std::vector<Point3> shapeA;
	shapeA.push_back(Point3(-1, -1, -1));
	shapeA.push_back(Point3(1, -1, -1));
	shapeA.push_back(Point3(1, 1, -1));
	shapeA.push_back(Point3(-1, 1, -1));
	shapeA.push_back(Point3(-1, -1, 1));
	shapeA.push_back(Point3(1, -1, 1));
	shapeA.push_back(Point3(1, 1, 1));
	shapeA.push_back(Point3(-1, 1, 1));

	std::vector<Point3> shapeB;
	shapeB.push_back(Point3(1 + 1e-10, -1, -1));
	shapeB.push_back(Point3(3 + 1e-10, -1, -1));
	shapeB.push_back(Point3(3 + 1e-10, 1, -1));
	shapeB.push_back(Point3(1 + 1e-10, 1, -1));
	shapeB.push_back(Point3(1 + 1e-10, -1, 1));
	shapeB.push_back(Point3(3 + 1e-10, -1, 1));
	shapeB.push_back(Point3(3 + 1e-10, 1, 1));
	shapeB.push_back(Point3(1 + 1e-10, 1, 1));
	std::vector<GJK::Vertex> simplex;
	EXPECT_TRUE(GJK::intersect3D(shapeA, shapeB, simplex));
	GJK::expandToTetrahedron(simplex, shapeA, shapeB);
	GJK::PenetrationInfo info;
	GJK::computePenetration(shapeA, shapeB, simplex, info);
	cout << "--------------------------------------------" << endl;

}

// 形状A是一个点，形状B是立方体，验证正确性
//TEST(GJKTest, GJKTest22) {
//	std::vector<Point3> shapeA;
//	shapeA.push_back(Point3(2, 2, 2));
//
//	std::vector<Point3> shapeB;
//	shapeB.push_back(Point3(1, 1, 1));
//	shapeB.push_back(Point3(3, 1, 1));
//	shapeB.push_back(Point3(3, 3, 1));
//	shapeB.push_back(Point3(1, 3, 1));
//	shapeB.push_back(Point3(1, 1, 3));
//	shapeB.push_back(Point3(3, 1, 3));
//	shapeB.push_back(Point3(3, 3, 3));
//	shapeB.push_back(Point3(1, 3, 3));
//	std::vector<GJK::Vertex> simplex;
//	EXPECT_TRUE(GJK::intersect3D(shapeA, shapeB, simplex));
//	GJK::expandToTetrahedron(simplex, shapeA, shapeB);
//	GJK::PenetrationInfo info;
//	GJK::computePenetration(shapeA, shapeB, simplex, info);
//	cout << "--------------------------------------------" << endl;
//
//}

// 生成随机立方体
MeshObject* generateRandomCube(std::mt19937& rng) {
	std::uniform_real_distribution<double> posDist(-50.0, 50.0);
	std::uniform_real_distribution<double> sizeDist(0.5, 3.0);

	double x = posDist(rng);
	double y = posDist(rng);
	double z = posDist(rng);
	double sx = sizeDist(rng);
	double sy = sizeDist(rng);
	double sz = sizeDist(rng);

	std::vector<Point3> vertices = {
		Point3(x, y, z),
		Point3(x + sx, y, z),
		Point3(x + sx, y + sy, z),
		Point3(x, y + sy, z),
		Point3(x, y, z + sz),
		Point3(x + sx, y, z + sz),
		Point3(x + sx, y + sy, z + sz),
		Point3(x, y + sy, z + sz)
	};

	MeshObject* cube = new MeshObject();
	cube->initCube(vertices);
	return cube;
}

// 性能对比：
// 100个随机凸体之间的两两碰撞检测。
// 对比GJK耗时和基于BVH的三角形级别碰撞检测耗时。
// GJK应该比BVH三角形级别检测快很多（因为GJK是O(N)而三角形级别是O(M×N)）
TEST(GJKTest, GJKTest23) {

	std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

	// 生成100个随机立方体（可修改数量测试不同场景）
	const int NUM_OBJECTS = 1000;
	std::vector<MeshObject*> objects;
	for (int i = 0; i < NUM_OBJECTS; i++) {
		objects.push_back(generateRandomCube(rng));
	}

	// 控制台输出：英文
	std::cout << "=== Performance Comparison of Three Collision Algorithms ===" << std::endl;
	std::cout << "Test Objects: " << NUM_OBJECTS << " random cubes" << std::endl;
	std::cout << "Each cube: 8 vertices, 12 triangles" << std::endl << std::endl;

	// ==================== 1. 测试纯AABB SAT（第一层粗筛） ====================
	std::cout << "Testing AABB SAT only..." << std::endl;
	auto startAABB = std::chrono::high_resolution_clock::now();
	int collisionsAABB = 0;
	for (size_t i = 0; i < NUM_OBJECTS; i++) {
		for (size_t j = i + 1; j < NUM_OBJECTS; j++) {
			// SAT
			if (objects[i]->aabb.intersects(objects[j]->aabb)) {
				collisionsAABB++;
			}
		}
	}
	auto endAABB = std::chrono::high_resolution_clock::now();
	double timeAABB = std::chrono::duration<double, std::milli>(endAABB - startAABB).count();

	// ==================== 2. 测试GJK(AABB凸包近似) ====================
	std::cout << "Testing GJK (AABB Convex Hull)..." << std::endl;
	auto startGJK = std::chrono::high_resolution_clock::now();
	int collisionsGJK = 0;
	for (size_t i = 0; i < NUM_OBJECTS; i++) {
		for (size_t j = i + 1; j < NUM_OBJECTS; j++) {
			// GJK
			std::vector<GJK::Vertex> simplex;
			if (GJK::intersect3D(objects[i]->vertices, objects[j]->vertices, simplex)) {
				collisionsGJK++;
				//if (!objects[i]->aabb.intersects(objects[j]->aabb)) {
				//	AABB aabox = objects[i]->aabb;
				//	AABB bbbox = objects[j]->aabb;
				//	AABB aBox;
				//	for (Point3& p : objects[i]->vertices) {
				//		aBox.expand(p);
				//	}
				//	AABB bBox;
				//	for (Point3& p : objects[j]->vertices) {
				//		bBox.expand(p);
				//	}
				//	std::vector<GJK::Vertex> simplex2;
				//	GJK::intersect3D(objects[i]->vertices, objects[j]->vertices, simplex2);
				//	std::cout << "----------------------" << std::endl;
				//}
			}
		}
	}
	auto endGJK = std::chrono::high_resolution_clock::now();
	double timeGJK = std::chrono::duration<double, std::milli>(endGJK - startGJK).count();

	// ==================== 3. 测试BVH三角形级别检测 ====================
	std::cout << "Testing BVH Triangle-Level Detection..." << std::endl << std::endl;
	auto startBVH = std::chrono::high_resolution_clock::now();
	int collisionsBVH = 0;
	for (size_t i = 0; i < NUM_OBJECTS; i++) {
		for (size_t j = i + 1; j < NUM_OBJECTS; j++) {
			// BVH
			std::vector<std::pair<int, int>> collisionPairs;
			::collideBVH(objects[i]->bvhRoot, objects[j]->bvhRoot, objects[i]->triangles, objects[j]->triangles, collisionPairs);
			if (collisionPairs.size() > 0) ++collisionsBVH;
			//if (collisionPairs.size() > 0) {
			//	++collisionsBVH;
			//	std::vector<GJK::Vertex> simplex;
			//	if (!GJK::intersect3D(objects[i]->vertices, objects[j]->vertices, simplex)) {
			//		GJK::intersect3D(objects[i]->vertices, objects[j]->vertices, simplex);
			//		std::cout << "----------------------" << std::endl;
			//	}
			//}
		}
	}
	auto endBVH = std::chrono::high_resolution_clock::now();
	double timeBVH = std::chrono::duration<double, std::milli>(endBVH - startBVH).count();

	// ==================== 控制台输出结果（英文） ====================
	std::cout << std::fixed << std::setprecision(3);
	std::cout << "=== Final Test Results ===" << std::endl;
	std::cout << "1. AABB SAT Only" << std::endl;
	std::cout << "   Time: " << timeAABB << " ms" << std::endl;
	std::cout << "   Collisions Detected: " << collisionsAABB << std::endl;
	std::cout << "   Relative Speed: " << timeBVH / timeAABB << "x faster than BVH" << std::endl << std::endl;

	std::cout << "2. GJK (AABB Convex Hull Approximation)" << std::endl;
	std::cout << "   Time: " << timeGJK << " ms" << std::endl;
	std::cout << "   Collisions Detected: " << collisionsGJK << std::endl;
	std::cout << "   Relative Speed: " << timeBVH / timeGJK << "x faster than BVH" << std::endl;
	std::cout << "   False Positive Rate: " << (collisionsGJK - collisionsBVH) * 100.0 / collisionsBVH << " %" << std::endl << std::endl;

	std::cout << "3. BVH Triangle-Level Detection (Baseline)" << std::endl;
	std::cout << "   Time: " << timeBVH << " ms" << std::endl;
	std::cout << "   Collisions Detected: " << collisionsBVH << " (Most Accurate)" << std::endl << std::endl;

	// ==================== 写入结果到docs/gjk_perf.md（保持中文不变） ====================
	std::ofstream file("docs/gjk_perf.md");
	if (file.is_open()) {
		file << "# GJK vs AABB SAT vs BVH 性能对比测试" << std::endl << std::endl;
		file << "## 测试环境" << std::endl;
		file << "- 物体数量:" << NUM_OBJECTS << " 个随机立方体" << std::endl;
		file << "- 每个立方体：8个顶点，12个三角形" << std::endl;
		file << "- 测试时间:" << __DATE__ << " " << __TIME__ << std::endl << std::endl;

		file << "## 性能对比" << std::endl << std::endl;
		file << "| 算法 | 耗时(ms) | 检测碰撞数 | 相对BVH速度 | 误报率 |" << std::endl;
		file << "|------|----------|------------|-------------|--------|" << std::endl;
		file << "| 纯AABB SAT | " << timeAABB << " | " << collisionsAABB << " | " << timeBVH / timeAABB << "x | " << (collisionsAABB - collisionsBVH) * 100.0 / collisionsBVH << "% |" << std::endl;
		file << "| GJK(AABB凸包) | " << timeGJK << " | " << collisionsGJK << " | " << timeBVH / timeGJK << "x | " << (collisionsGJK - collisionsBVH) * 100.0 / collisionsBVH << "% |" << std::endl;
		file << "| BVH三角形级别 | " << timeBVH << " | " << collisionsBVH << " | 1x | 0% |" << std::endl << std::endl;

		//file << "## 结论" << std::endl << std::endl;
		//file << "1.纯AABB、SAT速度最快,适合作为第一层粗筛" << std::endl;
		//file << "2.GJK速度次之,精度高于AABB,适合作为第二层过滤" << std::endl;
		//file << "3.BVH三角形级别最精确,但速度最慢,适合最终确认" << std::endl;
		//file << "4.工业界标准:三层分层检测策略" << std::endl;

		file.close();
		std::cout << "Results have been written to docs/gjk_perf.md" << std::endl;
	}

}


