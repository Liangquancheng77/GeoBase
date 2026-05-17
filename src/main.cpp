#include "../include/GeoBase/Matrix4.h"
#include "../Viewer.h"
#include <gtest/gtest.h>
#include "../include/GeoBase/Plane.h"
#include "../include/GeoBase/AABB.h"
#include "../include/GeoBase/Triangle3.h"

// ====================== 一键切换模式 ======================
//#define RUN_TESTS        // 运行单元测试
 //#define RUN_VISUAL_CUBE  // 运行正方体可视化
// #define RUN_VISUAL_PLANE // 运行平面可视化
 //#define RUN_VISUAL_AABB // 运行AABB可视化
 #define RUN_VISUAL_TRIANGLES // 运行三角形可视化
// ==========================================================



// 画彩色三角形
void pointTriangle3() {
    Viewer viewer(800, 600, "彩色三角形可视化 - 正交投影");
    Triangle3 tri(Point3(0, 0, 0), Point3(1, 0, 0), Point3(1, 1, 1));

    while (!viewer.shouldClose()) {
        viewer.beginFrame();
        Matrix4 view = viewer.getViewMatrix();
        Matrix4 proj = Matrix4::createOrthographic(-3, 3, -3, 3, -1000, 1000);
        Matrix4 mvp = proj.multiply(view);
        viewer.setUniformMVP(mvp);

        viewer.drawLine({ 0,0,0 }, { 2,0,0 }, 1, 0, 0);
        viewer.drawLine({ 0,0,0 }, { 0,2,0 }, 0, 1, 0);
        viewer.drawLine({ 0,0,0 }, { 0,0,2 }, 0, 0, 1);

        viewer.drawLine(tri.v0, tri.v1, 1, 0, 0);
        viewer.drawLine(tri.v0, tri.v2, 0, 1, 0);
        viewer.drawLine(tri.v1, tri.v2, 0, 0, 1);

        viewer.endFrame();
    }

}

// 工具函数：根据平面法向量生成正交方向向量
void getPlaneVectors(const Vector3& normal, Vector3& right, Vector3& up)
{
    Vector3 ref = fabs(normal.y) < 0.999 ? Vector3(0, 1, 0) : Vector3(1, 0, 0);
    right = normal.cross(ref).normalized();
    up = right.cross(normal).normalized();
}
// 画点平面
void pointPlane() {
    Viewer viewer(800, 600, "平面可视化 + 轨道相机");
    Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));
    const float PLANE_SIZE = 1.5f;

    Vector3 u, v;
    getPlaneVectors(plane.normal, u, v);
    Vector3 p0 = plane.origin - u * PLANE_SIZE - v * PLANE_SIZE;
    Vector3 p1 = plane.origin + u * PLANE_SIZE - v * PLANE_SIZE;
    Vector3 p2 = plane.origin + u * PLANE_SIZE + v * PLANE_SIZE;
    Vector3 p3 = plane.origin - u * PLANE_SIZE + v * PLANE_SIZE;

    Vector3 testPoint(1, 2, 1);
    Vector3 projPoint = plane.projectPoint(testPoint);

    while (!viewer.shouldClose()) {
        viewer.beginFrame();
        Matrix4 view = viewer.getViewMatrix();
        Matrix4 proj = Matrix4::createOrthographic(-3, 3, -3, 3, -10, 10);
        Matrix4 mvp = proj.multiply(view);
        viewer.setUniformMVP(mvp);

        viewer.drawLine({ 0,0,0 }, { 2,0,0 }, 1, 0, 0);
        viewer.drawLine({ 0,0,0 }, { 0,2,0 }, 0, 1, 0);
        viewer.drawLine({ 0,0,0 }, { 0,0,2 }, 0, 0, 1);

        viewer.drawTriangle(p0, p1, p2, 0.3f, 0.7f, 1.0f);
        viewer.drawTriangle(p0, p2, p3, 0.3f, 0.7f, 1.0f);
        viewer.drawLine(plane.origin, plane.origin + plane.normal * 1.0f, 1, 0, 0);

        viewer.drawLine(testPoint, testPoint, 0, 1, 0);
        viewer.drawLine(projPoint, projPoint, 1, 1, 0);
        viewer.drawLine(testPoint, projPoint, 1, 1, 1);

        viewer.endFrame();
    }
}



