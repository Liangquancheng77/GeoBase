#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "./include/GeoBase/Vector3.h"
#include "./include/GeoBase/Matrix4.h"
#include "./include/GeoBase/Ray.h" // 必须包含Ray类

class Viewer {
public:
    // 构造函数：必须传入宽高
    Viewer(int width, int height, const char* title);
    ~Viewer();

    bool shouldClose() const;
    void beginFrame();
    void endFrame();

    // 绘图方法
    void drawLine(const Vector3& from, const Vector3& to, float r, float g, float b);
    void drawTriangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, float r, float g, float b);
    void drawPoint(const Vector3& pos, float r, float g, float b, float size = 0.1f);
    void drawAABB(const Vector3& min, const Vector3& max, float r, float g, float b);

    // 相机与射线
    Matrix4 getViewMatrix();
    Ray screenToRay(double mouseX, double mouseY, const Matrix4& invMVP) const;

    // 鼠标点击记录
    double getLastClickX() const { return lastClickX; }
    double getLastClickY() const { return lastClickY; }

    // 新增：获取窗口句柄（解决私有访问问题）
    GLFWwindow* getWindow() const { return window; }

    // 鼠标回调
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

public:
    GLFWwindow* window;
    unsigned int shaderProgram;
    unsigned int vao;
    unsigned int vbo;

    int width, height;
    float theta = 0.0f;
    float phi = 0.6f;
    float radius = 3.0f;
    Vector3 target{ 0,0,0 };

    bool isDragging = false;
    double lastX = 0, lastY = 0;
    double lastClickX = 0, lastClickY = 0;

    void setUniformColor(float r, float g, float b);
public:
    void setUniformMVP(const Matrix4& mvp);
private:
    void uploadVertices(const float* data, int count);
};