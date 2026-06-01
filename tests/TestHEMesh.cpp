#include <gtest/gtest.h>
#include "../include/GeoBase/HEMesh.h"
using namespace std;

//TEST(TestHEMesh, TestHEMesh1) {
//	HEMesh mesh;
//
//	mesh.loadOBJ("cube.obj");
//
//    auto faces = mesh.getFaces();
//	auto edges = mesh.getHalfEdges();
//	auto verties = mesh.getVertices();
//
//	std::cout << "  Vertices:    " << mesh.numVerts() << std::endl;
//	std::cout << "  Half-edges:  " << mesh.numHalfEdges() << std::endl;
//	std::cout << "  Faces:       " << mesh.numFaces() << std::endl;
//
//	cout << "-----" << endl;
//
//}
//
//TEST(TestHEMesh, TestHEMesh2) {
//	
//	HEMesh mesh;
//
//	mesh.loadOBJ("tetrahedron.obj");
//
//	auto faces = mesh.getFaces();
//	auto edges = mesh.getHalfEdges();
//	auto verties = mesh.getVertices();
//
//	std::cout << "  Vertices:    " << mesh.numVerts() << std::endl;
//	std::cout << "  Half-edges:  " << mesh.numHalfEdges() << std::endl;
//	std::cout << "  Faces:       " << mesh.numFaces() << std::endl;
//
//	cout << "-----" << endl;
//
//}
//
// //面半边遍历
//TEST(TestHEMesh, TestHEMesh3) {
//
//	HEMesh mesh;
//
//	mesh.loadOBJ("tetrahedron.obj");
//
//	mesh.forEachHalfEdgeInFace(mesh.getFaces()[0], [&](HEHalfEdge* he) {
//			std::cout << *he->vertex << std::endl;
//		});
//
//	cout << "--------------------------------------------" << endl;
//
//}
//
//// 顶点邻边遍历
//TEST(TestHEMesh, TestHEMesh4) {
//
//	HEMesh mesh;
//
//	mesh.loadOBJ("tetrahedron.obj");
//
//	mesh.forEachOutgoingHalfEdge(mesh.getVertices()[3], [&](HEHalfEdge* he) {
//		std::cout << *he->vertex << std::endl;
//		return true;
//		});
//
//	cout << "--------------------------------------------" << endl;
//
//}
//
//// 顶点邻点遍历
//TEST(TestHEMesh, TestHEMesh5) {
//
//	HEMesh mesh;
//
//	mesh.loadOBJ("tetrahedron.obj");
//
//	mesh.forEachNeighborVertex(mesh.getVertices()[0], [&](HEVert* v) {
//		std::cout << *v << std::endl;
//		return true;
//		});
//
//	cout << "--------------------------------------------" << endl;
//
//}
//
//// 创建单个三角形，验证顶点数=3，半边数=6，面数=1，网格不封闭
//TEST(TestHEMesh, TestHEMesh6) {
//
//	HEMesh mesh;
//	mesh.addTriangle(Point3(1.0, 0, 0), Point3(0, 1.0, 0), Point3(0, 0, -1.0));
//
//	auto faces = mesh.getFaces();
//	auto edges = mesh.getHalfEdges();
//	auto verties = mesh.getVertices();
//
//	auto boundaryEdges = mesh.getBoundaryEdges();
//	EXPECT_EQ(boundaryEdges.size(), 3);
//
//	EXPECT_EQ(verties.size(), 3);
//	EXPECT_EQ(edges.size(), 6);
//	EXPECT_EQ(faces.size(), 1);
//
//	EXPECT_FALSE(mesh.isClosed());
//	//EXPECT_TRUE(mesh.validate());
//
//}
//
//// 创建两个相邻三角形，验证共享边的pair关系正确,网格不封闭
//TEST(TestHEMesh, TestHEMesh7) {
//
//	HEMesh mesh;
//	mesh.addTriangle(Point3(1.0, 0, 0), Point3(0, 1.0, 0), Point3(0, 0, -1.0));
//	mesh.addTriangle(Point3(0, 1.0, 0), Point3(1.0, 0, 0), Point3(0, 0, 1.0));
//
//	auto faces = mesh.getFaces();
//	auto edges = mesh.getHalfEdges();
//	auto verties = mesh.getVertices();
//
//	auto boundaryEdges = mesh.getBoundaryEdges();
//	EXPECT_EQ(boundaryEdges.size(), 4);
//
//	EXPECT_EQ(verties.size(), 4);
//	EXPECT_EQ(edges.size(), 10);
//	EXPECT_EQ(faces.size(), 2);
//	
//
//	EXPECT_FALSE(mesh.isClosed());
//	//EXPECT_TRUE(mesh.validate());
//
//}
//
//// 创建四面体，验证validate,网格封闭
//TEST(TestHEMesh, TestHEMesh8) {
//
//	HEMesh mesh;
//	mesh.addTriangle(Point3(1.0, 0, 0), Point3(0, 1.0, 0), Point3(0, 0, -1.0));
//	mesh.addTriangle(Point3(0, 1.0, 0), Point3(1.0, 0, 0), Point3(0, 0, 1.0));
//	mesh.addTriangle(Point3(0, 1.0, 0), Point3(0, 0, 1.0), Point3(0, 0, -1.0));
//	mesh.addTriangle(Point3(1, 0, 0), Point3(0, 0, -1.0), Point3(0, 0, 1.0));
//
//	auto faces = mesh.getFaces();
//	auto edges = mesh.getHalfEdges();
//	auto verties = mesh.getVertices();
//
//	auto boundaryEdges = mesh.getBoundaryEdges();
//	EXPECT_EQ(boundaryEdges.size(), 0);
//
//	EXPECT_EQ(verties.size(), 4);
//	EXPECT_EQ(edges.size(), 12);
//	EXPECT_EQ(faces.size(), 4);
//
//	EXPECT_TRUE(mesh.validate());
//	EXPECT_TRUE(mesh.isClosed());
//
//}
//
//// 提取边界环
//TEST(TestHEMesh, TestHEMesh9) {
//
//	HEMesh mesh;
//	mesh.addTriangle(Point3(1.0, 0, 0), Point3(0, 1.0, 0), Point3(0, 0, -1.0));
//	mesh.addTriangle(Point3(0, 1.0, 0), Point3(1.0, 0, 0), Point3(0, 0, 1.0));
//	mesh.addTriangle(Point3(0, 1.0, 0), Point3(0, 0, 1.0), Point3(0, 0, -1.0));
//
//	auto loops = mesh.getBoundaryLoops();
//
//	cout << "--------------------------------------------" << endl;
//}
//
//// 判断顶点是否在边界上
//TEST(TestHEMesh, TestHEMesh10) {
//	HEMesh mesh;
//	mesh.addTriangle(Point3(1.0, 0, 0), Point3(0, 1.0, 0), Point3(0, 0, -1.0));
//	mesh.addTriangle(Point3(0, 1.0, 0), Point3(1.0, 0, 0), Point3(0, 0, 1.0));
//	mesh.addTriangle(Point3(0, 1.0, 0), Point3(0, 0, 1.0), Point3(0, 0, -1.0));
//
//	EXPECT_TRUE(mesh.isBoundaryVertex(mesh.getVertices()[0]));
//	EXPECT_FALSE(mesh.isBoundaryVertex(mesh.getVertices()[1]));
//
//}
//
//// 测试创建非流形边（三个面共享一条边）
//TEST(TestHEMesh, TestHEMesh11) {
//
//	HEMesh mesh;
//	mesh.addTriangle(Point3(1.0, 0, 0), Point3(0, 1.0, 0), Point3(0, 0, -1.0));
//	mesh.addTriangle(Point3(0, 1.0, 0), Point3(1.0, 0, 0), Point3(0, 0, 1.0));
//	EXPECT_THROW(mesh.addTriangle(Point3(0, 1.0, 0), Point3(1.0, 0, 0), Point3(0, 0, 2.0)), std::runtime_error);
//}
//
// =============================边翻转==============================
// 创建一个由两个三角形组成的四边形，执行翻转，验证翻转后仍有两个三角形，对角线顶点变化
//TEST(TestHEMesh, TestHEMesh12) {
//	HEMesh mesh;
//	mesh.addTriangle(Point3(0, -1.0, 0), Point3(0, 1.0, 0), Point3(0, 0, -1.0));
//	mesh.addTriangle(Point3(0, -1.0, 0), Point3(0, 0, 1.0), Point3(0, 1.0, 0));
//	EXPECT_TRUE(mesh.flipEdge(mesh.getHalfEdges()[0]));
//	cout << "--------------------------------------------" << endl;
//}
//
//// 连续翻转两次，验证回到原始状态
//// 创建一个由两个三角形组成的四边形，执行翻转，验证翻转后仍有两个三角形，对角线顶点变化
//TEST(TestHEMesh, TestHEMesh13) {
//	HEMesh mesh;
//	mesh.addTriangle(Point3(0, -1.0, 0), Point3(0, 1.0, 0), Point3(0, 0, -1.0));
//	mesh.addTriangle(Point3(0, -1.0, 0), Point3(0, 0, 1.0), Point3(0, 1.0, 0));
//	EXPECT_TRUE(mesh.flipEdge(mesh.getHalfEdges()[0]));
//	EXPECT_TRUE(mesh.flipEdge(mesh.getHalfEdges()[0]));
//	cout << "--------------------------------------------" << endl;
//}
//
//// 对立方体随机执行多次翻转，每次翻转后调用validate验证网格完整性
//TEST(TestHEMesh, TestHEMesh14) {
//	HEMesh mesh;
//	mesh.loadOBJ("cube.obj");
//	EXPECT_TRUE(mesh.flipEdge(mesh.getHalfEdges()[4]));
//	EXPECT_TRUE(mesh.flipEdge(mesh.getHalfEdges()[11]));
//	EXPECT_TRUE(mesh.flipEdge(mesh.getHalfEdges()[11]));
//	EXPECT_TRUE(mesh.flipEdge(mesh.getHalfEdges()[10]));
//	EXPECT_TRUE(mesh.validate());
//	
//}
//
//// 尝试翻转边界边（pair为空），验证返回false
//TEST(TestHEMesh, TestHEMesh15) {
//	HEMesh mesh;
//	mesh.addTriangle(Point3(0, -1.0, 0), Point3(0, 1.0, 0), Point3(0, 0, -1.0));
//	mesh.addTriangle(Point3(0, -1.0, 0), Point3(0, 0, 1.0), Point3(0, 1.0, 0));
//	EXPECT_FALSE(mesh.flipEdge(mesh.getHalfEdges()[2]));
//	cout << "--------------------------------------------" << endl;
//}
//
// // 退化三角形，翻转失败
//TEST(TestHEMesh, TestHEMesh16) {
//	HEMesh mesh;
//	mesh.addTriangle(Point3(0, 0, 0), Point3(0, 1.0, 0), Point3(0, 0, -1.0));
//	mesh.addTriangle(Point3(0, 0, 0), Point3(0, 0, 1.0), Point3(0, 1.0, 0));
//	EXPECT_FALSE(mesh.flipEdge(mesh.getHalfEdges()[0]));
//
//}
//
//// =============================边折叠==============================
//
// 对四面体的一条边折叠，验证面数减少2，顶点数减少1
//TEST(TestHEMesh, TestHEMesh17) {
//	HEMesh mesh;
//	mesh.loadOBJ("tetrahedron.obj");
//	EXPECT_TRUE(mesh.collapseEdge(mesh.getHalfEdges()[2]));
//	EXPECT_EQ(mesh.numFaces(), 2);
//	EXPECT_EQ(mesh.numVerts(), 3);
//	EXPECT_EQ(mesh.numHalfEdges(), 6);
//}
//
////// 折叠到中点，验证新顶点位置正确
////TEST(TestHEMesh, TestHEMesh18) {
////	HEMesh mesh;
////	mesh.loadOBJ("tetrahedron.obj");
////	bool ok = mesh.collapseEdge(mesh.getHalfEdges()[2]);
////	EXPECT_TRUE(ok);
////	cout << "--------------------------------------------" << endl;
////}
//
//// 对立方体连续折叠多条边，每次验证validate通过
//TEST(TestHEMesh, TestHEMesh19) {
//	HEMesh mesh;
//	mesh.loadOBJ("cube.obj");
//	//EXPECT_TRUE(mesh.validate());
//	EXPECT_TRUE(mesh.collapseEdge(mesh.getHalfEdges()[2]));
//	//EXPECT_TRUE(mesh.collapseEdge(mesh.getHalfEdges()[4]));
//	//EXPECT_TRUE(mesh.collapseEdge(mesh.getHalfEdges()[6]));
//	EXPECT_TRUE(mesh.validate());
//}


