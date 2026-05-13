#include<iostream>
using namespace std;
#include "Matrix4.h";
#include <cmath>
#include<gtest/gtest.h>

//const double EPS = 1e-9;


void printMatrix4(const Matrix4& mat) {
	for (size_t i = 0; i < sizeof(mat.m) / sizeof(mat.m[0]); i++)
	{
		for (size_t j = 0; j < sizeof(mat.m[0]) / sizeof(double); j++)
		{
			cout << mat.m[i][j] << "\t";
		}
		cout << endl;
	}
}


//int main() {
//
//	Matrix4 m1;
//	cout << "m1.00:" << m1.m[0][0] << endl;
//
//	Matrix4 m2 = Matrix4::createTranslation(1, 2, 3);
//	cout << "m2.23:" << m2.m[2][3] << endl;
//
//	Matrix4 m3 = Matrix4::createRotationX(PI/6);
//	cout << "m3.21:" << m3.m[2][1] << endl;
//
//	Matrix4 m4 = m2.multiply(m1);
//	printMatrix4(m4);
//
//
//	system("pause");
//
//	return 0;
//
//}

static Matrix4 makeExpectedTranslation(double tx, double ty, double tz) {
	Matrix4 m;
	m.setIdentity();
	m.m[0][3] = tx;
	m.m[1][3] = ty;
	m.m[2][3] = tz;
	return m;
}

//==================== 1. 构造与单位矩阵 ====================
TEST(Matrix4Test, DefaultConstructorIsIdentity) {
	Matrix4 m;
	EXPECT_DOUBLE_EQ(m.m[0][0], 1.0);
	EXPECT_DOUBLE_EQ(m.m[0][1], 0.0);
	EXPECT_DOUBLE_EQ(m.m[0][2], 0.0);
	EXPECT_DOUBLE_EQ(m.m[0][3], 0.0);
	EXPECT_DOUBLE_EQ(m.m[1][1], 1.0);
	EXPECT_DOUBLE_EQ(m.m[2][2], 1.0);
	EXPECT_DOUBLE_EQ(m.m[3][3], 1.0);
	// 随机抽一个非对角
	EXPECT_DOUBLE_EQ(m.m[1][2], 0.0);
}

TEST(Matrix4Test, SetIdentityWorks) {
	Matrix4 m;
	m.m[0][0] = 99.0;
	m.m[1][1] = 99.0;
	m.setIdentity();
	EXPECT_DOUBLE_EQ(m.m[0][0], 1.0);
	EXPECT_DOUBLE_EQ(m.m[1][1], 1.0);
	EXPECT_DOUBLE_EQ(m.m[2][2], 1.0);
	EXPECT_DOUBLE_EQ(m.m[0][1], 0.0);
}

//==================== 2. 平移矩阵 ====================

//平移矩阵变换点：点坐标增加平移量
TEST(Matrix4Test, TranslationTransformPoint) {
	Matrix4 trans = Matrix4::createTranslation(2.0, 3.0, 4.0);
	Vector3 p(1.0, 2.0, 3.0);
	Vector3 result = trans.transformPoint(p);

	EXPECT_NEAR(result.x, 3.0, 1e-9);
	EXPECT_NEAR(result.y, 5.0, 1e-9);
	EXPECT_NEAR(result.z, 7.0, 1e-9);
}

//静态工厂 createTranslation 生成的矩阵与手动设置平移矩阵等价
TEST(Matrix4Test, CreateTranslationFactory) {
	Matrix4 trans = Matrix4::createTranslation(3.0, 7.0, 11.0);
	Matrix4 expected = makeExpectedTranslation(3.0, 7.0, 11.0);
	EXPECT_TRUE(trans == expected);

}

//setTranslation 方法能正确设置平移矩阵。
TEST(Matrix4Test, SetTranslationMethod) {
	Matrix4 trans;
	trans.setTranslation(1.0, 31.0, 21.0);
	Matrix4 expected = makeExpectedTranslation(1.0, 31.0, 21.0);
	EXPECT_TRUE(trans == expected);
}

//平移矩阵变换向量：向量不变
TEST(Matrix4Test, TranslationTransformVector) {
	Vector3 vec = { 1.0,3.0,7.0 };
	Matrix4 trans = Matrix4::createTranslation(3.0, 7.0, 11.0);
	Vector3 result = trans.transformVector(vec);
	EXPECT_NEAR(result.x, 1.0, 1e-9);
	EXPECT_NEAR(result.y, 3.0, 1e-9);
	EXPECT_NEAR(result.z, 7.0, 1e-9);
}




