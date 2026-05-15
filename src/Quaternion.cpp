#include "../include/GeoBase/Quaternion.h"
#include <cassert>
#include <stdexcept>

// 从欧拉角创建 Y-X-Z
Quaternion Quaternion::fromEuler(const EulerAngles& angles) {

	double cosYaw = cos(angles.yaw / 2.0);
	double sinYaw = sin(angles.yaw / 2.0);
	Quaternion quaYaw(cosYaw, 0, sinYaw, 0);

	double cosPitch = cos(angles.pitch / 2.0);
	double sinPitch = sin(angles.pitch / 2.0);
	Quaternion quaPitch(cosPitch, sinPitch, 0, 0);

	double cosRoll = cos(angles.roll / 2.0);
	double sinRoll = sin(angles.roll / 2.0);
	Quaternion quaRoll(cosRoll, 0, 0, sinRoll);

	return quaRoll*quaPitch*quaYaw.normalize();

}

// Matrix3 转四元数
Quaternion Quaternion::fromMatrix3(const Matrix3& mat) {
	// 先判断是不是是正交矩阵
	if (!mat.isOrthogonal())
	{
		throw std::invalid_argument("不是正交矩阵,不能转四元数");
	}
	double sw = std::max(0.0, 1 + mat.m[0][0] + mat.m[1][1] + mat.m[2][2]);
	double sx = std::max(0.0, 1 + mat.m[0][0] - mat.m[1][1] - mat.m[2][2]);
	double sy = std::max(0.0, 1 - mat.m[0][0] + mat.m[1][1] - mat.m[2][2]);
	double sz = std::max(0.0, 1 - mat.m[0][0] - mat.m[1][1] + mat.m[2][2]);

	//double max = std::max({ sw, sx, sy, sz });
	double w1, x1, y1, z1;
	//if (std::abs(sw - max) < EPS_ABS) {
	if (sw>sx && sw>sy && sw> sz) {
		w1 = sqrt(sw) / 2;
		x1 = (mat.m[2][1] - mat.m[1][2]) / (4 * w1);
		y1 = (mat.m[0][2] - mat.m[2][0]) / (4 * w1);
		z1 = (mat.m[1][0] - mat.m[0][1]) / (4 * w1);
	}
	//else if (std::abs(sx - max) < EPS_ABS) 
	else if (sx > sy && sx > sz)
	{
		x1 = sqrt(sx) / 2;
		w1 = (mat.m[2][1] - mat.m[1][2]) / (4 * x1);
		y1 = (mat.m[1][0] + mat.m[0][1]) / (4 * x1);
		z1 = (mat.m[0][2] + mat.m[2][0]) / (4 * x1);
	}

	//else if (std::abs(sy - max) < EPS_ABS)
	else if (sy > sz)
	{
		y1 = sqrt(sy) / 2;
		w1 = (mat.m[0][2] - mat.m[2][0]) / (4 * y1);
		x1 = (mat.m[1][0] + mat.m[0][1]) / (4 * y1);
		z1 = (mat.m[2][1] + mat.m[1][2]) / (4 * y1);
	}
	else {
		z1 = sqrt(sz) / 2;
		w1 = (mat.m[1][0] - mat.m[0][1]) / (4 * z1);
		x1 = (mat.m[0][2] + mat.m[2][0]) / (4 * z1);
		y1 = (mat.m[2][1] + mat.m[1][2]) / (4 * z1);
	}
	return Quaternion(w1, x1, y1, z1);
}

// 四元数乘法 旋转组合
Quaternion Quaternion::operator*(const Quaternion& other) const {
	Quaternion newQua;
	Vector3 vec1(x, y, z);
	Vector3 vec2(other.x, other.y, other.z);
	double newW = w * other.w - vec1.dot(vec2);
	Vector3 vec = vec2*w + vec1* other.w + vec1.cross(vec2);
	return Quaternion(newW, vec.x, vec.y, vec.z);
}

// 转3x3旋转矩阵
Matrix3 Quaternion::toMatrix3() const {
	double xx = x * x, yy = y * y, zz = z * z;
	double xy = x * y, xz = x * z, yz = y * z;
	double wx = w * x, wy = w * y, wz = w * z;

	double m[3][3];
	m[0][0] = 1.0 - 2.0 * (yy + zz);
	m[0][1] = 2.0 * (xy - wz);
	m[0][2] = 2.0 * (xz + wy);

	m[1][0] = 2.0 * (xy + wz);
	m[1][1] = 1.0 - 2.0 * (xx + zz);
	m[1][2] = 2.0 * (yz - wx);

	m[2][0] = 2.0 * (xz - wy);
	m[2][1] = 2.0 * (yz + wx);
	m[2][2] = 1.0 - 2.0 * (xx + yy);

	return Matrix3(m);
}

// 转欧拉角
EulerAngles Quaternion::toEulerAngles() const {
	// 先转3x3旋转矩阵
	Matrix3 mat = toMatrix3();
	// 再转欧拉角
	return mat.toEulerAngles();
}

// 归一化 变成单位四元数
Quaternion& Quaternion::normalize() {
	double all = w * w + x * x + y * y + z * z;
	double len = sqrt(std::fmax(0.0, all));
	if (len == 0.0) {
		throw std::invalid_argument("Cannot normalize a zero quaternion");
	}
	else
	{
		w /= len;
		x /= len;
		y /= len;
		z /= len;
	}
	return *this;
}

// 共轭
Quaternion Quaternion::conjugate() const {
	return Quaternion(w, -x, -y, -z);
}

// 求逆 单位四元数逆 = 共轭
Quaternion Quaternion::inverse() const {
	return conjugate();
}


// 从轴和角度创建四元数
Quaternion Quaternion::fromAxisAngle(const Vector3& axis, double angle) {
	double halfAngle = angle * 0.5;
	double s = std::sin(halfAngle);
	return Quaternion(
		std::cos(halfAngle),
		axis.x * s,
		axis.y * s,
		axis.z * s
	);
}

// 用四元数旋转向量
Vector3 Quaternion::rotate(const Vector3& v) const {
	// 公式：v' = q * v * q⁻¹（单位四元数q⁻¹ = q.conjugate()）
	Quaternion qv(0, v.x, v.y, v.z);
	Quaternion result = (*this) * qv * conjugate();
	return Vector3(result.x, result.y, result.z);
}