// =============================边分割==============================
// 普通测试
//TEST(TestHEMesh, TestHEMesh20) {
//	HEMesh mesh;
//	mesh.addTriangle(Point3(0, 0, 0), Point3(2.0, 0, 0), Point3(1.0, 1.0, 0));
//	mesh.addTriangle(Point3(0, 0, 0), Point3(1.0, -1.0, 0), Point3(2.0, 0, 0));
//	HEVert* X = mesh.splitEdge(mesh.getHalfEdges()[0]);
//
//}

// 测试四面体边分割后的网格完整性
//TEST(TestHEMesh, TestHEMesh21) {
//	HEMesh mesh;
//	mesh.loadOBJ("tetrahedron.obj");
//	mesh.meshQualityReport();
//	HEVert* X = mesh.splitEdge(mesh.getHalfEdges()[0], 0.5);
//	EXPECT_TRUE(mesh.validate());
//	mesh.meshQualityReport();
//}
//
//// 对立方体执行随机边分割，验证面数增加，面积缩小
//TEST(TestHEMesh, TestHEMesh22) {
//	HEMesh mesh;
//	mesh.loadOBJ("cube.obj");
//	mesh.meshQualityReport();
//	HEVert* X = mesh.splitEdge(mesh.getHalfEdges()[randomInt(0, mesh.numHalfEdges() - 1)], randomDouble(0, 1));
//	EXPECT_TRUE(mesh.validate());
//	mesh.meshQualityReport();
//}
//
//// 对茶壶OBJ运行质量报告
//TEST(TestHEMesh, TestHEMesh23) {
//	HEMesh mesh;
//	mesh.loadOBJ("teapot.obj");
//	mesh.meshQualityReport();
//}


