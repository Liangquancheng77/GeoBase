#include <gtest/gtest.h>
#include "Matrix3.h"
#include "Matrix4.h" 
#include<iostream>
using namespace std;
#include <cmath>

// 测试默认构造函数是否为单位矩阵
TEST(Matrix3Test, DefaultConstructorIsIdentity) {
    Matrix3 m;
    // 遍历 3x3 矩阵，验证主对角线为1，其余为0
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (i == j) EXPECT_DOUBLE_EQ(m.m[i][j], 1.0);
            else EXPECT_DOUBLE_EQ(m.m[i][j], 0.0);
}

// 测试 setIdentity 方法是否能正确将矩阵置为单位矩阵
TEST(Matrix3Test, SetIdentityWorks) {
    Matrix3 m(2.0);   // 初始化所有元素为2
    m.setIdentity();  // 执行单位化
    // 验证单位矩阵
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (i == j) EXPECT_DOUBLE_EQ(m.m[i][j], 1.0);
            else EXPECT_DOUBLE_EQ(m.m[i][j], 0.0);
}

// 测试矩阵乘法
TEST(Matrix3Test, Multiplication) {
    Matrix3 a = Matrix3::rotationZ(0.5);
    Matrix3 b = Matrix3::rotationX(0.3);
    Matrix3 c = a * b;

    // 使用转置验证乘法正确性：(a*b) * b^T ≈ a
    Matrix3 bt = b.transpose();
    Matrix3 a2 = c * bt;
    EXPECT_TRUE(a2.isApprox(a, 1e-6));
}

// 测试转置两次等于原矩阵（验证转置正确性）
TEST(Matrix3Test, TransposeTwiceIsIdentity) {
    Matrix3 a = Matrix3::rotationY(0.7);
    Matrix3 aT = a.transpose();   // 第一次转置
    Matrix3 aTT = aT.transpose(); // 第二次转置
    EXPECT_TRUE(aTT.isApprox(a));
}

// 测试绕 Z 轴旋转90度
TEST(Matrix3Test, RotationZ90Degrees) {
    const double PI = 3.14159265358979323846;
    Matrix3 rotZ = Matrix3::rotationZ(PI / 2.0);
    Vector3 v(1.0, 0.0, 0.0);
    Vector3 result = rotZ * v;

    // 预期 (1,0,0) 旋转后变为 (0,1,0)
    EXPECT_NEAR(result.x, 0.0, 1e-9);
    EXPECT_NEAR(result.y, 1.0, 1e-9);
    EXPECT_NEAR(result.z, 0.0, 1e-9);
}

// 测试绕 X 轴旋转90度
TEST(Matrix3Test, RotationX90Degrees) {
    const double PI = 3.14159265358979323846;
    Matrix3 rotX = Matrix3::rotationX(PI / 2.0);
    Vector3 v(0.0, 1.0, 0.0);
    Vector3 result = rotX * v;

    // 预期 (0,1,0) 旋转后变为 (0,0,1)
    EXPECT_NEAR(result.x, 0.0, 1e-9);
    EXPECT_NEAR(result.y, 0.0, 1e-9);
    EXPECT_NEAR(result.z, 1.0, 1e-9);
}

// 测试绕 Y 轴旋转90度
TEST(Matrix3Test, RotationY90Degrees) {
    const double PI = 3.14159265358979323846;
    Matrix3 rotY = Matrix3::rotationY(PI / 2.0);
    Vector3 v(0.0, 0.0, 1.0);
    Vector3 result = rotY * v;

    // 预期 (0,0,1) 旋转后变为 (1,0,0)
    EXPECT_NEAR(result.x, 1.0, 1e-9);
    EXPECT_NEAR(result.y, 0.0, 1e-9);
    EXPECT_NEAR(result.z, 0.0, 1e-9);
}

// 测试 3x3 矩阵转 4x4 矩阵功能
TEST(Matrix3Test, ToMatrix4) {
    Matrix3 m3 = Matrix3::rotationZ(0.8);
    Matrix4 m4 = m3.toMatrix4();

    // 检查第四行第四列是否为 [0,0,0,1]
    EXPECT_DOUBLE_EQ(m4.m[3][0], 0.0);
    EXPECT_DOUBLE_EQ(m4.m[3][1], 0.0);
    EXPECT_DOUBLE_EQ(m4.m[3][2], 0.0);
    EXPECT_DOUBLE_EQ(m4.m[3][3], 1.0);

    // 检查左上角 3x3 区域与原矩阵一致
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            EXPECT_DOUBLE_EQ(m4.m[i][j], m3.m[i][j]);
}

