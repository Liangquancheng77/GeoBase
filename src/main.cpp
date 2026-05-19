#include "../include/GeoBase/Matrix4.h"
#include "../Viewer.h"
#include <gtest/gtest.h>
#include "../include/GeoBase/Plane.h"
#include "../include/GeoBase/AABB.h"
#include "../include/GeoBase/Triangle3.h"
#include <iostream>
#include <vector>
#include <limits>
#include <optional>  // 新增：用于std::optional，避免Triangle3默认构造问题
#include <cmath>


// ====================== 一键切换模式 ======================
#define RUN_TESTS        // 运行单元测试
 //#define RUN_VISUAL_CUBE  // 运行正方体可视化
// #define RUN_VISUAL_PLANE // 运行平面可视化
 //#define RUN_VISUAL_AABB // 运行AABB可视化
 //#define RUN_VISUAL_TRIANGLES // 运行三角形可视化
//#define RUN_VISUAL_AABB_RAY // AABB射线求交可视化
//#define RUN_VISUAL_PICK   // 运行三角形鼠标拾取
// ==========================================================

// 获取相机真实位置（轨道相机的eye坐标）
Point3 getCameraEye(const Viewer& viewer) {
    float theta = viewer.theta;
    float phi = viewer.phi;
    float radius = viewer.radius;
    Vector3 eye;
    eye.x = viewer.target.x + radius * sin(theta) * cos(phi);
    eye.y = viewer.target.y + radius * sin(phi);
    eye.z = viewer.target.z + radius * cos(theta) * cos(phi);
    return Point3(eye.x, eye.y, eye.z);
}

// Möller-Trumbore射线三角形求交（和Triangle3::intersect逻辑一致，可直接用）
bool intersectRayTriangle(const Ray& ray, const Triangle3& tri, double& t, double& u, double& v, bool cullBackface = false) {
    Vector3 E1 = tri.v1 - tri.v0;
    Vector3 E2 = tri.v2 - tri.v0;
    Vector3 T = ray.origin - tri.v0;
    Vector3 P = ray.direction.cross(E2);
    double denom = P.dot(E1);

    // 背面剔除/双面检测分支
    if (cullBackface) {
        if (denom < EPS_ABS) return false;
    }
    else {
        if (fabs(denom) < EPS_ABS) return false;
    }

    double invDenom = 1.0 / denom;
    u = P.dot(T) * invDenom;
    if (u < 0.0 || u > 1.0) return false;

    Vector3 Q = T.cross(E1);
    v = Q.dot(ray.direction) * invDenom;
    if (v < 0.0 || u + v > 1.0) return false;

    t = Q.dot(E2) * invDenom;
    return t > EPS_ABS;
}