// =============================退化测试==============================

// 空网格（0顶点、0面）：所有遍历函数不崩溃
//TEST(TestHEMesh, TestHEMesh24) {
//	HEMesh mesh;
//
//	// 随便找个不存在的顶点（这里不传），或空遍历
//	size_t cnt = 0;
//	// 空网格没有顶点，下面这行通常不会触发
//	// 但你接口要保证：传 nullptr 也不崩
//	mesh.forEachFaceAroundVertex(nullptr, [&](HEFace*) { cnt++; return true; });
//
//	EXPECT_EQ(cnt, 0);
//}

// 单个三角形网格：遍历正确，只有边界边
//TEST(TestHEMesh, TestHEMesh25) {
//	HEMesh mesh;
//	mesh.addTriangle(Point3(0, -1.0, 0), Point3(0, 1.0, 0), Point3(0, 0, -1.0));
//	int boundaryCount = 0;
//	mesh.forEachOutgoingHalfEdge(mesh.getVertices()[0], [&](HEHalfEdge* he) -> bool {
//		if (mesh.isBoundary(he)) {
//			++boundaryCount;
//		}
//		return true;
//	});
//	EXPECT_EQ(boundaryCount, 2);
//}

// 非流形边（三个三角形共享一条边）
//TEST(TestHEMesh, TestHEMesh26) {
//	HEMesh mesh;
//	mesh.addTriangle(Point3(1.0, 0, 0), Point3(0, 1.0, 0), Point3(0, 0, -1.0));
//	mesh.addTriangle(Point3(0, 1.0, 0), Point3(1.0, 0, 0), Point3(0, 0, 1.0));
//	EXPECT_THROW(mesh.addTriangle(Point3(0, 1.0, 0), Point3(1.0, 0, 0), Point3(0, 0, 2.0)), std::runtime_error);
//
//}
//
//// 大量随机边翻转：100次随机翻转后，validate仍然通过
//TEST(TestHEMesh, TestHEMesh27) {
//	HEMesh mesh;
//	mesh.loadOBJ("cube.obj");
//
//	for (int i = 0; i < 100; i++)
//	{
//		mesh.flipEdge(mesh.getHalfEdges()[randomInt(0, mesh.numHalfEdges() - 1)]);
//	}
//	mesh.validate();
//}