// 测试从 4x4 矩阵提取 3x3 矩阵
TEST(Matrix3Test, FromMatrix4) {
    Matrix4 m4;
    m4.setRotationX(0.5);  // 假设 Matrix4 已有 setRotationX 方法
    Matrix3 m3;
    m3.fromMatrix4(m4);

    // 验证左上角 3x3 区域匹配
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            EXPECT_DOUBLE_EQ(m3.m[i][j], m4.m[i][j]);
}

// 测试浮点数近似相等判断函数 isApprox
TEST(Matrix3Test, IsApprox) {
    Matrix3 a = Matrix3::rotationZ(0.5);
    Matrix3 b = a;

    // 完全相等
    EXPECT_TRUE(a.isApprox(b));

    // 微小误差
    b.m[0][0] += 1e-8;

    // 宽松精度下通过
    EXPECT_TRUE(a.isApprox(b, 1e-6));

    // 严格精度下不通过
    EXPECT_FALSE(a.isApprox(b, 1e-9));
}

// ==================== 退化测试 ====================

// 1. 零矩阵：所有元素为0。验证乘法、转置、行列式、与自身相等。
TEST(Matrix3Test, ZeroMatrix) {
    Matrix3 zero(0.0);          // 所有元素置0
    // 验证行列式为0
    EXPECT_DOUBLE_EQ(zero.determinant(), 0.0);
    // 转置仍是零矩阵
    Matrix3 zeroT = zero.transpose();
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            EXPECT_DOUBLE_EQ(zeroT.m[i][j], 0.0);
    // 零矩阵 * 任意矩阵 = 零矩阵
    Matrix3 rot = Matrix3::rotationZ(0.5);
    Matrix3 product = zero * rot;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            EXPECT_DOUBLE_EQ(product.m[i][j], 0.0);
    // 零矩阵与自身近似相等
    EXPECT_TRUE(zero.isApprox(zero));
}

// 2. 奇异矩阵：行列式为0，如某一行全为0。
TEST(Matrix3Test, SingularMatrix) {
    Matrix3 singular;
    // 第二行全0
    singular.m[1][0] = singular.m[1][1] = singular.m[1][2] = 0.0;
     //其他元素保持单位矩阵，确保行列式为0
    EXPECT_NEAR(singular.determinant(), 0.0, 1e-9);
    // 奇异矩阵乘法后仍奇异，验证不崩溃
    Matrix3 rot = Matrix3::rotationY(0.3);
    Matrix3 prod = singular * rot;
    EXPECT_NEAR(prod.determinant(), 0.0, 1e-9);
}

// 3. 接近奇异的矩阵：行列式非常小，但不为0，测试数值稳定性。
TEST(Matrix3Test, NearlySingularMatrix) {
    Matrix3 nearly;
    // 构造一个接近奇异的矩阵：第一行 [1, 1, 1]，第二行 [1, 1+eps, 1]，第三行 [1, 1, 1+eps]
    const double eps = 1e-8;
    nearly.m[0][0] = 1.0; nearly.m[0][1] = 1.0; nearly.m[0][2] = 1.0;
    nearly.m[1][0] = 1.0; nearly.m[1][1] = 1.0 + eps; nearly.m[1][2] = 1.0;
    nearly.m[2][0] = 1.0; nearly.m[2][1] = 1.0; nearly.m[2][2] = 1.0 + eps;
    double det = nearly.determinant();
    //double det = eps* eps;
     //行列式应近似 eps^2 量级
    //EXPECT_LT(det, 1e-14);
    //EXPECT_GT(det, 0.0);
    // 检查逆矩阵是否存在（det不为0），计算逆矩阵不应崩溃
    // 注意：inverse() 要求 det 不为0，这里 det 很小但非零，仍可求逆
    Matrix3 inv = nearly.inverse();
    // 验证原矩阵 * 逆 ≈ 单位矩阵
    Matrix3 prod = nearly * inv;
    EXPECT_TRUE(prod.isApprox(Matrix3::identity(), 1e-5));
}

