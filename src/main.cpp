#include "../include/GeoBase/Matrix4.h"
#include "../Viewer.h"
#include<gtest/gtest.h>
#include"../include/GeoBase/Plane.h"

//int main() {
//    Viewer viewer(800, 600, "轨道相机 - 拖拽旋转");
//
//    while (!viewer.shouldClose()) {
//        viewer.beginFrame();
//
//        // ✅ 1. 获取相机矩阵 + 正交投影（大视口，不裁剪）
//        Matrix4 view = viewer.getViewMatrix();
//        Matrix4 proj = Matrix4::createOrthographic(-3, 3, -3, 3, -10, 10);
//        Matrix4 mvp = proj.multiply(view);
//        viewer.setUniformMVP(mvp);
//
//        // ✅ 2. 随便画图形
//        viewer.drawLine({ 0,0,0 }, { 2,0,0 }, 1, 0, 0);
//        viewer.drawLine({ 0,0,0 }, { 0,2,0 }, 0, 1, 0);
//        viewer.drawLine({ 0,0,0 }, { 0,0,2 }, 0, 0, 1);
//        viewer.drawTriangle({ 0,0,0 }, { 1,0,0 }, { 0,1,0 }, 1, 0, 1);
//
//        viewer.endFrame();
//    }
//    return 0;
//}

//int main(int argc, char** argv) {
//    testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
//}

// 工具函数：根据平面法向量生成正交方向向量（用于画平面四边形）
void getPlaneVectors(const Vector3& normal, Vector3& right, Vector3& up)
{
    Vector3 ref = fabs(normal.y) < 0.999 ? Vector3(0, 1, 0) : Vector3(1, 0, 0);
    right = normal.cross(ref).normalized();
    up = right.cross(normal).normalized();
}

// 画点平面
void pointPlane() {
    Viewer viewer(800, 600, "平面可视化 + 轨道相机");

    // ====================== 1. 创建你的平面（和单元测试一致：y=0平面）
    Plane plane(Vector3(0, 1, 0), Vector3(0, 0, 0));
    const float PLANE_SIZE = 1.5f; // 平面大小

    // 生成平面四个顶点
    Vector3 u, v;
    getPlaneVectors(plane.normal, u, v);
    Vector3 p0 = plane.origin - u * PLANE_SIZE - v * PLANE_SIZE;
    Vector3 p1 = plane.origin + u * PLANE_SIZE - v * PLANE_SIZE;
    Vector3 p2 = plane.origin + u * PLANE_SIZE + v * PLANE_SIZE;
    Vector3 p3 = plane.origin - u * PLANE_SIZE + v * PLANE_SIZE;

    // 测试点 + 投影点（验证你的函数）
    Vector3 testPoint(1, 2, 1);
    Vector3 projPoint = plane.projectPoint(testPoint);

    // ====================== 渲染循环
    while (!viewer.shouldClose()) {
        viewer.beginFrame();

        // 相机矩阵
        Matrix4 view = viewer.getViewMatrix();
        Matrix4 proj = Matrix4::createOrthographic(-3, 3, -3, 3, -10, 10);
        Matrix4 mvp = proj.multiply(view);
        viewer.setUniformMVP(mvp);

        // 1. 画坐标轴（红X/绿Y/蓝Z）
        viewer.drawLine({ 0,0,0 }, { 2,0,0 }, 1, 0, 0);
        viewer.drawLine({ 0,0,0 }, { 0,2,0 }, 0, 1, 0);
        viewer.drawLine({ 0,0,0 }, { 0,0,2 }, 0, 0, 1);

        // 2. 画平面（浅蓝色，两个三角形拼成四边形）
        viewer.drawTriangle(p0, p1, p2, 0.3f, 0.7f, 1.0f);
        viewer.drawTriangle(p0, p2, p3, 0.3f, 0.7f, 1.0f);

        // 3. 画平面法线（红色，从平面原点指向法线方向）
        viewer.drawLine(plane.origin, plane.origin + plane.normal * 1.0f, 1, 0, 0);

        // 4. 画测试点（绿色）+ 投影点（黄色）
        viewer.drawLine(testPoint, testPoint, 0, 1, 0); // 点=线段重合
        viewer.drawLine(projPoint, projPoint, 1, 1, 0);

        // 5. 画测试点到投影点的连线（白色）
        viewer.drawLine(testPoint, projPoint, 1, 1, 1);

        viewer.endFrame();
    }
}

// 正方体
void cube() {
    Viewer viewer(800, 600, "正方体可视化 - 正交投影");

    // ====================== 1. 正方体参数（你可以随便改）
    Vector3 cubeCenter(0, 0, 0);  // 正方体中心在原点
    float cubeSize = 1.0f;        // 正方体边长（改大改小都可以）
    float half = cubeSize / 2.0f;  // 半边长，方便计算顶点

    // ====================== 2. 生成正方体 8 个顶点（标准立方体）
    Vector3 v[8] = {
        // 下层4个点 (z - half)
        {cubeCenter.x - half, cubeCenter.y - half, cubeCenter.z - half},
        {cubeCenter.x + half, cubeCenter.y - half, cubeCenter.z - half},
        {cubeCenter.x + half, cubeCenter.y + half, cubeCenter.z - half},
        {cubeCenter.x - half, cubeCenter.y + half, cubeCenter.z - half},
        // 上层4个点 (z + half)
        {cubeCenter.x - half, cubeCenter.y - half, cubeCenter.z + half},
        {cubeCenter.x + half, cubeCenter.y - half, cubeCenter.z + half},
        {cubeCenter.x + half, cubeCenter.y + half, cubeCenter.z + half},
        {cubeCenter.x - half, cubeCenter.y + half, cubeCenter.z + half},
    };

    // ====================== 渲染循环
    while (!viewer.shouldClose()) {
        viewer.beginFrame();

        // 相机 + 正交投影（你原来的代码，完全不变）
        Matrix4 view = viewer.getViewMatrix();
        Matrix4 proj = Matrix4::createOrthographic(-3, 3, -3, 3, -1000, 1000);
        Matrix4 mvp = proj.multiply(view);
        viewer.setUniformMVP(mvp);

        // 1. 画坐标轴（保留，方便看方向）
        viewer.drawLine({ 0,0,0 }, { 2,0,0 }, 1, 0, 0);
        viewer.drawLine({ 0,0,0 }, { 0,2,0 }, 0, 1, 0);
        viewer.drawLine({ 0,0,0 }, { 0,0,2 }, 0, 0, 1);

        // ====================== 3. 画正方体线框（12条棱，白色）
        // 底面 4条
        viewer.drawLine(v[0], v[1], 1, 1, 1);
        viewer.drawLine(v[1], v[2], 1, 1, 1);
        viewer.drawLine(v[2], v[3], 1, 1, 1);
        viewer.drawLine(v[3], v[0], 1, 1, 1);
        // 顶面 4条
        viewer.drawLine(v[4], v[5], 1, 1, 1);
        viewer.drawLine(v[5], v[6], 1, 1, 1);
        viewer.drawLine(v[6], v[7], 1, 1, 1);
        viewer.drawLine(v[7], v[4], 1, 1, 1);
        // 竖边 4条
        viewer.drawLine(v[0], v[4], 1, 1, 1);
        viewer.drawLine(v[1], v[5], 1, 1, 1);
        viewer.drawLine(v[2], v[6], 1, 1, 1);
        viewer.drawLine(v[3], v[7], 1, 1, 1);

        viewer.endFrame();
    }
}

int main() {

    //pointPlane();
    cube();

    return 0;
}