//==================== 3. 旋转矩阵 ====================

//绕Z轴旋转90°，点 (1,0,0) 变为 (0,1,0)
TEST(Matrix4Test, RotationZ90Degrees) {
	Vector3 p(1.0,0,0);
	Matrix4 trans = Matrix4::createRotationZ(PI/2.0);
	Vector3 result = trans.transformPoint(p);
	EXPECT_NEAR(result.x, 0, 1e-9);
	EXPECT_NEAR(result.y, 1.0, 1e-9);
	EXPECT_NEAR(result.z, 0, 1e-9);
}

//绕X轴旋转90°，点 (0,1,0) 变为 (0,0,1)。
TEST(Matrix4Test, RotationX90Degrees) {
	Vector3 p(0, 1.0, 0);
	Matrix4 trans = Matrix4::createRotationX(PI / 2.0);
	Vector3 result = trans.transformPoint(p);
	EXPECT_NEAR(result.x, 0, 1e-9);
	EXPECT_NEAR(result.y, 0, 1e-9);
	EXPECT_NEAR(result.z, 1, 1e-9);
}

//绕Y轴旋转90°，点 (0,0,1) 变为 (1,0,0)。
TEST(Matrix4Test, RotationY90Degrees) {
	Vector3 p(0, 0, 1.0);
	Matrix4 trans = Matrix4::createRotationY(PI / 2.0);
	Vector3 result = trans.transformPoint(p);
	EXPECT_NEAR(result.x, 1.0, 1e-9);
	EXPECT_NEAR(result.y, 0, 1e-9);
	EXPECT_NEAR(result.z, 0, 1e-9);
}

//旋转0弧度应得到单位矩阵。
TEST(Matrix4Test, RotationZeroIsIdentity) {
	Matrix4 rot = Matrix4::createRotationZ(0.0);
	Matrix4 identity;
	EXPECT_TRUE(rot.isEqualApprox(identity));

}

// 工厂与set方法一致性
TEST(Matrix4Test, CreateRotationXFactory) {
	Matrix4 byFactory = Matrix4::createRotationX(0.5);
	Matrix4 bySetter;
	bySetter.setRotationX(0.5);
	EXPECT_TRUE(byFactory.isEqualApprox(bySetter));
}

TEST(Matrix4Test, CreateRotationYFactory) {
	Matrix4 byFactory = Matrix4::createRotationY(0.5);
	Matrix4 bySetter;
	bySetter.setRotationY(0.5);
	EXPECT_TRUE(byFactory.isEqualApprox(bySetter));
}

TEST(Matrix4Test, CreateRotationZFactory) {
	Matrix4 byFactory = Matrix4::createRotationZ(0.5);
	Matrix4 bySetter;
	bySetter.setRotationZ(0.5);
	EXPECT_TRUE(byFactory.isEqualApprox(bySetter));
}


//==================== 4. 缩放矩阵 ====================

//静态工厂 createScaling 生成的矩阵对角线为缩放因子
TEST(Matrix4Test, CreateScalingFactory) {
	Matrix4 scale = Matrix4::createScaling(2.0, 3.0, 4.0);
	Matrix4 expected;
	expected.setIdentity();
	expected.m[0][0] = 2.0;
	expected.m[1][1] = 3.0;
	expected.m[2][2] = 4.0;
	EXPECT_TRUE(scale.isEqualApprox(expected));
}

//setScaling 能正确设置缩放矩阵
TEST(Matrix4Test, SetScalingMethod) {
	Matrix4 scale;
	scale.setScaling(0.5, 1.0, 2.0);
	Matrix4 expected;
	expected.setIdentity();
	expected.m[0][0] = 0.5;
	expected.m[1][1] = 1.0;
	expected.m[2][2] = 2.0;
	EXPECT_TRUE(scale.isEqualApprox(expected));
}

//缩放矩阵作用于点：各分量乘以对应缩放因子
TEST(Matrix4Test, ScalingTransformPoint) {
	Vector3 vec(14,21,60);
	Matrix4 scale = Matrix4::createScaling(0.5, 1.0/3.0, 0.25);
	Vector3 result = scale.transformPoint(vec);
	EXPECT_NEAR(result.x, 7.0, 1e-9);
	EXPECT_NEAR(result.y, 7.0, 1e-9);
	EXPECT_NEAR(result.z, 15.0, 1e-9);
}

