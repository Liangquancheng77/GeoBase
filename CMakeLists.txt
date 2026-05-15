cmake_minimum_required(VERSION 3.20)
project(GeoBaseViewer)

# 改这一行，换成你电脑上vcpkg的实际路径
set(CMAKE_TOOLCHAIN_FILE "E:/GeometricAlgorithm2/GeometricAlgorithmmvcpkg/scripts/buildsystems/vcpkg.cmake")

find_package(glfw3 CONFIG REQUIRED)

# 把 main.cpp 和你的几何库源码一起编译
add_executable(viewer
    main.cpp
    ./Vector3.cpp      # 如果你的实现是.cpp文件
    ./Matrix3.cpp
    ./Matrix4.cpp
    ./Quaternion.cpp
    # ... 其他你用到的.cpp文件
)

target_include_directories(viewer PRIVATE ${CMAKE_SOURCE_DIR}/..)
target_link_libraries(viewer glfw)