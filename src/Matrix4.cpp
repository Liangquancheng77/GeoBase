#include "../include/GeoBase/Matrix4.h"
#include <cstring>
#include <cmath>
#include <stdexcept>
#include "../include/GeoBase/Matrix3.h"

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

//创建观察矩阵
Matrix4 Matrix4::createLookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {

	Vector3 forward = target - eye;
	Vector3 right = forward.cross(up);
	Vector3 up2 = right.cross(forward);
	Matrix4 mat;
	mat.m[0][0] = right.x;
	mat.m[0][1] = right.y;
	mat.m[0][2] = right.z;
	mat.m[0][3] = -right.dot(eye);
	mat.m[1][0] = up2.x;
	mat.m[1][1] = up2.y;
	mat.m[1][2] = up2.z;
	mat.m[1][3] = -up2.dot(eye);
	mat.m[2][0] = forward.x;
	mat.m[2][1] = forward.y;
	mat.m[2][2] = forward.z;
	mat.m[2][3] = -forward.dot(eye);

	return mat;

}

//仿射矩阵的逆
Matrix4 Matrix4::inverseAffine() const {
	// 1. 判断是否是仿射矩阵
	constexpr double EPS = 1e-6f;
	if (fabs(m[3][0])> EPS || fabs(m[3][1]) > EPS || fabs(m[3][2])> EPS || fabs(m[3][3]-1.0f) > EPS) {
		throw std::invalid_argument("不是仿射矩阵");
	}
	
	Matrix4 mat;

	// 2. 提取旋转矩阵的转置（逆矩阵的旋转部分）
	mat.m[0][0] = m[0][0];
	mat.m[0][1] = m[1][0];
	mat.m[0][2] = m[2][0];

	mat.m[1][0] = m[0][1];
	mat.m[1][1] = m[1][1];
	mat.m[1][2] = m[2][1];

	mat.m[2][0] = m[0][2];
	mat.m[2][1] = m[1][2];
	mat.m[2][2] = m[2][2];

	// 3. 计算 -R^T * t（逆矩阵的平移部分）
	float tx = m[0][3];
	float ty = m[1][3];
	float tz = m[2][3];

	mat.m[0][3] = -(mat.m[0][0] * tx + mat.m[0][1] * ty + mat.m[0][2] * tz);
	mat.m[1][3] = -(mat.m[1][0] * tx + mat.m[1][1] * ty + mat.m[1][2] * tz);
	mat.m[2][3] = -(mat.m[2][0] * tx + mat.m[2][1] * ty + mat.m[2][2] * tz);

	// 4. 最后一行保持 [0, 0, 0, 1]
	mat.m[3][0] = 0.0f;
	mat.m[3][1] = 0.0f;
	mat.m[3][2] = 0.0f;
	mat.m[3][3] = 1.0f;

	return mat;


}


// 欧拉角转正交矩阵
Matrix4 Matrix4::fromEulerAngles(const EulerAngles angles) {

	Matrix4 my = Matrix4::createRotationY(angles.yaw);
	Matrix4 mx = Matrix4::createRotationX(angles.pitch);
	Matrix4 mz = Matrix4::createRotationZ(angles.roll);

	return mz.multiply(mx).multiply(my);

}

// 正交矩阵转欧拉角
EulerAngles Matrix4::toEulerAngles() const {
	EulerAngles angles;

	// 提取矩阵元素
	double m00 = m[0][0], m01 = m[0][1], m02 = m[0][2];
	double m10 = m[1][0], m11 = m[1][1], m12 = m[1][2];
	double m20 = m[2][0], m21 = m[2][1], m22 = m[2][2];

	// 计算俯仰角pitch的余弦值
	double cos_pitch = sqrt(m00 * m00 + m10 * m10);
	// 判断是否进入万向锁（余弦值接近0）
	bool is_gimbal_lock = cos_pitch < 1e-6;

	if (!is_gimbal_lock) {
		// 正常情况：非万向锁
		angles.yaw = atan2(m21, m22);    // 绕Y轴偏航角
		angles.pitch = atan2(-m20, cos_pitch); // 绕X轴俯仰角
		angles.roll = atan2(m10, m00);   // 绕Z轴滚转角
	}
	else {
		// 特殊情况：万向锁（pitch ≈ ±90°）
		angles.yaw = 0.0;               // 固定偏航角为0
		angles.pitch = atan2(-m20, cos_pitch);
		angles.roll = atan2(-m12, m11);  // 重新计算滚转角
	}

	return angles;
}