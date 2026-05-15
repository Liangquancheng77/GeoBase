#include<gtest/gtest.h>
#include "../include/GeoBase/Vector3.h"

// 测试向量加法运算
TEST(Vector3Test, Addition) {
    // 定义两个测试向量 a 和 b
    const Vector3 a(1, 2, 3);
    const Vector3 b(4, 5, 6);
    // 执行向量加法
    Vector3 c = a + b;
    // 断言验证 x/y/z 分量结果是否正确
    EXPECT_DOUBLE_EQ(c.x, 5);
    EXPECT_DOUBLE_EQ(c.y, 7);
    EXPECT_DOUBLE_EQ(c.z, 9);
}

// 测试向量减法运算
TEST(Vector3Test, Subtraction) {
    Vector3 a(7, 8, 9);
    Vector3 b(1, 4, 6);
    // 执行向量减法
    Vector3 c = a - b;
    // 验证减法结果
    EXPECT_DOUBLE_EQ(c.x, 6);
    EXPECT_DOUBLE_EQ(c.y, 4);
    EXPECT_DOUBLE_EQ(c.z, 3);
}

// 测试向量标量乘法（数乘）
TEST(Vector3Test, ScalarMultiplication) {
    Vector3 a(7, 8, 9);
    // 向量乘以标量 1.6
    Vector3 c = a * 1.6;
    // 验证数乘结果
    EXPECT_DOUBLE_EQ(c.x, 11.2);
    EXPECT_DOUBLE_EQ(c.y, 12.8);
    EXPECT_DOUBLE_EQ(c.z, 14.4);
}

// 测试向量标量除法
TEST(Vector3Test, ScalarDivision) {
    Vector3 a(7, 8, 9);
    // 向量除以标量 5
    Vector3 c = a / 5;
    // 验证除法结果
    EXPECT_DOUBLE_EQ(c.x, 1.4);
    EXPECT_DOUBLE_EQ(c.y, 1.6);
    EXPECT_DOUBLE_EQ(c.z, 1.8);
}

// 测试向量点乘（点积）运算
TEST(Vector3Test, DotProduct) {
    Vector3 a(0, 1, 3);
    Vector3 b(1, 4, 6);
    // 计算点积结果
    double c = a.dot(b);
    // 验证点积值
    EXPECT_DOUBLE_EQ(c, 22);
}

// 测试向量叉乘（叉积）运算
TEST(Vector3Test, CrossProduct) {
    // x轴单位向量
    Vector3 a(1, 0, 0);
    // y轴单位向量
    Vector3 b(0, 1, 0);
    // 计算叉乘
    Vector3 c = a.cross(b);
    // 叉乘结果应为 z轴单位向量 (0,0,1)
    EXPECT_DOUBLE_EQ(c.x, 0);
    EXPECT_DOUBLE_EQ(c.y, 0);
    EXPECT_DOUBLE_EQ(c.z, 1);
}

// 测试向量长度（模长）计算
TEST(Vector3Test, Length) {
    Vector3 a(3, 4, 12);
    // 计算向量长度
    double len = a.length();
    // 验证长度为 13
    EXPECT_DOUBLE_EQ(len, 13);
}

// 测试向量归一化（单位化）
TEST(Vector3Test, Normalized) {
    Vector3 a(3, 4, 12);
    // 对向量进行归一化
    Vector3 c = a.normalized();
    // 验证归一化后的分量
    EXPECT_DOUBLE_EQ(c.x, 3.0 / 13.0);
    EXPECT_DOUBLE_EQ(c.y, 4.0 / 13.0);
    EXPECT_DOUBLE_EQ(c.z, 12.0 / 13.0);
}