//缩放矩阵作用于向量：结果同样乘以缩放因子
TEST(Matrix4Test, ScalingTransformVector) {
	Vector3 vec(14, 21, 60);
	Matrix4 scale = Matrix4::createScaling(0.5, 1.0 / 3.0, 0.25);
	Vector3 result = scale.transformVector(vec);
	EXPECT_NEAR(result.x, 7.0, 1e-9);
	EXPECT_NEAR(result.y, 7.0, 1e-9);
	EXPECT_NEAR(result.z, 15.0, 1e-9);
}

//==================== 5. 矩阵乘法 ====================

//单位矩阵乘任何矩阵等于原矩阵，任何矩阵乘单位矩阵也等于原矩阵
TEST(Matrix4Test, MultiplyIdentity) {
	Matrix4 trans =Matrix4::createTranslation(1.0, 4.0, 7.0);
	Matrix4 identity;
	Matrix4 result1 = trans.multiply(identity);
	Matrix4 result2 = identity.multiply(trans);
	EXPECT_TRUE(trans == result1);
	EXPECT_TRUE(trans == result2);
}

//矩阵乘法满足结合律：(A*B)*C == A*(B*C)
TEST(Matrix4Test, MultiplyAssociative) {
	Matrix4 A = Matrix4::createRotationZ(0.5);
	Matrix4 B = Matrix4::createTranslation(1.0, 2.0, 3.0);
	Matrix4 C = Matrix4::createScaling(2.0, 2.0, 2.0);
	Matrix4 left = A.multiply(B.multiply(C));
	Matrix4 right = (A.multiply(B)).multiply(C);
	EXPECT_TRUE(left.isEqualApprox(right));
}

//乘法结果与连续变换的一致性：(A*B).transformPoint(p) == A.transformPoint(B.transformPoint(p))
TEST(Matrix4Test, MultiplyConsistentWithTransform) {
	Matrix4 A = Matrix4::createRotationZ(0.3);
	Matrix4 B = Matrix4::createTranslation(1.0, 2.0, 3.0);
	Vector3 p(1.0, 1.0, 1.0);
	Matrix4 C = A.multiply(B);
	Vector3 result1 = C.transformPoint(p);
	Vector3 result2 = A.transformPoint(B.transformPoint(p));
	EXPECT_NEAR(result1.x, result2.x, 1e-9);
	EXPECT_NEAR(result1.y, result2.y, 1e-9);
	EXPECT_NEAR(result1.z, result2.z, 1e-9);
}

//==================== 6. 变换点与向量组合 ====================

//组合变换（先旋转后平移）的结果与单独应用顺序一致
TEST(Matrix4Test, TransformPointCombination) {
	Matrix4 rot = Matrix4::createRotationZ(1.0);
	Matrix4 trans = Matrix4::createTranslation(10.0, 0.0, 0.0);
	Matrix4 combined = trans.multiply(rot); // 先旋转后平移
	Vector3 p(1.0, 0.0, 0.0);
	Vector3 result = combined.transformPoint(p);
	Vector3 expected = trans.transformPoint(rot.transformPoint(p));
	EXPECT_NEAR(result.x, expected.x, 1e-9);
	EXPECT_NEAR(result.y, expected.y, 1e-9);
	EXPECT_NEAR(result.z, expected.z, 1e-9);
}

//任意矩阵的 transformVector 应忽略平移部分（第四列不影响向量）
TEST(Matrix4Test, TransformVectorIgnoredTranslation) {
	Matrix4 trans = Matrix4::createTranslation(100.0, 0.0, 0.0);
	Vector3 v(1.0, 0.0, 0.0);
	Vector3 result = trans.transformVector(v);
	EXPECT_NEAR(result.x, 1.0, 1e-9);
	EXPECT_NEAR(result.y, 0.0, 1e-9);
	EXPECT_NEAR(result.z, 0.0, 1e-9);
}

//==================== 7. 转置 ====================

//两次转置恢复原矩阵
TEST(Matrix4Test, TransposeTwice) {
	Matrix4 trans = Matrix4::createTranslation(1.0, 2.0, 3.0);
	Matrix4 transposed = trans.transpose();
	Matrix4 doubleTransposed = transposed.transpose();
	EXPECT_TRUE(doubleTransposed.isEqualApprox(trans));
}

//旋转矩阵的转置等于其逆矩阵（旋转矩阵乘以转置近似为单位矩阵）
TEST(Matrix4Test, RotationMatrixTransposeIsInverse) {
	Matrix4 rot = Matrix4::createRotationZ(0.8);
	Matrix4 inv = rot.transpose();
	Matrix4 product = rot.multiply(inv);
	Matrix4 identity;
	EXPECT_TRUE(product.isEqualApprox(identity));
}


//==================== 8. 近似相等与运算符== ====================

