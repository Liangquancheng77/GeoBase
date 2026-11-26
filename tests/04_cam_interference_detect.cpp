//#include<gtest/gtest.h>
//#include <BRepPrimAPI_MakeBox.hxx>
//#include <BRepMesh_IncrementalMesh.hxx>
//#include <GProp_GProps.hxx>
//#include <BRepGProp.hxx>
//#include <iostream>
//#include <cmath>
//#include <TopoDS_Shape.hxx>
//#include <TopExp_Explorer.hxx>
//#include <TopoDS_Face.hxx>
//#include <BRep_Tool.hxx>
//#include <Poly_Triangulation.hxx>
//#include <gp_Pnt.hxx>
//#include <vector>
//#include <map>
//#include <array>
//#include <TopoDS.hxx>
//#include "../include/GeoBase/Triangle3.h"
//
//// 计算三角形的三个内角，返回弧度制
//std::array<double, 3> computeTriangleAngles(const Triangle3& tri) {
//    Vector3 e0 = tri.v1 - tri.v0;
//    Vector3 e1 = tri.v2 - tri.v1;
//    Vector3 e2 = tri.v0 - tri.v2;
//
//    double len0 = e0.length();
//    double len1 = e1.length();
//    double len2 = e2.length();
//
//    if (len0 < EPS_ABS || len1 < EPS_ABS || len2 < EPS_ABS) {
//        return { 0, 0, 0 };
//    }
//
//    double angle0 = std::acos((-(e0.dot(e2))) / (len0 * len2));
//    double angle1 = std::acos((-(e0.dot(e1))) / (len0 * len1));
//    double angle2 = std::acos((-(e1.dot(e2))) / (len1 * len2));
//
//    return { angle0, angle1, angle2 };
//}
//
//// 计算边长比，最长边与最短边的比值，衡量三角形是否狭长
//double computeAspectRatio(const Triangle3& tri) {
//    double d0 = (tri.v1 - tri.v0).length();
//    double d1 = (tri.v2 - tri.v1).length();
//    double d2 = (tri.v0 - tri.v2).length();
//    double maxEdge = std::max({ d0, d1, d2 });
//    double minEdge = std::min({ d0, d1, d2 });
//    if (minEdge < EPS_ABS) return std::numeric_limits<double>::infinity();
//    return maxEdge / minEdge;
//}
//
//struct MeshQualityStats {
//    int triangleCount;
//    double minArea, maxArea, avgArea;
//    double minAngle, maxAngle, avgAngle;
//    double minAspectRatio, maxAspectRatio, avgAspectRatio;
//};
//
//// 统计网格整体质量的函数，遍历所有三角形，汇总面积、角度、边长比的分布
//MeshQualityStats analyzeMeshQuality(const std::vector<Triangle3>& triangles) {
//    MeshQualityStats stats;
//    stats.triangleCount = triangles.size();
//    if (triangles.empty()) return stats;
//
//    stats.minArea = std::numeric_limits<double>::max();
//    stats.maxArea = 0;
//    stats.avgArea = 0;
//    stats.minAngle = std::numeric_limits<double>::max();
//    stats.maxAngle = 0;
//    stats.avgAngle = 0;
//    stats.minAspectRatio = std::numeric_limits<double>::max();
//    stats.maxAspectRatio = 0;
//    stats.avgAspectRatio = 0;
//
//    int totalAngles = 0;
//    for (const auto& tri : triangles) {
//        double area = tri.getArea();
//        stats.minArea = std::min(stats.minArea, area);
//        stats.maxArea = std::max(stats.maxArea, area);
//        stats.avgArea += area;
//
//        auto angles = computeTriangleAngles(tri);
//        for (double angle : angles) {
//            stats.minAngle = std::min(stats.minAngle, angle);
//            stats.maxAngle = std::max(stats.maxAngle, angle);
//            stats.avgAngle += angle;
//            totalAngles++;
//        }
//
//        double ar = computeAspectRatio(tri);
//        stats.minAspectRatio = std::min(stats.minAspectRatio, ar);
//        stats.maxAspectRatio = std::max(stats.maxAspectRatio, ar);
//        stats.avgAspectRatio += ar;
//    }
//
//    stats.avgArea /= triangles.size();
//    stats.avgAngle /= totalAngles;
//    stats.avgAspectRatio /= triangles.size();
//
//    return stats;
//}
//
//
//// 提取网格化后的顶点和三角形索引
//void extractMesh(const TopoDS_Shape& shape,
//    std::vector<Point3>& outVertices, 
//    std::vector<std::array<int, 3>>& outTriangles)
//{
//    outVertices.clear();
//    outTriangles.clear();
//
//    // 顶点去重映射，改用 Point3
//    struct PntLess {
//        bool operator()(const Point3& a, const Point3& b) const {
//            if (a.x != b.x) return a.x < b.x;
//            if (a.y != b.y) return a.y < b.y;
//            return a.z < b.z;
//        }
//    };
//    std::map<Point3, int, PntLess> vertexMap;
//
//    // 遍历所有面
//    for (TopExp_Explorer exp(shape, TopAbs_FACE); exp.More(); exp.Next()) {
//        const TopoDS_Face& face = TopoDS::Face(exp.Current());
//        TopLoc_Location loc;
//        Handle(Poly_Triangulation) tri = BRep_Tool::Triangulation(face, loc);
//        if (tri.IsNull()) continue;
//
//        int numNodes = tri->NbNodes();
//        std::vector<int> localToGlobal(numNodes + 1);
//
//        for (int i = 1; i <= numNodes; ++i) {
//            // 读取 OCC 的 gp_Pnt，应用坐标变换，转为你的 Point3
//            gp_Pnt p = tri->Node(i).Transformed(loc);
//            Point3 myPoint(p.X(), p.Y(), p.Z());  // 转为你的 Point3
//
//            auto ins = vertexMap.insert({ myPoint, static_cast<int>(outVertices.size()) });
//            if (ins.second) {
//                outVertices.push_back(myPoint);
//            }
//            localToGlobal[i] = ins.first->second;
//        }
//
//        int numTri = tri->NbTriangles();
//        for (int i = 1; i <= numTri; ++i) {
//            const Poly_Triangle& t = tri->Triangle(i);
//            int i1, i2, i3;
//            t.Get(i1, i2, i3);
//
//            int g1 = localToGlobal[i1];
//            int g2 = localToGlobal[i2];
//            int g3 = localToGlobal[i3];
//            outTriangles.push_back({ g1, g2, g3 });
//        }
//    }
//}
//
//// OCC网格质量评估
//TEST(cam_interference_detectTest, cam_interference_detectTest1) {
//    // 1. 创建边长为10的正方体
//    gp_Pnt origin(0.0, 0.0, 0.0);
//    gp_Pnt corner(10.0, 10.0, 10.0);
//    TopoDS_Solid box = BRepPrimAPI_MakeBox(origin, corner).Solid();
//
//    // 2. 计算网格化前的体积
//    GProp_GProps propsBefore;
//    BRepGProp::VolumeProperties(box, propsBefore, Precision::Confusion());
//    double volumeBefore = propsBefore.Mass();
//    std::cout << "Volume before meshing: " << volumeBefore << std::endl;
//
//    // 3. 执行网格化，线性偏差设为 0.1
//    BRepMesh_IncrementalMesh mesher(box, 0.1, Standard_False, 0.5, Standard_True);
//
//    // 4. 计算网格化后的体积（应保持不变）
//    GProp_GProps propsAfter;
//    BRepGProp::VolumeProperties(box, propsAfter, Precision::Confusion());
//    double volumeAfter = propsAfter.Mass();
//    std::cout << "Volume after meshing: " << volumeAfter << std::endl;
//
//    // 5. 输出差异
//    std::cout << "Volume difference: " << std::abs(volumeAfter - volumeBefore) << std::endl;
//
//
//    std::vector<Point3> outVertices;
//    std::vector<std::array<int, 3>> occTriangles;
//    extractMesh(box, outVertices, occTriangles);
//
//    // 每个元素是三个顶点坐标，构造 GeoBase 的 Triangle3
//    std::vector<Triangle3> geoTriangles;
//    for (const auto& occTri : occTriangles) {
//        geoTriangles.push_back(Triangle3(outVertices[occTri[0]], outVertices[occTri[1]], outVertices[occTri[2]]));
//    }
//
//    // 分析网格质量
//    MeshQualityStats stats = analyzeMeshQuality(geoTriangles);
//
//    // 打印结果
//    std::cout << "Triangle count: " << stats.triangleCount << std::endl;
//    std::cout << "Area range: " << stats.minArea << " ~ " << stats.maxArea << std::endl;
//    std::cout << "Average area: " << stats.avgArea << std::endl;
//    std::cout << "Angle range (deg): " << stats.minAngle * 180.0 / PI << " ~ " << stats.maxAngle * 180.0 / PI << std::endl;
//    std::cout << "Average angle (deg): " << stats.avgAngle * 180.0 / PI << std::endl;
//    std::cout << "Aspect ratio range: " << stats.minAspectRatio << " ~ " << stats.maxAspectRatio << std::endl;
//    std::cout << "Average aspect ratio: " << stats.avgAspectRatio << std::endl;
//
//}
//