// 三角形鼠标拾取主函数
void visualPick() {
    Viewer viewer(800, 600, "Triangle Picking - Click to Shoot Ray");

    // ---------------------- 定义场景三角形（正方体12个面） ----------------------
    std::vector<Triangle3> sceneTriangles;
    Point3 v[8] = {
        {-1, -1, -1}, // 0
        { 1, -1, -1}, // 1
        { 1,  1, -1}, // 2
        {-1,  1, -1}, // 3
        {-1, -1,  1}, // 4
        { 1, -1,  1}, // 5
        { 1,  1,  1}, // 6
        {-1,  1,  1}  // 7
    };

    // 正方体6个面，每个面2个三角形
    sceneTriangles.emplace_back(v[0], v[1], v[2]); // 前
    sceneTriangles.emplace_back(v[0], v[2], v[3]);
    sceneTriangles.emplace_back(v[4], v[6], v[5]); // 后
    sceneTriangles.emplace_back(v[4], v[7], v[6]);
    sceneTriangles.emplace_back(v[0], v[3], v[7]); // 左
    sceneTriangles.emplace_back(v[0], v[7], v[4]);
    sceneTriangles.emplace_back(v[1], v[5], v[6]); // 右
    sceneTriangles.emplace_back(v[1], v[6], v[2]);
    sceneTriangles.emplace_back(v[0], v[4], v[5]); // 底
    sceneTriangles.emplace_back(v[0], v[5], v[1]);
    sceneTriangles.emplace_back(v[3], v[2], v[6]); // 顶
    sceneTriangles.emplace_back(v[3], v[6], v[7]);

    // ---------------------- 状态变量 ----------------------
    Ray currentRay(Point3(0, 0, 0), Vector3(0, 0, -1));
    bool hasRay = false;
    bool wasMousePressed = false;

    // 最近命中信息（用std::optional避免Triangle3默认构造问题）
    std::optional<Triangle3> closestTri;
    double closestT = std::numeric_limits<double>::max(); // 初始化为最大值
    double closestU = 0.0, closestV = 0.0;
    Point3 closestHitPoint;
    bool hasClosestHit = false;

    while (!viewer.shouldClose()) {
        viewer.beginFrame();
        Matrix4 view = viewer.getViewMatrix();
        Matrix4 proj = Matrix4::createOrthographic(-5, 5, -5, 5, -1000, 1000);
        Matrix4 mvp = proj.multiply(view);
        viewer.setUniformMVP(mvp);

        // 绘制坐标轴
        viewer.drawLine({ 0,0,0 }, { 2,0,0 }, 1, 0, 0);
        viewer.drawLine({ 0,0,0 }, { 0,2,0 }, 0, 1, 0);
        viewer.drawLine({ 0,0,0 }, { 0,0,2 }, 0, 0, 1);

        // ---------------------- 鼠标点击生成射线 ----------------------
        bool isMousePressed = glfwGetMouseButton(viewer.getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
        if (isMousePressed && !wasMousePressed) {
            // 1. 获取相机位置
            Point3 cameraEye = getCameraEye(viewer);

            // 2. 鼠标坐标转NDC（简化版，也可以用viewer.screenToRay）
            double mouseX, mouseY;
            glfwGetCursorPos(viewer.getWindow(), &mouseX, &mouseY);
            float ndcX = (2.0f * mouseX) / 800 - 1.0f;
            float ndcY = 1.0f - (2.0f * mouseY) / 600;
            Vector3 dir = Vector3(ndcX * 0.5f, ndcY * 0.5f, -1.0f).normalized();

            // 3. 生成射线
            currentRay = Ray(cameraEye, dir);
            hasRay = true;

            // ---------------------- 遍历三角形找最近命中 ----------------------
            hasClosestHit = false;
            closestT = std::numeric_limits<double>::max(); // 重置为最大值
            closestTri.reset(); // 清空之前的命中
            for (const auto& tri : sceneTriangles) {
                double t, u, v;
                // 双面检测，不剔除背面
                if (intersectRayTriangle(currentRay, tri, t, u, v, false)) {
                    if (t < closestT) {
                        closestT = t;
                        closestU = u;
                        closestV = v;
                        closestTri = tri; // 赋值给std::optional
                        hasClosestHit = true;
                    }
                }
            }

            // ---------------------- 打印命中信息 ----------------------
            std::cout << "\n=== Ray Generated ===" << std::endl;
            std::cout << "Camera Pos: (" << cameraEye.x << ", " << cameraEye.y << ", " << cameraEye.z << ")" << std::endl;
            std::cout << "Ray Dir: (" << dir.x << ", " << dir.y << ", " << dir.z << ")" << std::endl;
            if (hasClosestHit && closestTri.has_value()) {
                closestHitPoint = currentRay.pointAt(closestT);
                std::cout << "Hit Triangle! t = " << closestT << ", u = " << closestU << ", v = " << closestV << std::endl;
                std::cout << "Hit Point: (" << closestHitPoint.x << ", " << closestHitPoint.y << ", " << closestHitPoint.z << ")" << std::endl;
            }
            else {
                std::cout << "No hit" << std::endl;
            }
        }
        wasMousePressed = isMousePressed;

        // ---------------------- 绘制场景 ----------------------
        // 1. 所有三角形（灰色线框）
        for (const auto& tri : sceneTriangles) {
            viewer.drawLine(tri.v0, tri.v1, 0.5f, 0.5f, 0.5f);
            viewer.drawLine(tri.v1, tri.v2, 0.5f, 0.5f, 0.5f);
            viewer.drawLine(tri.v2, tri.v0, 0.5f, 0.5f, 0.5f);
        }

        // 2. 绘制射线（蓝色）
        if (hasRay) {
            Point3 rayEnd = currentRay.origin + currentRay.direction * 10.0;
            viewer.drawLine(currentRay.origin, rayEnd, 0, 0, 1);

            // 3. 高亮最近命中的三角形（白色线框）+ 红色交点
            if (hasClosestHit && closestTri.has_value()) {
                const Triangle3& hitTri = closestTri.value();
                viewer.drawLine(hitTri.v0, hitTri.v1, 1, 1, 1);
                viewer.drawLine(hitTri.v1, hitTri.v2, 1, 1, 1);
                viewer.drawLine(hitTri.v2, hitTri.v0, 1, 1, 1);
                viewer.drawPoint(closestHitPoint, 1, 0, 0, 0.15f);
            }
        }

        viewer.endFrame();
    }
}

// 点击生成射线函数（原AABB求交，保留）
void visualAABBRay() {
    Viewer viewer(800, 600, "AABB Ray Intersection - Click to Shoot");
    AABB box(Point3(-1, -1, -1), Point3(1, 1, 1));

    Ray currentRay(Point3(0, 0, 0), Vector3(0, 0, -1));
    bool hasRay = false;
    bool hit = false;
    double tMin = 0, tMax = 0;
    Point3 hitPoint;
    bool wasMousePressed = false;

    while (!viewer.shouldClose()) {
        viewer.beginFrame();
        Matrix4 view = viewer.getViewMatrix();
        Matrix4 proj = Matrix4::createOrthographic(-5, 5, -5, 5, -1000, 1000);
        Matrix4 mvp = proj.multiply(view);
        viewer.setUniformMVP(mvp);

        // 绘制世界坐标轴
        viewer.drawLine({ 0,0,0 }, { 2,0,0 }, 1, 0, 0);
        viewer.drawLine({ 0,0,0 }, { 0,2,0 }, 0, 1, 0);
        viewer.drawLine({ 0,0,0 }, { 0,0,2 }, 0, 0, 1);

        // 绘制白色AABB包围盒线框
        viewer.drawAABB(box.min, box.max, 1, 1, 1);

        // 鼠标左键按下瞬间生成射线，避免持续触发
        bool isMousePressed = glfwGetMouseButton(viewer.getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
        if (isMousePressed && !wasMousePressed) {
            // 1. 获取当前相机世界坐标
            Point3 cameraEye = getCameraEye(viewer);

            // 2. 获取鼠标屏幕坐标
            double mouseX, mouseY;
            glfwGetCursorPos(viewer.getWindow(), &mouseX, &mouseY);

            // 3. 屏幕坐标转简易NDC坐标
            float ndcX = (2.0f * mouseX) / 800 - 1.0f;
            float ndcY = 1.0f - (2.0f * mouseY) / 600;

            // 生成归一化射线方向
            Vector3 dir = Vector3(ndcX * 0.5f, ndcY * 0.5f, -1.0f).normalized();

            // 赋值当前射线
            currentRay = Ray(cameraEye, dir);
            hasRay = true;

            // 执行射线与AABB求交
            hit = box.intersect(currentRay, tMin, tMax);
            if (hit) {
                hitPoint = currentRay.pointAt(tMin);
                std::cout << "\n=== Ray Generated Successfully ===" << std::endl;
                std::cout << "Camera Position: (" << cameraEye.x << ", " << cameraEye.y << ", " << cameraEye.z << ")" << std::endl;
                std::cout << "Ray Direction: (" << dir.x << ", " << dir.y << ", " << dir.z << ")" << std::endl;
                std::cout << "Hit AABB! tMin: " << tMin << ", tMax: " << tMax << std::endl;
            }
            else {
                std::cout << "\n=== Ray Generated Successfully ===" << std::endl;
                std::cout << "Camera Position: (" << cameraEye.x << ", " << cameraEye.y << ", " << cameraEye.z << ")" << std::endl;
                std::cout << "No Hit" << std::endl;
            }
        }
        wasMousePressed = isMousePressed;

        // 绘制生成的蓝色射线
        if (hasRay) {
            Point3 rayEnd = currentRay.origin + currentRay.direction * 10.0;
            viewer.drawLine(currentRay.origin, rayEnd, 0, 0, 1);

            // 命中后绘制相交线段与交点
            if (hit) {
                Point3 exitPoint = currentRay.pointAt(tMax);
                viewer.drawLine(hitPoint, exitPoint, 1, 1, 0);
                viewer.drawPoint(hitPoint, 0, 1, 0, 0.15f);
            }
        }

        viewer.endFrame();
    }
}

// 其他函数（cube/pointPlane等，保留不变）
void pointTriangle3() {
    Viewer viewer(800, 600, "Colored Triangle Visualization");
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

void getPlaneVectors(const Vector3& normal, Vector3& right, Vector3& up)
{
    Vector3 ref = fabs(normal.y) < 0.999 ? Vector3(0, 1, 0) : Vector3(1, 0, 0);
    right = normal.cross(ref).normalized();
    up = right.cross(normal).normalized();
}

void pointPlane() {
    Viewer viewer(800, 600, "Plane Visualization + Orbit Camera");
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

void cube() {
    Viewer viewer(800, 600, "Cube Visualization - Orthographic Projection");
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

void pointAABB() {
    Viewer viewer(800, 600, "AABB Bounding Box Visualization - Orthographic Projection");
    AABB box(Point3(-1, -1, -1), Point3(1, 1, 1));
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

// 唯一的主函数（已添加RUN_VISUAL_PICK分支）
int main(int argc, char** argv) {
#ifdef RUN_TESTS
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#elif defined(RUN_VISUAL_CUBE)
    cube();
    return 0;
#elif defined(RUN_VISUAL_PLANE)
    pointPlane();
    return 0;
#elif defined(RUN_VISUAL_AABB)
    pointAABB();
    return 0;
#elif defined(RUN_VISUAL_TRIANGLES)
    pointTriangle3();
    return 0;
#elif defined(RUN_VISUAL_AABB_RAY)
    visualAABBRay();
    return 0;
#elif defined(RUN_VISUAL_PICK) // 新增：三角形拾取模式分支
    visualPick();
    return 0;
#else
    return 0;
#endif
}