//两个相同矩阵应被 isEqualApprox 判定为相等
TEST(Matrix4Test, IsEqualApproxTrueForSameMatrix) {
	Matrix4 a = Matrix4::createTranslation(1.0, 2.0, 3.0);
	Matrix4 b = Matrix4::createTranslation(1.0, 2.0, 3.0);
	EXPECT_TRUE(a.isEqualApprox(b));
	EXPECT_TRUE(a == b);
}

//不同矩阵应被判定为不等
TEST(Matrix4Test, IsEqualApproxFalseForDifferentMatrix) {
	Matrix4 a = Matrix4::createTranslation(1.0, 2.0, 3.0);
	Matrix4 b = Matrix4::createTranslation(1.0, 2.0, 3.1);
	EXPECT_FALSE(a.isEqualApprox(b));
	EXPECT_FALSE(a == b);
}

//验证 eps 参数能正确控制容差
TEST(Matrix4Test, IsEqualApproxToleranceWorks) {
	Matrix4 a = Matrix4::createTranslation(1.0, 2.0, 3.0);
	Matrix4 b = Matrix4::createTranslation(1.0, 2.0, 3.0 + 1e-8);
	EXPECT_TRUE(a.isEqualApprox(b, 1e-6));
	EXPECT_FALSE(a.isEqualApprox(b, 1e-9));
}

//==================== 9. 边界与退化 ====================

//缩放因子为0时，变换点变为原点
TEST(Matrix4Test, TransformPointWithZeroScaling) {
	Matrix4 scale = Matrix4::createScaling(0.0, 0.0, 0.0);
	Vector3 p(1.0, 2.0, 3.0);
	Vector3 result = scale.transformPoint(p);
	EXPECT_NEAR(result.x, 0.0, 1e-9);
	EXPECT_NEAR(result.y, 0.0, 1e-9);
	EXPECT_NEAR(result.z, 0.0, 1e-9);
}

//缩放因子为0时，变换向量变为零向量
TEST(Matrix4Test, TransformVectorWithZeroScaling) {
	Matrix4 scale = Matrix4::createScaling(0.0, 0.0, 0.0);
	Vector3 v(5.0, 6.0, 7.0);
	Vector3 result = scale.transformVector(v);
	EXPECT_NEAR(result.x, 0.0, 1e-9);
	EXPECT_NEAR(result.y, 0.0, 1e-9);
	EXPECT_NEAR(result.z, 0.0, 1e-9);
}

//极大的平移量（如1e10）不会导致浮点溢出，结果应为线性增加
TEST(Matrix4Test, LargeTranslationDoesNotOverflow) {
	Matrix4 trans = Matrix4::createTranslation(1e10, 1e10, 1e10);
	Vector3 p(1.0, 1.0, 1.0);
	Vector3 result = trans.transformPoint(p);
	EXPECT_DOUBLE_EQ(result.x, 1e10 + 1.0);
	EXPECT_DOUBLE_EQ(result.y, 1e10 + 1.0);
	EXPECT_DOUBLE_EQ(result.z, 1e10 + 1.0);
}



//相机坐标和目标坐标有极大距离时不会导致浮点溢出
TEST(Matrix4Test, LookAt_LargeDistanceNoOverflow) {
	Vector3 eye(0, 0, 1e9);
	Vector3 target(0, 0, 3e9);
	Matrix4 mat = Matrix4::createLookAt(eye, target);
	EXPECT_NEAR(mat.m[0][3], 0, 1e-9);
	EXPECT_NEAR(mat.m[1][3], 0, 1e-9);
	EXPECT_NEAR(mat.m[2][3], -2e18, 1e-9);
}

// 旋转角度为2π或 - 2π时，结果应近似单位矩阵
TEST(Matrix4Test, RotationExtremeAngles) {
	Matrix4 rot = Matrix4::createRotationZ(2.0 * PI);
	Matrix4 identity;
	EXPECT_TRUE(rot.isEqualApprox(identity, 1e-9));
	rot = Matrix4::createRotationZ(-2.0 * PI);
	EXPECT_TRUE(rot.isEqualApprox(identity, 1e-9));
}

// 非仿射矩阵求逆的异常测试
TEST(Matrix4Test, InverseAffine_NonAffineMatrixThrows) {
	// 构造一个非仿射矩阵（最后一行不是 0 0 0 1）
	Matrix4 mat;
	mat.m[3][0] = 1.0f; // 修改最后一行，使其不满足仿射矩阵条件

	// 预期调用 inverseAffine 会抛出 std::invalid_argument 异常
	EXPECT_THROW({
		mat.inverseAffine();
		}, std::invalid_argument);
}