// =============================QEM集成到半边结构==============================
//TEST(TestHEMesh, TestHEMesh28) {
//	std::priority_queue < EdgeCollapse, std::vector<EdgeCollapse>, std::greater<EdgeCollapse>> pq;
//	//HEHalfEdge* he = new HEHalfEdge();
//	EdgeCollapse e1(new HEHalfEdge(), 0.15, Point3(1,1,1));
//	EdgeCollapse e2(new HEHalfEdge(), 0.01, Point3(2,1,1));
//	EdgeCollapse e3(new HEHalfEdge(), 0.25, Point3(3,1,1));
//	EdgeCollapse e4(new HEHalfEdge(), 0.14, Point3(4,1,1));
//
//	pq.push(e1);
//	pq.push(e2);
//	pq.push(e3);
//	pq.push(e4);
//
//	EdgeCollapse e5 = pq.top(); pq.pop();
//	EdgeCollapse e6 = pq.top(); pq.pop();
//	EdgeCollapse e7 = pq.top(); pq.pop();
//	EdgeCollapse e8 = pq.top(); pq.pop();
//
//
//	cout << "--------------------------------------------" << endl;
//
//}

// 加载正方体OBJ（12面），简化到6面，验证简化次数为1，面数减少到6，顶点数减少到5，半边数减少到18
//TEST(TestHEMesh, TestHEMesh29) {
//	HEMesh mesh;
//	mesh.loadOBJ("cube.obj");
//	mesh.simplify(6);
//	EXPECT_EQ(mesh.numFaces(), 6);
//	EXPECT_EQ(mesh.numVerts(), 5);
//	EXPECT_EQ(mesh.numHalfEdges(), 18);
//	
//
//}

