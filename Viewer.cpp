#include "Viewer.h"
#include <iostream>
#include <cmath>

static const char* vertexSrc = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 uMVP;
void main() { gl_Position = uMVP * vec4(aPos, 1.0); }
)";

static const char* fragmentSrc = R"(
#version 330 core
uniform vec3 uColor;
out vec4 FragColor;
void main() { FragColor = vec4(uColor, 1.0); }
)";

Viewer::Viewer(int width, int height, const char* title) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glDisable(GL_DEPTH_TEST);

    // 着色器
    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexSrc, nullptr);
    glCompileShader(vs);
    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentSrc, nullptr);
    glCompileShader(fs);
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);
    glDeleteShader(vs);
    glDeleteShader(fs);

    // VAO/VBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // ✅ 绑定鼠标回调（核心！）
    glfwSetWindowUserPointer(window, this);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);
}

Viewer::~Viewer() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

bool Viewer::shouldClose() const { return glfwWindowShouldClose(window); }
void Viewer::beginFrame() { glClear(GL_COLOR_BUFFER_BIT); }
void Viewer::endFrame() { glfwSwapBuffers(window); glfwPollEvents(); }

// ✅ 轨道相机：计算视图矩阵
Matrix4 Viewer::getViewMatrix() {
    Vector3 eye;
    eye.x = target.x + radius * sin(theta) * cos(phi);
    eye.y = target.y + radius * sin(phi);
    eye.z = target.z + radius * cos(theta) * cos(phi);
    return Matrix4::createLookAt(eye, target, Vector3(0, 1, 0));
}

// ✅ 鼠标按键回调
void Viewer::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Viewer* viewer = (Viewer*)glfwGetWindowUserPointer(window);
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        viewer->isDragging = (action == GLFW_PRESS);
        glfwGetCursorPos(window, &viewer->lastX, &viewer->lastY);
    }
}

// ✅ 鼠标移动回调（拖拽旋转）
void Viewer::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    Viewer* viewer = (Viewer*)glfwGetWindowUserPointer(window);
    if (!viewer->isDragging) return;

    float dx = xpos - viewer->lastX;
    float dy = ypos - viewer->lastY;
    viewer->theta -= dx * 0.005f;
    viewer->phi += dy * 0.005f;

    // 限制俯仰角，防止翻转
    if (viewer->phi > 1.4) viewer->phi = 1.4;
    if (viewer->phi < -1.4) viewer->phi = -1.4;

    viewer->lastX = xpos;
    viewer->lastY = ypos;
}

// ✅ 滚轮缩放
void Viewer::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Viewer* viewer = (Viewer*)glfwGetWindowUserPointer(window);
    viewer->radius -= yoffset * 0.3f;
    if (viewer->radius < 0.5f) viewer->radius = 0.5f;
}

// 绘制方法（不变）
void Viewer::drawLine(const Vector3& from, const Vector3& to, float r, float g, float b) {
    setUniformColor(r, g, b);
    float verts[] = { (float)from.x,(float)from.y,(float)from.z, (float)to.x,(float)to.y,(float)to.z };
    uploadVertices(verts, 2);
    glDrawArrays(GL_LINES, 0, 2);
}
void Viewer::drawTriangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, float r, float g, float b) {
    setUniformColor(r, g, b);
    float verts[] = { (float)v0.x,(float)v0.y,(float)v0.z, (float)v1.x,(float)v1.y,(float)v1.z, (float)v2.x,(float)v2.y,(float)v2.z };
    uploadVertices(verts, 3);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
void Viewer::setUniformColor(float r, float g, float b) {
    glUseProgram(shaderProgram);
    glUniform3f(glGetUniformLocation(shaderProgram, "uColor"), r, g, b);
}
void Viewer::setUniformMVP(const Matrix4& mvp) {
    glUseProgram(shaderProgram);
    float data[16];
    for (int i = 0;i < 4;i++)for (int j = 0;j < 4;j++)data[i * 4 + j] = mvp.m[j][i];
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uMVP"), 1, GL_FALSE, data);
}
void Viewer::uploadVertices(const float* data, int count) {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, count * 3 * sizeof(float), data, GL_STREAM_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
}
void Viewer::drawPoint(const Vector3& pos, float r, float g, float b, float size) {}
void Viewer::drawAABB(const Vector3& min, const Vector3& max, float r, float g, float b) {}