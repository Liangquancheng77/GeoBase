#pragma once
#include "Vector3.h"
#include "Triangle3.h"

class GJK {

public:

    struct Vertex {
        Point3 point;   // Minkowski 差点 A-B
        Point3 pointA;  // 形状 A 上的点
        Point3 pointB;  // 形状 B 上的点

        Vertex() {}
        Vertex(Point3 p, Point3 A, Point3 B) : point(p),pointA(A),pointB(B) {}

    };

    struct PenetrationInfo {
        double depth; // 穿透深度
        Vector3 normal; // 分离法线（由B指向A）
        Point3 contactPointA;  // A上的接触点
        Point3 contactPointB;  // B上的接触点
    };

	// 2D版本：判断两个凸多边形是否相交
	static bool intersect2D(const std::vector<Point3>& shapeA, const std::vector<Point3>& shapeB);

	// 3D版本：判断两个凸多面体是否相交
	static bool intersect3D(const std::vector<Point3>& shapeA, 
        const std::vector<Point3>& shapeB, 
        std::vector<Vertex>& simplex);


	// 如果相交，单纯形扩展到四面体
    static void expandToTetrahedron(std::vector<Vertex>& simplex,
        const std::vector<Point3>& shapeA,
        const std::vector<Point3>& shapeB);

    // 计算两个相交凸体的穿透深度与分离法线
    static bool computePenetration(const std::vector<Point3>& shapeA,
        const std::vector<Point3>& shapeB,
        std::vector<Vertex>& simplex,
        PenetrationInfo& info);

private:

    // 支持函数：返回形状在direction方向上投影最大的顶点
    static Point3 support(const std::vector<Point3>& shape, const Vector3& dir);

    // Minkowski差的支持函数
    static Vertex supportMinkowski(const std::vector<Point3>& shapeA,
        const std::vector<Point3>& shapeB,
        const Vector3& dir);

    // 单纯形进化（2D）
    static bool updateSimplex2D(std::vector<Vertex>& simplex, Vector3& direction);

    // 单纯形进化（3D）
    static bool updateSimplex3D(std::vector<Vertex>& simplex, Vector3& direction);

    //// 判断原点是否在线段/三角形内
    //static bool containsOrigin(std::vector<Point3>& simplex, Vector3& direction);

};