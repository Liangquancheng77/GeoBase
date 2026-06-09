#include "../include/GeoBase/MeshObject.h"


MeshObject::MeshObject() {}

MeshObject::~MeshObject() {
	delete bvhRoot;
	bvhRoot = nullptr;
}


// 初始化立方体
void MeshObject::initCube(const std::vector<Vector3>& cubeVertices) {
	vertices.clear();
	triangles.clear();
	delete bvhRoot;
	bvhRoot = nullptr;
	vertices = cubeVertices;

    // 生成立方体12个三角形（标准拓扑）
    triangles = {
        // 前
        Triangle3{cubeVertices[0], cubeVertices[1], cubeVertices[2]},
        Triangle3{cubeVertices[0], cubeVertices[2], cubeVertices[3]},

        // 后
        Triangle3{cubeVertices[4], cubeVertices[5], cubeVertices[6]},
        Triangle3{cubeVertices[4], cubeVertices[6], cubeVertices[7]},

        // 下
        Triangle3{cubeVertices[0], cubeVertices[1], cubeVertices[5]},
        Triangle3{cubeVertices[0], cubeVertices[5], cubeVertices[4]},

        // 上
        Triangle3{cubeVertices[2], cubeVertices[3], cubeVertices[7]},
        Triangle3{cubeVertices[2], cubeVertices[7], cubeVertices[6]},

        // 右
        Triangle3{cubeVertices[1], cubeVertices[2], cubeVertices[6]},
        Triangle3{cubeVertices[1], cubeVertices[6], cubeVertices[5]},

        // 左
        Triangle3{cubeVertices[0], cubeVertices[3], cubeVertices[7]},
        Triangle3{cubeVertices[0], cubeVertices[7], cubeVertices[4]}

    };

    computeAABB();

    buildBVH();

    vertices = {
    Vector3(aabb.min.x, aabb.min.y, aabb.min.z),
    Vector3(aabb.max.x, aabb.min.y, aabb.min.z),
    Vector3(aabb.max.x, aabb.max.y, aabb.min.z),
    Vector3(aabb.min.x, aabb.max.y, aabb.min.z),
    Vector3(aabb.min.x, aabb.min.y, aabb.max.z),
    Vector3(aabb.max.x, aabb.min.y, aabb.max.z),
    Vector3(aabb.max.x, aabb.max.y, aabb.max.z),
    Vector3(aabb.min.x, aabb.max.y, aabb.max.z)
};

}

// 计算包围盒
void MeshObject::computeAABB() {
    aabb = AABB();
    for (Point3& vert : vertices)
    {
        aabb.expand(vert);
    }
}

// 构建 BVH
void MeshObject::buildBVH() {
    bvhRoot = ::buildBVH(triangles, 0, (int)triangles.size(), 0);
}