// 正方体
void cube() {
    Viewer viewer(800, 600, "正方体可视化 - 正交投影");
    Vector3 cubeCenter(0, 0, 0);
    float cubeSize = 1.0f;
    float half = cubeSize / 2.0f;

    Vector3 v[8] = {
        {cubeCenter.x - half, cubeCenter.y - half, cubeCenter.z - half},
        {cubeCenter.x + half, cubeCenter.y - half, cubeCenter.z - half},
        {cubeCenter.x + half, cubeCenter.y + half, cubeCenter.z - half},
        {cubeCenter.x - half, cubeCenter.y + half, cubeCenter.z - half},
        {cubeCenter.x - half, cubeCenter.y - half, cubeCenter.z + half},
        {cubeCenter.x + half, cubeCenter.y - half, cubeCenter.z + half},
        {cubeCenter.x + half, cubeCenter.y + half, cubeCenter.z + half},
        {cubeCenter.x - half, cubeCenter.y + half, cubeCenter.z + half},
    };

    while (!viewer.shouldClose()) {
        viewer.beginFrame();
        Matrix4 view = viewer.getViewMatrix();
        Matrix4 proj = Matrix4::createOrthographic(-3, 3, -3, 3, -1000, 1000);
        Matrix4 mvp = proj.multiply(view);
        viewer.setUniformMVP(mvp);

        viewer.drawLine({ 0,0,0 }, { 2,0,0 }, 1, 0, 0);
        viewer.drawLine({ 0,0,0 }, { 0,2,0 }, 0, 1, 0);
        viewer.drawLine({ 0,0,0 }, { 0,0,2 }, 0, 0, 1);

        viewer.drawLine(v[0], v[1], 1, 1, 1);
        viewer.drawLine(v[1], v[2], 1, 1, 1);
        viewer.drawLine(v[2], v[3], 1, 1, 1);
        viewer.drawLine(v[3], v[0], 1, 1, 1);
        viewer.drawLine(v[4], v[5], 1, 1, 1);
        viewer.drawLine(v[5], v[6], 1, 1, 1);
        viewer.drawLine(v[6], v[7], 1, 1, 1);
        viewer.drawLine(v[7], v[4], 1, 1, 1);
        viewer.drawLine(v[0], v[4], 1, 1, 1);
        viewer.drawLine(v[1], v[5], 1, 1, 1);
        viewer.drawLine(v[2], v[6], 1, 1, 1);
        viewer.drawLine(v[3], v[7], 1, 1, 1);

        viewer.endFrame();
    }
}

// 画AABB
void pointAABB() {
    Viewer viewer(800, 600, "AABB包围盒可视化 - 正交投影");
    AABB box(Point3(-1,-1,-1), Point3(1,1,1));
    Point3 p1(box.min.x, box.min.y, box.min.z);
    Point3 p2(box.min.x, box.max.y, box.min.z);
    Point3 p3(box.max.x, box.min.y, box.min.z);
    Point3 p4(box.max.x, box.max.y, box.min.z);
    Point3 p5(box.min.x, box.min.y, box.max.z);
    Point3 p6(box.min.x, box.max.y, box.max.z);
    Point3 p7(box.max.x, box.min.y, box.max.z);
    Point3 p8(box.max.x, box.max.y, box.max.z);

    while (!viewer.shouldClose()) {
        viewer.beginFrame();
        Matrix4 view = viewer.getViewMatrix();
        Matrix4 proj = Matrix4::createOrthographic(-3, 3, -3, 3, -1000, 1000);
        Matrix4 mvp = proj.multiply(view);
        viewer.setUniformMVP(mvp);

        viewer.drawLine({ 0,0,0 }, { 2,0,0 }, 1, 0, 0);
        viewer.drawLine({ 0,0,0 }, { 0,2,0 }, 0, 1, 0);
        viewer.drawLine({ 0,0,0 }, { 0,0,2 }, 0, 0, 1);

        viewer.drawLine(p1, p2, 1, 1, 1);
        viewer.drawLine(p1, p3, 1, 1, 1);
        viewer.drawLine(p2, p4, 1, 1, 1);
        viewer.drawLine(p3, p4, 1, 1, 1);
        viewer.drawLine(p5, p6, 1, 1, 1);
        viewer.drawLine(p5, p7, 1, 1, 1);
        viewer.drawLine(p6, p8, 1, 1, 1);
        viewer.drawLine(p7, p8, 1, 1, 1);
        viewer.drawLine(p1, p5, 1, 1, 1);
        viewer.drawLine(p2, p6, 1, 1, 1);
        viewer.drawLine(p3, p7, 1, 1, 1);
        viewer.drawLine(p4, p8, 1, 1, 1);

        viewer.endFrame();
    }

}

// 唯一的主函数
int main(int argc, char** argv) {
#ifdef RUN_TESTS
    // 运行单元测试
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#elif defined(RUN_VISUAL_CUBE)
    // 运行正方体可视化
    cube();
    return 0;
#elif defined(RUN_VISUAL_PLANE)
    // 运行平面可视化
    pointPlane();
    return 0;
#elif defined(RUN_VISUAL_AABB)
    // 运行AABB可视化
    pointAABB();
    return 0;
#elif defined(RUN_VISUAL_TRIANGLES)
    // 运行三角形可视化
    pointTriangle3();
    return 0;
#else
    return 0;
#endif
}