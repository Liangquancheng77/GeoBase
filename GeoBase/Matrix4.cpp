#include "Matrix4.h"
#include <cstring>
#include <cmath>


// 默认构造函数
// 直接初始化为单位矩阵，保证矩阵一开始就是合法状态
Matrix4::Matrix4() {
	setIdentity();
}

// 将矩阵设置为 单位矩阵
// 单位矩阵：对角线为1，其余为0，作用是“不做任何变换”
void Matrix4::setIdentity() {
	// 先把所有元素清零
	memset(m, 0, sizeof(m));

	// 对角线置 1
	m[0][0] = 1.0;
	m[1][1] = 1.0;
	m[2][2] = 1.0;
	m[3][3] = 1.0;
}

// ==================== 静态工厂方法：创建变换矩阵 ====================

// 创建 平移矩阵
// tx, ty, tz：分别是 X、Y、Z 轴的平移量
// 只影响点，不影响向量
Matrix4 Matrix4::createTranslation(double tx, double ty, double tz) {
	Matrix4 mat;
	mat.m[0][3] = tx;
	mat.m[1][3] = ty;
	mat.m[2][3] = tz;
	return mat;
}

// 创建 绕X轴旋转矩阵
// rad：旋转弧度（右手坐标系）
// X 不动，Y-Z 平面旋转
Matrix4 Matrix4::createRotationX(double rad) {
	Matrix4 mat;
	mat.m[1][1] = cos(rad);
	mat.m[1][2] = -sin(rad);
	mat.m[2][1] = sin(rad);
	mat.m[2][2] = cos(rad);
	return mat;
}

// 创建 绕Y轴旋转矩阵
// rad：旋转弧度（右手坐标系）
// Y 不动，X-Z 平面旋转
Matrix4 Matrix4::createRotationY(double rad) {
	Matrix4 mat;
	mat.m[0][0] = cos(rad);
	mat.m[0][2] = sin(rad);
	mat.m[2][0] = -sin(rad);
	mat.m[2][2] = cos(rad);
	return mat;
}

// 创建 绕Z轴旋转矩阵
// rad：旋转弧度（右手坐标系）
// Z 不动，X-Y 平面旋转
Matrix4 Matrix4::createRotationZ(double rad) {
	Matrix4 mat;
	mat.m[0][0] = cos(rad);
	mat.m[0][1] = -sin(rad);
	mat.m[1][0] = sin(rad);
	mat.m[1][1] = cos(rad);
	return mat;
}

// 创建 缩放矩阵
// sx, sy, sz：各轴缩放系数
// 对点和向量都生效
Matrix4 Matrix4::createScaling(double sx, double sy, double sz) {
	Matrix4 mat;
	mat.m[0][0] = sx;
	mat.m[1][1] = sy;
	mat.m[2][2] = sz;
	return mat;
}

// ==================== 设置当前矩阵为某种变换 ====================

// 设置为平移矩阵（覆盖原有数据）
void Matrix4::setTranslation(double tx, double ty, double tz) {
	setIdentity();
	m[0][3] = tx;
	m[1][3] = ty;
	m[2][3] = tz;
}

// 设置为绕X轴旋转矩阵
void Matrix4::setRotationX(double rad) {
	setIdentity();
	m[1][1] = cos(rad);
	m[1][2] = -sin(rad);
	m[2][1] = sin(rad);
	m[2][2] = cos(rad);
}

// 设置为绕Y轴旋转矩阵
void Matrix4::setRotationY(double rad) {
	setIdentity();
	m[0][0] = cos(rad);
	m[0][2] = sin(rad);
	m[2][0] = -sin(rad);
	m[2][2] = cos(rad);
}

// 设置为绕Z轴旋转矩阵
void Matrix4::setRotationZ(double rad) {
	setIdentity();
	m[0][0] = cos(rad);
	m[0][1] = -sin(rad);
	m[1][0] = sin(rad);
	m[1][1] = cos(rad);
}

// 设置为缩放矩阵
void Matrix4::setScaling(double sx, double sy, double sz) {
	setIdentity();
	m[0][0] = sx;
	m[1][1] = sy;
	m[2][2] = sz;
}

// ==================== 矩阵核心运算 ====================

// 矩阵乘法：this * other
// 用于变换组合：先应用 other，再应用 this
Matrix4 Matrix4::multiply(const Matrix4& other) const {
	Matrix4 mat;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			// 矩阵乘法公式：结果[i][j] = 左矩阵第i行 · 右矩阵第j列
			mat.m[i][j] =
				m[i][0] * other.m[0][j] +
				m[i][1] * other.m[1][j] +
				m[i][2] * other.m[2][j] +
				m[i][3] * other.m[3][j];
		}
	}
	return mat;
}

// 变换一个 3D 点（齐次坐标 w=1）
// 会受到 平移、旋转、缩放 共同影响
Vector3 Matrix4::transformPoint(const Vector3& vec) const {
	Vector3 newVec;
	newVec.x = m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z + m[0][3];
	newVec.y = m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z + m[1][3];
	newVec.z = m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z + m[2][3];
	return newVec;
}

// 变换一个 方向向量（齐次坐标 w=0）
// 只受旋转、缩放影响，**不受平移影响**
Vector3 Matrix4::transformVector(const Vector3& vec) const {
	Vector3 newVec;
	newVec.x = m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z;
	newVec.y = m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z;
	newVec.z = m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z;
	return newVec;
}

// 矩阵转置
// 行 <-> 列互换
Matrix4 Matrix4::transpose() const
{
	Matrix4 mat;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			mat.m[i][j] = m[j][i];
		}
	}
	return mat;
}

// ==================== 精度与判断 ====================

// 浮点近似相等判断
// eps：容差，默认 1e-9，解决浮点数精度误差
bool Matrix4::isEqualApprox(const Matrix4& other, double eps) const {
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (fabs(m[i][j] - other.m[i][j]) >= eps) {
				return false;
			}
		}
	}
	return true;
}

// 重载 == 运算符
// 直接使用默认容差的近似相等
bool Matrix4::operator==(const Matrix4& other) const {
	return isEqualApprox(other);
}