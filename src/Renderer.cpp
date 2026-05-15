#include "../Renderer.h"
#include <iostream>

// 着色器编译函数实现
unsigned int compileShader(const char* source, GLenum type) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Shader compile error: " << infoLog << std::endl;
    }
    return shader;
}

// 程序链接函数实现（修正glGetProgramInfoLog参数）
unsigned int createShaderProgram() {
    unsigned int vs = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    unsigned int fs = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    unsigned int program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        // 关键修正：补全glGetProgramInfoLog的参数
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "Program link error: " << infoLog << std::endl;
    }
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

// Renderer类方法实现（保持不变）
Renderer::Renderer() {
    shaderProgram = createShaderProgram();
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // 设置默认MVP为单位矩阵，确保三角形能显示
    Matrix4 identity;
    identity.setIdentity();
    setMVP(identity);
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shaderProgram);
}

void Renderer::drawLine(const Vector3& from, const Vector3& to, float r, float g, float b) {
    glUseProgram(shaderProgram);
    glUniform3f(glGetUniformLocation(shaderProgram, "uColor"), r, g, b);

    float vertices[] = {
        (float)from.x, (float)from.y, (float)from.z,
        (float)to.x,   (float)to.y,   (float)to.z
    };
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_LINES, 0, 2);
}

void Renderer::drawTriangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, float r, float g, float b) {
    glUseProgram(shaderProgram);
    glUniform3f(glGetUniformLocation(shaderProgram, "uColor"), r, g, b);

    float vertices[] = {
        (float)v0.x, (float)v0.y, (float)v0.z,
        (float)v1.x, (float)v1.y, (float)v1.z,
        (float)v2.x, (float)v2.y, (float)v2.z
    };
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::setMVP(const Matrix4& mvp) {
    glUseProgram(shaderProgram);
    float mvpData[16];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            mvpData[i * 4 + j] = (float)mvp.m[j][i];
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uMVP"), 1, GL_FALSE, mvpData);
}