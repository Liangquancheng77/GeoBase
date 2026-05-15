#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "./include/GeoBase/Vector3.h"
#include "./include/GeoBase/Matrix4.h"

class Viewer {
public:
    Viewer(int width, int height, const char* title);
    ~Viewer();

    bool shouldClose() const;
    void beginFrame();
    void endFrame();

    void drawLine(const Vector3& from, const Vector3& to, float r, float g, float b);
    void drawTriangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, float r, float g, float b);
    void drawPoint(const Vector3& pos, float r, float g, float b, float size = 0.1f);
    void drawAABB(const Vector3& min, const Vector3& max, float r, float g, float b);

    // ✅ 对外：获取相机的视图矩阵（直接用！）
    Matrix4 getViewMatrix();

    // 内部鼠标回调（不用管）
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
    GLFWwindow* window;
    unsigned int shaderProgram;
    unsigned int vao;
    unsigned int vbo;

    // ✅ 轨道相机核心参数
    float theta = 0.0f;    // 水平旋转角
    float phi = 0.6f;      // 垂直俯仰角
    float radius = 3.0f;   // 相机距离中心点的距离
    Vector3 target{ 0,0,0 }; // 相机盯着的点

    // 鼠标状态
    bool isDragging = false;
    double lastX = 0, lastY = 0;

    // 内部方法
    void setUniformColor(float r, float g, float b);
public:
    void setUniformMVP(const Matrix4& mvp);
private:
    void uploadVertices(const float* data, int count);
};