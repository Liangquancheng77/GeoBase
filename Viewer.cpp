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

Viewer::Viewer(int width, int height, const char* title)
    : width(width), height(height) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glDisable(GL_DEPTH_TEST);

    // 着色器编译
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

Matrix4 Viewer::getViewMatrix() {
    Vector3 eye;
    eye.x = target.x + radius * sin(theta) * cos(phi);
    eye.y = target.y + radius * sin(phi);
    eye.z = target.z + radius * cos(theta) * cos(phi);
    return Matrix4::createLookAt(eye, target, Vector3(0, 1, 0));
}

void Viewer::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Viewer* viewer = (Viewer*)glfwGetWindowUserPointer(window);
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            viewer->isDragging = true;
            glfwGetCursorPos(window, &viewer->lastX, &viewer->lastY);
            viewer->lastClickX = viewer->lastX;
            viewer->lastClickY = viewer->lastY;
        }
        else {
            viewer->isDragging = false;
        }
    }
}

void Viewer::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    Viewer* viewer = (Viewer*)glfwGetWindowUserPointer(window);
    if (!viewer->isDragging) return;

    float dx = xpos - viewer->lastX;
    float dy = ypos - viewer->lastY;
    viewer->theta -= dx * 0.005f;
    viewer->phi += dy * 0.005f;

    if (viewer->phi > 1.4) viewer->phi = 1.4;
    if (viewer->phi < -1.4) viewer->phi = -1.4;

    viewer->lastX = xpos;
    viewer->lastY = ypos;
}

void Viewer::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Viewer* viewer = (Viewer*)glfwGetWindowUserPointer(window);
    viewer->radius -= yoffset * 0.3f;
    if (viewer->radius < 0.5f) viewer->radius = 0.5f;
}

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

// 画点（十字线，更清晰）
void Viewer::drawPoint(const Vector3& pos, float r, float g, float b, float size) {
    drawLine(pos - Vector3(size, 0, 0), pos + Vector3(size, 0, 0), r, g, b);
    drawLine(pos - Vector3(0, size, 0), pos + Vector3(0, size, 0), r, g, b);
    drawLine(pos - Vector3(0, 0, size), pos + Vector3(0, 0, size), r, g, b);
}

// 画AABB线框（必须实现，否则会报C2660）
void Viewer::drawAABB(const Vector3& min, const Vector3& max, float r, float g, float b) {
    Vector3 v[8] = {
        {min.x, min.y, min.z}, {max.x, min.y, min.z},
        {max.x, max.y, min.z}, {min.x, max.y, min.z},
        {min.x, min.y, max.z}, {max.x, min.y, max.z},
        {max.x, max.y, max.z}, {min.x, max.y, max.z},
    };
    // 底面
    drawLine(v[0], v[1], r, g, b); drawLine(v[1], v[2], r, g, b);
    drawLine(v[2], v[3], r, g, b); drawLine(v[3], v[0], r, g, b);
    // 顶面
    drawLine(v[4], v[5], r, g, b); drawLine(v[5], v[6], r, g, b);
    drawLine(v[6], v[7], r, g, b); drawLine(v[7], v[4], r, g, b);
    // 竖边
    drawLine(v[0], v[4], r, g, b); drawLine(v[1], v[5], r, g, b);
    drawLine(v[2], v[6], r, g, b); drawLine(v[3], v[7], r, g, b);
}

Ray Viewer::screenToRay(double mouseX, double mouseY, const Matrix4& invMVP) const {
    float ndcX = (2.0f * mouseX) / width - 1.0f;
    float ndcY = 1.0f - (2.0f * mouseY) / height;

    Point3 nearNDC(ndcX, ndcY, -1.0f);
    Point3 farNDC(ndcX, ndcY, 1.0f);

    Point3 worldNear = invMVP.transformPoint(nearNDC);
    Point3 worldFar = invMVP.transformPoint(farNDC);

    Vector3 dir = worldFar - worldNear;

    // 防零向量：如果方向几乎为零，给个默认向前的方向
    if (dir.length() < 1e-9) {
        dir = Vector3(0, 0, -1);
    }

    dir = dir.normalized();
    return Ray(worldNear, dir);
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