// 非仿射矩阵求逆：最后一行赋值极大浮点数（极端数值场景）→ 抛异常
TEST(Matrix4Test, InverseAffine_LargeValueLastRowThrows) {
	Matrix4 mat;
	mat.setIdentity();
	// 几何算法极端数值测试，防止数值溢出导致判断失效
	mat.m[3][2] = 1e10f;
	EXPECT_THROW(mat.inverseAffine(), std::invalid_argument);
}

// 非仿射矩阵求逆：W分量刚好超出EPS边界（1.000002f，浮点边界测试）
TEST(Matrix4Test, InverseAffine_WComponentOutOfToleranceThrows) {
	Matrix4 mat;
	mat.setIdentity();
	mat.m[3][3] = 1.000002f; // 误差刚好超过1e-6
	EXPECT_THROW(mat.inverseAffine(), std::invalid_argument);
}


//==================== 11. 仿射矩阵的逆 ====================

// 单位矩阵的逆还是单位矩阵
TEST(Matrix4Test, InverseAffine_Identity) {
	Matrix4 mat;
	Matrix4 result = mat.inverseAffine();
	EXPECT_TRUE(mat.multiply(result).isEqualApprox(mat, 1e-9));
	EXPECT_NEAR(result.m[0][0], 1.0, 1e-9);
	EXPECT_NEAR(result.m[1][1], 1.0, 1e-9);
	EXPECT_NEAR(result.m[2][2], 1.0, 1e-9);
	EXPECT_NEAR(result.m[3][3], 1.0, 1e-9);
	EXPECT_NEAR(result.m[2][1], 0.0, 1e-9);
}
// 平移矩阵的逆是反向平移
TEST(Matrix4Test, InverseAffine_Translation) {
	Matrix4 mat;
	mat.m[0][3] = 1.0;
	mat.m[1][3] = 7.0;
	mat.m[2][3] = 4.0;
	Matrix4 result = mat.inverseAffine();
	Matrix4 mat2;
	EXPECT_TRUE(mat.multiply(result).isEqualApprox(mat2, 1e-9));
	EXPECT_NEAR(result.m[0][3], -1.0, 1e-9);
	EXPECT_NEAR(result.m[1][3], -7.0, 1e-9);
	EXPECT_NEAR(result.m[2][3], -4.0, 1e-9);
	EXPECT_NEAR(result.m[3][0], 0.0, 1e-9);
}




//==================== 11. 复合功能：旋转矩阵正交性 ====================

//验证旋转矩阵列向量单位正交（点积为0，模长为1）
TEST(Matrix4Test, OrthogonalMatrixProperties) {
	Matrix4 rot = Matrix4::createRotationZ(0.3);
	// 提取前三列向量
	Vector3 col0(rot.m[0][0], rot.m[1][0], rot.m[2][0]);
	Vector3 col1(rot.m[0][1], rot.m[1][1], rot.m[2][1]);
	Vector3 col2(rot.m[0][2], rot.m[1][2], rot.m[2][2]);
	// 列向量模长应为1
	EXPECT_NEAR(col0.length(), 1.0, 1e-9);
	EXPECT_NEAR(col1.length(), 1.0, 1e-9);
	EXPECT_NEAR(col2.length(), 1.0, 1e-9);
	// 列向量两两正交
	EXPECT_NEAR(col0.dot(col1), 0.0, 1e-9);
	EXPECT_NEAR(col0.dot(col2), 0.0, 1e-9);
	EXPECT_NEAR(col1.dot(col2), 0.0, 1e-9);
}

//验证平移、旋转、缩放矩阵的最后一行为 [0,0,0,1]
TEST(Matrix4Test, AffineMatrixStructure) {
	// 平移、旋转、缩放矩阵的最后一行应为 [0,0,0,1]
	Matrix4 trans = Matrix4::createTranslation(1, 2, 3);
	EXPECT_DOUBLE_EQ(trans.m[3][0], 0.0);
	EXPECT_DOUBLE_EQ(trans.m[3][1], 0.0);
	EXPECT_DOUBLE_EQ(trans.m[3][2], 0.0);
	EXPECT_DOUBLE_EQ(trans.m[3][3], 1.0);

	Matrix4 rot = Matrix4::createRotationZ(0.5);
	EXPECT_DOUBLE_EQ(rot.m[3][0], 0.0);
	EXPECT_DOUBLE_EQ(rot.m[3][1], 0.0);
	EXPECT_DOUBLE_EQ(rot.m[3][2], 0.0);
	EXPECT_DOUBLE_EQ(rot.m[3][3], 1.0);
}

