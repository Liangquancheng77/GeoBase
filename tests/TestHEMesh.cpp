#include <gtest/gtest.h>
#include "../include/GeoBase/HEMesh.h"
using namespace std;

TEST(TestHEMesh, TestHEMesh1) {
	HEMesh mesh;

	mesh.loadOBJ("cube.obj");

    auto faces = mesh.getFaces();
	auto edges = mesh.getHalfEdges();
	auto verties = mesh.getVertices();

	std::cout << "  Vertices:    " << mesh.numVerts() << std::endl;
	std::cout << "  Half-edges:  " << mesh.numHalfEdges() << std::endl;
	std::cout << "  Faces:       " << mesh.numFaces() << std::endl;

	cout << "-----" << endl;

}

TEST(TestHEMesh, TestHEMesh2) {
	
	HEMesh mesh;

	mesh.loadOBJ("tetrahedron.obj");

	auto faces = mesh.getFaces();
	auto edges = mesh.getHalfEdges();
	auto verties = mesh.getVertices();

	std::cout << "  Vertices:    " << mesh.numVerts() << std::endl;
	std::cout << "  Half-edges:  " << mesh.numHalfEdges() << std::endl;
	std::cout << "  Faces:       " << mesh.numFaces() << std::endl;

	cout << "-----" << endl;

}

 //面半边遍历
TEST(TestHEMesh, TestHEMesh3) {

	HEMesh mesh;

	mesh.loadOBJ("tetrahedron.obj");

	mesh.forEachHalfEdgeInFace(mesh.getFaces()[0], [&](HEHalfEdge* he) {
			std::cout << *he->vertex << std::endl;
		});

	cout << "--------------------------------------------" << endl;

}

// 顶点邻边遍历
TEST(TestHEMesh, TestHEMesh4) {

	HEMesh mesh;

	mesh.loadOBJ("tetrahedron.obj");

	mesh.forEachOutgoingHalfEdge(mesh.getVertices()[3], [&](HEHalfEdge* he) {
		std::cout << *he->vertex << std::endl;
		});

	cout << "--------------------------------------------" << endl;

}

// 顶点邻点遍历
TEST(TestHEMesh, TestHEMesh5) {

	HEMesh mesh;

	mesh.loadOBJ("tetrahedron.obj");

	mesh.forEachNeighborVertex(mesh.getVertices()[0], [&](HEVert* v) {
		std::cout << *v << std::endl;
		});

	cout << "--------------------------------------------" << endl;

}

// 创建单个三角形，验证顶点数=3，半边数=6，面数=1，网格不封闭
TEST(TestHEMesh, TestHEMesh6) {

	HEMesh mesh;
	mesh.addTriangle(Point3(1.0, 0, 0), Point3(0, 1.0, 0), Point3(0, 0, -1.0));

	auto faces = mesh.getFaces();
	auto edges = mesh.getHalfEdges();
	auto verties = mesh.getVertices();

	auto boundaryEdges = mesh.getBoundaryEdges();
	EXPECT_EQ(boundaryEdges.size(), 3);

	EXPECT_EQ(verties.size(), 3);
	EXPECT_EQ(edges.size(), 6);
	EXPECT_EQ(faces.size(), 1);

	EXPECT_FALSE(mesh.isClosed());
	//EXPECT_TRUE(mesh.validate());

}

// 创建两个相邻三角形，验证共享边的pair关系正确,网格不封闭
TEST(TestHEMesh, TestHEMesh7) {

	HEMesh mesh;
	mesh.addTriangle(Point3(1.0, 0, 0), Point3(0, 1.0, 0), Point3(0, 0, -1.0));
	mesh.addTriangle(Point3(0, 1.0, 0), Point3(1.0, 0, 0), Point3(0, 0, 1.0));

	auto faces = mesh.getFaces();
	auto edges = mesh.getHalfEdges();
	auto verties = mesh.getVertices();

	auto boundaryEdges = mesh.getBoundaryEdges();
	EXPECT_EQ(boundaryEdges.size(), 4);

	EXPECT_EQ(verties.size(), 4);
	EXPECT_EQ(edges.size(), 10);
	EXPECT_EQ(faces.size(), 2);
	

	EXPECT_FALSE(mesh.isClosed());
	//EXPECT_TRUE(mesh.validate());

}

// 创建四面体，验证validate,网格封闭
TEST(TestHEMesh, TestHEMesh8) {

	HEMesh mesh;
	mesh.addTriangle(Point3(1.0, 0, 0), Point3(0, 1.0, 0), Point3(0, 0, -1.0));
	mesh.addTriangle(Point3(0, 1.0, 0), Point3(1.0, 0, 0), Point3(0, 0, 1.0));
	mesh.addTriangle(Point3(0, 1.0, 0), Point3(0, 0, 1.0), Point3(0, 0, -1.0));
	mesh.addTriangle(Point3(1, 0, 0), Point3(0, 0, -1.0), Point3(0, 0, 1.0));

	auto faces = mesh.getFaces();
	auto edges = mesh.getHalfEdges();
	auto verties = mesh.getVertices();

	auto boundaryEdges = mesh.getBoundaryEdges();
	EXPECT_EQ(boundaryEdges.size(), 0);

	EXPECT_EQ(verties.size(), 4);
	EXPECT_EQ(edges.size(), 12);
	EXPECT_EQ(faces.size(), 4);

	EXPECT_TRUE(mesh.validate());
	EXPECT_TRUE(mesh.isClosed());

}

// 提取边界环
TEST(TestHEMesh, TestHEMesh9) {

	HEMesh mesh;
	mesh.addTriangle(Point3(1.0, 0, 0), Point3(0, 1.0, 0), Point3(0, 0, -1.0));
	mesh.addTriangle(Point3(0, 1.0, 0), Point3(1.0, 0, 0), Point3(0, 0, 1.0));
	mesh.addTriangle(Point3(0, 1.0, 0), Point3(0, 0, 1.0), Point3(0, 0, -1.0));

	auto loops = mesh.getBoundaryLoops();

	cout << "--------------------------------------------" << endl;
}

// 判断顶点是否在边界上
TEST(TestHEMesh, TestHEMesh10) {
	HEMesh mesh;
	mesh.addTriangle(Point3(1.0, 0, 0), Point3(0, 1.0, 0), Point3(0, 0, -1.0));
	mesh.addTriangle(Point3(0, 1.0, 0), Point3(1.0, 0, 0), Point3(0, 0, 1.0));
	mesh.addTriangle(Point3(0, 1.0, 0), Point3(0, 0, 1.0), Point3(0, 0, -1.0));

	EXPECT_TRUE(mesh.isBoundaryVertex(mesh.getVertices()[0]));
	EXPECT_FALSE(mesh.isBoundaryVertex(mesh.getVertices()[1]));

}

// 测试创建非流形边（三个面共享一条边）
TEST(TestHEMesh, TestHEMesh11) {

	HEMesh mesh;
	mesh.addTriangle(Point3(1.0, 0, 0), Point3(0, 1.0, 0), Point3(0, 0, -1.0));
	mesh.addTriangle(Point3(0, 1.0, 0), Point3(1.0, 0, 0), Point3(0, 0, 1.0));
	EXPECT_THROW(mesh.addTriangle(Point3(0, 1.0, 0), Point3(1.0, 0, 0), Point3(0, 0, 2.0)), std::runtime_error);
}