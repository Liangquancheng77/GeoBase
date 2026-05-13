#include <gtest/gtest.h>
#include "Matrix3.h"
#include "Matrix4.h"   // 用于互转测试

TEST(Matrix3Test, DefaultConstructorIsIdentity) {
    Matrix3 m;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (i == j) EXPECT_DOUBLE_EQ(m.m[i][j], 1.0);
            else EXPECT_DOUBLE_EQ(m.m[i][j], 0.0);
}

TEST(Matrix3Test, SetIdentityWorks) {
    Matrix3 m(2.0);   // 全为2
    m.setIdentity();
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (i == j) EXPECT_DOUBLE_EQ(m.m[i][j], 1.0);
            else EXPECT_DOUBLE_EQ(m.m[i][j], 0.0);
}

TEST(Matrix3Test, Multiplication) {
    Matrix3 a = Matrix3::rotationZ(0.5);
    Matrix3 b = Matrix3::rotationX(0.3);
    Matrix3 c = a * b;
    // 手工验证会比较麻烦，我们用逆运算验证： (a*b)*b^T ≈ a
    Matrix3 bt = b.transpose();
    Matrix3 a2 = c * bt;
    EXPECT_TRUE(a2.isApprox(a, 1e-6));
}

TEST(Matrix3Test, TransposeTwiceIsIdentity) {
    Matrix3 a = Matrix3::rotationY(0.7);
    Matrix3 aT = a.transpose();
    Matrix3 aTT = aT.transpose();
    EXPECT_TRUE(aTT.isApprox(a));
}

TEST(Matrix3Test, RotationZ90Degrees) {
    const double PI = 3.14159265358979323846;
    Matrix3 rotZ = Matrix3::rotationZ(PI / 2.0);
    Vector3 v(1.0, 0.0, 0.0);
    Vector3 result = rotZ * v;
    EXPECT_NEAR(result.x, 0.0, 1e-9);
    EXPECT_NEAR(result.y, 1.0, 1e-9);
    EXPECT_NEAR(result.z, 0.0, 1e-9);
}

TEST(Matrix3Test, RotationX90Degrees) {
    const double PI = 3.14159265358979323846;
    Matrix3 rotX = Matrix3::rotationX(PI / 2.0);
    Vector3 v(0.0, 1.0, 0.0);
    Vector3 result = rotX * v;
    EXPECT_NEAR(result.x, 0.0, 1e-9);
    EXPECT_NEAR(result.y, 0.0, 1e-9);
    EXPECT_NEAR(result.z, 1.0, 1e-9);
}

TEST(Matrix3Test, RotationY90Degrees) {
    const double PI = 3.14159265358979323846;
    Matrix3 rotY = Matrix3::rotationY(PI / 2.0);
    Vector3 v(0.0, 0.0, 1.0);
    Vector3 result = rotY * v;
    EXPECT_NEAR(result.x, 1.0, 1e-9);
    EXPECT_NEAR(result.y, 0.0, 1e-9);
    EXPECT_NEAR(result.z, 0.0, 1e-9);
}

TEST(Matrix3Test, ToMatrix4) {
    Matrix3 m3 = Matrix3::rotationZ(0.8);
    Matrix4 m4 = m3.toMatrix4();
    // 检查第四行第四列为 [0,0,0,1]
    EXPECT_DOUBLE_EQ(m4.m[3][0], 0.0);
    EXPECT_DOUBLE_EQ(m4.m[3][1], 0.0);
    EXPECT_DOUBLE_EQ(m4.m[3][2], 0.0);
    EXPECT_DOUBLE_EQ(m4.m[3][3], 1.0);
    // 左上角应与 Matrix3 相同
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            EXPECT_DOUBLE_EQ(m4.m[i][j], m3.m[i][j]);
}

TEST(Matrix3Test, FromMatrix4) {
    Matrix4 m4;
    m4.setRotationX(0.5);  // 假设 Matrix4 已有 setRotationX 方法
    Matrix3 m3;
    m3.fromMatrix4(m4);
    // 验证提取的左上角与原始矩阵一致
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            EXPECT_DOUBLE_EQ(m3.m[i][j], m4.m[i][j]);
}

TEST(Matrix3Test, IsApprox) {
    Matrix3 a = Matrix3::rotationZ(0.5);
    Matrix3 b = a;
    EXPECT_TRUE(a.isApprox(b));
    b.m[0][0] += 1e-8;
    EXPECT_TRUE(a.isApprox(b, 1e-6));
    EXPECT_FALSE(a.isApprox(b, 1e-9));
}