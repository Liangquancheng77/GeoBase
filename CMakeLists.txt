cmake_minimum_required(VERSION 3.16)
project(GeoBase)

# C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 解决中文编码警告
add_compile_options(/utf-8)

# 头文件路径
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/thirdparty
)

# ======================
# 通配符 * 自动匹配所有文件
# 不用手动写，新增文件自动识别
# ======================
file(GLOB ALL_FILES CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/include/GeoBase/*.h
    ${PROJECT_SOURCE_DIR}/thirdparty/glad/*.c
    ${PROJECT_SOURCE_DIR}/*.h
    ${PROJECT_SOURCE_DIR}/*.cpp
    ${PROJECT_SOURCE_DIR}/tests/*.cpp
)

add_executable(GeoBase ${ALL_FILES})

# [D01] RUN_TESTS 是 src/main.cpp:563 的测试入口开关，不定义就走可视化分支
target_compile_definitions(GeoBase PRIVATE RUN_TESTS)

# 链接GLFW
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(GeoBase PRIVATE glfw)

# 链接GTest（解决你刚才的链接报错！）
find_package(GTest CONFIG REQUIRED)
target_link_libraries(GeoBase PRIVATE GTest::gtest)
add_compile_options(/utf-8)