//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <iostream>
//#include "Matrix4.h"  // 你的Matrix4头文件
//#include "Quaternion.h" // 你的Quaternion头文件
//
//// 窗口大小回调
//void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//    glViewport(0, 0, width, height);
//}
//
//int main() {
//    // 初始化GLFW
//    if (!glfwInit()) {
//        std::cerr << "GLFW init failed!" << std::endl;
//        return -1;
//    }
//
//    // OpenGL 3.3 核心模式
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    // 创建窗口
//    GLFWwindow* window = glfwCreateWindow(800, 600, "GeoBase Viewer", nullptr, nullptr);
//    if (!window) {
//        std::cerr << "Window create failed!" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//    // 初始化GLAD
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//        std::cerr << "GLAD init failed!" << std::endl;
//        return -1;
//    }
//
//    // 开启深度测试（画3D必备）
//    glEnable(GL_DEPTH_TEST);
//
//    // 渲染循环
//    while (!glfwWindowShouldClose(window)) {
//        // 清屏
//        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        // 后面的绘制/相机逻辑，都写在这里！
//
//        // 交换缓冲区，处理事件
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    glfwTerminate();
//    return 0;
//}
//
//