// 加载茶壶OBJ，简化到50%面数，打印简化后的面数
//TEST(TestHEMesh, TestHEMesh30) {
//	HEMesh mesh;
//	mesh.loadOBJ("teapot2.obj");
//	mesh.simplify(mesh.numFaces() * 0.5);
//	EXPECT_EQ(mesh.numFaces(), 6);
//	EXPECT_EQ(mesh.numVerts(), 5);
//	EXPECT_EQ(mesh.numHalfEdges(), 18);
//
//
//}

// 加载正方体OBJ,比正常正方体少一个三角形面
TEST(TestHEMesh, TestHEMesh31) {
	HEMesh mesh;
	mesh.loadOBJ("cube.obj");
	mesh.simplify(mesh.numFaces() * 0.5);
	cout << "--------------------------------------------" << endl;
}

// 简化到只剩2个面（或更少），验证不崩溃
TEST(TestHEMesh, TestHEMesh32) {
	HEMesh mesh;
	mesh.loadOBJ("cube.obj");
	mesh.simplify(2);
	cout << "--------------------------------------------" << endl;
}

// 对一个平面网格（所有顶点共面）执行简化，验证Q矩阵正确
TEST(TestHEMesh, TestHEMesh33) {
	HEMesh mesh;
	mesh.addTriangle(Point3(0, 0, -1.0), Point3(1.0, 0, 0), Point3(0, 0, 1.0));
	mesh.addTriangle(Point3(0, 0, 1.0), Point3(1.0, 0, 0), Point3(1.0, 0, 2.0));
	mesh.addTriangle(Point3(0, 0, 1.0), Point3(1.0, 0, 2.0), Point3(0, 0, 3.0));
	mesh.addTriangle(Point3(0, 0, 3.0), Point3(1.0, 0, 4.0), Point3(0, 0, 5.0));
	mesh.simplify(2);
	cout << "--------------------------------------------" << endl;
}