// 4. 极大/极小数值：元素为 1e20 或 1e-20，验证乘法、转置不溢出。
TEST(Matrix3Test, ExtremeValues) {
    Matrix3 large;
    large.m[0][0] = 1e20; large.m[0][1] = 1e20; large.m[0][2] = 0.0;
    large.m[1][0] = 0.0;  large.m[1][1] = 1e20; large.m[1][2] = 0.0;
    large.m[2][0] = 0.0;  large.m[2][1] = 0.0;  large.m[2][2] = 1e20;
    // 乘法不应溢出
    Matrix3 product = large * large;
    // 结果应为 diag(1e40, 1e40, 1e40)
    EXPECT_NEAR(product.m[0][0], 1e40, 1e30);
    EXPECT_NEAR(product.m[1][1], 1e40, 1e30);
    EXPECT_NEAR(product.m[2][2], 1e40, 1e30);
    // 极小数值
    Matrix3 tiny;
    tiny.m[0][0] = 1e-20; tiny.m[0][1] = 1e-20;tiny.m[1][0] = 1e-20;
    // 行列式值接近0，但不应出现 NaN
    EXPECT_NO_THROW(tiny.determinant());
    // 转置不影响数值大小
    Matrix3 tinyT = tiny.transpose();
    EXPECT_DOUBLE_EQ(tinyT.m[0][1], 1e-20);
}

// 5. 零矩阵乘法（已隐式测试，这里显式测试左乘和右乘的退化）
TEST(Matrix3Test, ZeroMatrixMultiplication) {
    Matrix3 zero(0.0);
    Matrix3 rot = Matrix3::rotationX(1.2);
    // 左乘：zero * rot = zero
    Matrix3 left = zero * rot;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            EXPECT_DOUBLE_EQ(left.m[i][j], 0.0);
    // 右乘：rot * zero = zero
    Matrix3 right = rot * zero;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            EXPECT_DOUBLE_EQ(right.m[i][j], 0.0);
}

//-----------------欧拉角---------------------

// 1. EulerAngles(0,0,0)转矩阵是单位矩阵
TEST(Matrix3EulerTest, ZeroEulerIsIdentity) {
    EulerAngles angles; // 默认构造 (0,0,0)
    Matrix3 mat = Matrix3::fromEulerAngles(angles);
    EXPECT_TRUE(mat.isApprox(Matrix3::identity()));
}

// 2. EulerAngles(π/2, 0, 0) 转矩阵验证（仅Y轴旋转90°）
TEST(Matrix3EulerTest, Yaw90DegMatrix) {
    EulerAngles angles;
    angles.yaw = PI / 2;
    angles.pitch = 0;
    angles.roll = 0;

    Matrix3 mat = Matrix3::fromEulerAngles(angles);
    // Y轴旋转90°标准矩阵校验
    EXPECT_NEAR(mat.m[0][0], 0.0, 1e-6);
    EXPECT_NEAR(mat.m[0][2], 1.0, 1e-6);
    EXPECT_NEAR(mat.m[2][0], -1.0, 1e-6);
    EXPECT_NEAR(mat.m[2][2], 0.0, 1e-6);
}

// 3. 普通例子验证正交矩阵转欧拉角（双向互转）
TEST(Matrix3EulerTest, MatrixToEulerNormal) {
    EulerAngles src;
    src.yaw = 0.5;
    src.pitch = 0.2;
    src.roll = 0.3;

    Matrix3 mat = Matrix3::fromEulerAngles(src);
    EulerAngles res = mat.toEulerAngles();

     //验证互转误差极小
    EXPECT_NEAR(res.yaw, src.yaw, 1e-6);
    EXPECT_NEAR(res.pitch, src.pitch, 1e-6);
    EXPECT_NEAR(res.roll, src.roll, 1e-6);
}

// 4. 验证万向锁判断
TEST(Matrix3EulerTest, GimbalLockCheck) {
    EulerAngles angles;
    // 构造接近万向锁：pitch = π/2
    angles.pitch = PI / 2 - 1e-7;
    // 应判定为万向锁
    EXPECT_TRUE(angles.isGimbalLocked());

    // 普通角度，不应触发
    angles.pitch = 0.5;
    EXPECT_FALSE(angles.isGimbalLocked());
}

