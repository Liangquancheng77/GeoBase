#include <gtest/gtest.h>
#include "Matrix3.h"
#include "Matrix4.h" 
#include "Quaternion.h" 
#include<iostream>
using namespace std;
#include <cmath>


// 转3x3旋转矩阵 绕x轴转60°
TEST(QuaternionTest, QuaternionTest1) {
	Quaternion qua(cos(PI/6), 0.5, 0, 0);
	Matrix3 mat1 = qua.toMatrix3();
	Matrix3 mat2 = Matrix3::rotationX(PI/3.0);
	EXPECT_TRUE(mat1.isApprox(mat2));
}

 // 从欧拉角创建 Y-X-Z
TEST(QuaternionTest, QuaternionTest2) {
	EulerAngles angles(PI / 2, 0.0,0.0);

	Quaternion qua = Quaternion::fromEuler(angles);

	EXPECT_NEAR(qua.w, cos(PI/4),EPS_ABS);
	EXPECT_NEAR(qua.x, 0,EPS_ABS);
	EXPECT_NEAR(qua.y, sin(PI/4),EPS_ABS);
	EXPECT_NEAR(qua.z, 0,EPS_ABS);

}

// 单位四元数 (1,0,0,0) 转矩阵是单位矩阵
TEST(QuaternionTest, QuaternionTest3) {
	Quaternion qua(1, 0, 0, 0);
	Matrix3 mat1 = qua.toMatrix3();
	Matrix3 mat2;
	EXPECT_TRUE(mat1.isApprox(mat2));
}

// Matrix3 转四元数
TEST(QuaternionTest, QuaternionTest4) {
	Matrix3 mat;
	mat.m[1][1] = cos(PI / 3);
	mat.m[1][2] = -sin(PI / 3);
	mat.m[2][1] = sin(PI / 3);
	mat.m[2][2] = cos(PI / 3);

	Quaternion qua = Quaternion::fromMatrix3(mat);

	EXPECT_NEAR(qua.w, cos(PI / 6), EPS_ABS);
	EXPECT_NEAR(qua.x, sin(PI / 6), EPS_ABS);
	EXPECT_NEAR(qua.y, 0.0, EPS_ABS);
	EXPECT_NEAR(qua.z, 0.0, EPS_ABS);

}

// q和-q等价
TEST(QuaternionTest, QuaternionTest5) {
	Quaternion qua1(1, 2, 3, 4);
	Quaternion qua2(-1, -2, -3, -4);
	Matrix3 mat1 = qua1.toMatrix3();
	Matrix3 mat2 = qua2.toMatrix3();
	EXPECT_TRUE(mat1.isApprox(mat2));
}

// 大角度测试
TEST(QuaternionTest, QuaternionTest6) {
	Quaternion qua(cos(PI * 7.4), sin(PI * 7.4), 0, 0);
	Matrix3 mat1 = qua.toMatrix3();
	Matrix3 mat2 = Matrix3::rotationX(PI * 14.8);
	EXPECT_TRUE(mat1.isApprox(mat2));
}

// 连续应用两个四元数旋转，得到的矩阵应该等于两个矩阵相乘
TEST(QuaternionTest, QuaternionTest7) {
	Quaternion q1 = Quaternion::fromEuler(EulerAngles(0.3, 0, 0));
	Quaternion q2 = Quaternion::fromEuler(EulerAngles(0, 0.5, 0));
	Quaternion q12 = q1*(q2);
	Matrix3 mat_q12 = q12.toMatrix3();
	Matrix3 mat_q1 = q1.toMatrix3();
	Matrix3 mat_q2 = q2.toMatrix3();
	EXPECT_TRUE(mat_q12.isApprox(mat_q1 * mat_q2));
}

// 退化场景：零四元数归一化抛出异常
TEST(QuaternionTest, QuaternionTest8) {
	// 构造零四元数
	Quaternion zeroQua(0.0, 0.0, 0.0, 0.0);

	// 验证：调用normalize时，会抛出 std::invalid_argument 异常
	EXPECT_THROW(zeroQua.normalize(), std::invalid_argument);
}

// 边界场景：极小模长四元数归一化，不产生NaN
TEST(QuaternionTest, TinyQuaternionNormalizeNoNaN) {
	// 构造一个模长极小但非零的四元数（避免触发异常）
	Quaternion tinyQua(1e-10, 0.0, 0.0, 0.0);
	Quaternion result = tinyQua;

	// 执行归一化
	result.normalize();

	// 验证：所有分量都不是NaN/Inf
	EXPECT_FALSE(std::isnan(result.w));
	EXPECT_FALSE(std::isnan(result.x));
	EXPECT_FALSE(std::isnan(result.y));
	EXPECT_FALSE(std::isnan(result.z));
	EXPECT_FALSE(std::isinf(result.w));
	EXPECT_FALSE(std::isinf(result.x));
	EXPECT_FALSE(std::isinf(result.y));
	EXPECT_FALSE(std::isinf(result.z));

	// 额外验证：归一化结果正确（这里应该变成单位四元数(1,0,0,0)）
	EXPECT_NEAR(result.w, 1.0, EPS_ABS);
	EXPECT_NEAR(result.x, 0.0, EPS_ABS);
}


