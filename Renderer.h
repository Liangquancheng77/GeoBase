#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "../thirdparty/glad/glad.h"
#include <GLFW/glfw3.h>

#include "../include/GeoBase/Vector3.h"
#include "../include/GeoBase/Matrix4.h"

static const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 uMVP;
void main() {
    gl_Position = uMVP * vec4(aPos, 1.0);
}
)";

static const char* fragmentShaderSource = R"(
#version 330 core
uniform vec3 uColor;
out vec4 FragColor;
void main() {
    FragColor = vec4(uColor, 1.0);
}
)";

unsigned int compileShader(const char* source, GLenum type);
unsigned int createShaderProgram();

class Renderer {
public:
    Renderer();
    ~Renderer();

    void drawLine(const Vector3& from, const Vector3& to, float r, float g, float b);
    void drawTriangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, float r, float g, float b);
    void setMVP(const Matrix4& mvp);

private:
    unsigned int shaderProgram;
    unsigned int vao, vbo;
};

#endif // RENDERER_H