// 5. 验证转角度制
TEST(Matrix3EulerTest, ToDegreesTest) {
    EulerAngles angles;
    angles.yaw = PI;      // 180°
    angles.pitch = PI / 2;  // 90°
    angles.roll = 0;      // 0°

    Vector3 deg = angles.toDegrees();
    EXPECT_NEAR(deg.x, 180.0, 1e-6);
    EXPECT_NEAR(deg.y, 90.0, 1e-6);
    EXPECT_NEAR(deg.z, 0.0, 1e-6);
}

// 6. 验证欧拉角转正交矩阵（普通值）
//TEST(Matrix3EulerTest, EulerToMatrixNormal) {
//    EulerAngles angles;
//    angles.yaw = 0.4;
//    angles.pitch = 0.5;
//    angles.roll = 0.7;
//
//    Matrix3 mat = Matrix3::fromEulerAngles(angles);
//    // 正交矩阵：转置 ≈ 逆矩阵
//    Matrix3 trans = mat.transpose();
//    Matrix3 inv = mat.inverse();
//    EXPECT_TRUE(trans.isApprox(inv, 1e-6));
//}

// 7. 退化测试：EulerAngles(2π,2π,2π)转矩阵是单位矩阵
TEST(Matrix3EulerTest, FullRotationIsIdentity) {
    EulerAngles angles;
    angles.yaw = 2 * PI;
    angles.pitch = 2 * PI;
    angles.roll = 2 * PI;

    Matrix3 mat = Matrix3::fromEulerAngles(angles);
    EXPECT_TRUE(mat.isApprox(Matrix3::identity(), 1e-6));
}

// 8. 万向锁情况：pitch = +90°，验证 toEulerAngles 将 roll 置 0，yaw 吸收原 roll
TEST(Matrix3EulerTest, GimbalLockAtPitch90) {
    EulerAngles src;
    src.yaw = 0.5;
    src.pitch = PI / 2.0;   // 90°
    src.roll = 0.3;

    Matrix3 mat = Matrix3::fromEulerAngles(src);
    EulerAngles dst = mat.toEulerAngles();

    // 万向锁下，roll 应被强制为 0，yaw 约等于原 yaw+roll，pitch 保持 90°
    EXPECT_NEAR(dst.roll, 0.0, 1e-6);
    EXPECT_NEAR(dst.yaw, src.yaw + src.roll, 1e-6);
    EXPECT_NEAR(dst.pitch, PI / 2.0, 1e-6);
}

// 9. 角度环绕测试：超过 2π 的范围应归一化到 [-π, π] 或等效旋转
TEST(Matrix3EulerTest, AngleWrapAround) {
    EulerAngles angles;
    angles.yaw = 3 * PI;    // 540°, 等效于 π
    angles.pitch = -2 * PI; // -360°, 等效于 0
    angles.roll = 4 * PI;   // 720°, 等效于 0

    Matrix3 mat = Matrix3::fromEulerAngles(angles);
    // 由于三角函数周期性，大角度应产生与 (π, 0, 0) 相同的矩阵
    EulerAngles expected;
    expected.yaw = PI;
    expected.pitch = 0.0;
    expected.roll = 0.0;
    Matrix3 expectedMat = Matrix3::fromEulerAngles(expected);

    EXPECT_TRUE(mat.isApprox(expectedMat, 1e-6));
}

// 10. 接近万向锁但未触发临界值：pitch = 89.999°，应正常转换（不进入万向锁分支）
TEST(Matrix3EulerTest, NearGimbalLockButStable) {
    const double eps = 1e-5;
    double pitchNear = PI / 2.0 - eps;  // 略小于 90°

    EulerAngles src;
    src.yaw = 0.2;
    src.pitch = pitchNear;
    src.roll = 0.4;

    Matrix3 mat = Matrix3::fromEulerAngles(src);
    EulerAngles dst = mat.toEulerAngles();

    // 由于未达到万向锁临界值，roll 不应被强制为 0
    EXPECT_GT(fabs(dst.roll), 1e-6);
    // 互转误差应很小
    EXPECT_NEAR(dst.yaw, src.yaw, 1e-5);
    EXPECT_NEAR(dst.pitch, src.pitch, 1e-5);
    EXPECT_NEAR(dst.roll, src.roll, 1e-5);
}