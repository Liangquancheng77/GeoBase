#pragma once
#include "BVH.h"

class MeshObject {

public:

	std::vector<Point3> vertices;
	std::vector<Triangle3> triangles;
	AABB aabb;
	BVHNode* bvhRoot;

	MeshObject();

	~MeshObject();

	// 禁止拷贝构造 & 赋值（避免裸指针重复释放）
	MeshObject(const MeshObject&)  = delete;
	MeshObject& operator=(const MeshObject&) = delete;

	// 初始化立方体
	void initCube(const std::vector<Vector3>& cubeVertices);
	// 计算包围盒
	void computeAABB();
	// 构建 BVH
	void buildBVH();

};
