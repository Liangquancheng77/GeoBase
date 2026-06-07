#include <gtest/gtest.h>
#include "../include/GeoBase/GJK.h"
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
	shapeB.push_back(Point3(1.00000001, 1.00000001, -1.00000001));
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