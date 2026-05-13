#include "Matrix3.h"
#include "Matrix4.h"   // 需要 Matrix4 定义
#include <cmath>
#include <cstring>

// ----- 构造与单位矩阵 -----
Matrix3::Matrix3() {
    setIdentity();
}

Matrix3::Matrix3(double val) {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            m[i][j] = val;
}

Matrix3::Matrix3(const double arr[3][3]) {
    memcpy(m, arr, sizeof(double) * 9);
}

Matrix3 Matrix3::identity() {
    Matrix3 res;
    res.setIdentity();
    return res;
}

void Matrix3::setIdentity() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            m[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
}

// ----- 三轴旋转矩阵 -----
Matrix3 Matrix3::rotationX(double rad) {
    double c = std::cos(rad), s = std::sin(rad);
    Matrix3 res;
    res.m[0][0] = 1.0; res.m[0][1] = 0.0; res.m[0][2] = 0.0;
    res.m[1][0] = 0.0; res.m[1][1] = c;   res.m[1][2] = -s;
    res.m[2][0] = 0.0; res.m[2][1] = s;   res.m[2][2] = c;
    return res;
}

void Matrix3::setRotationX(double rad) {
    *this = rotationX(rad);
}

Matrix3 Matrix3::rotationY(double rad) {
    double c = std::cos(rad), s = std::sin(rad);
    Matrix3 res;
    res.m[0][0] = c;   res.m[0][1] = 0.0; res.m[0][2] = s;
    res.m[1][0] = 0.0; res.m[1][1] = 1.0; res.m[1][2] = 0.0;
    res.m[2][0] = -s;  res.m[2][1] = 0.0; res.m[2][2] = c;
    return res;
}

void Matrix3::setRotationY(double rad) {
    *this = rotationY(rad);
}

Matrix3 Matrix3::rotationZ(double rad) {
    double c = std::cos(rad), s = std::sin(rad);
    Matrix3 res;
    res.m[0][0] = c;   res.m[0][1] = -s;  res.m[0][2] = 0.0;
    res.m[1][0] = s;   res.m[1][1] = c;   res.m[1][2] = 0.0;
    res.m[2][0] = 0.0; res.m[2][1] = 0.0; res.m[2][2] = 1.0;
    return res;
}

void Matrix3::setRotationZ(double rad) {
    *this = rotationZ(rad);
}

// ----- 运算 -----
Matrix3 Matrix3::operator*(const Matrix3& other) const {
    Matrix3 res;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            res.m[i][j] = m[i][0] * other.m[0][j] +
                m[i][1] * other.m[1][j] +
                m[i][2] * other.m[2][j];
        }
    }
    return res;
}

Vector3 Matrix3::operator*(const Vector3& v) const {
    return Vector3(
        m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
        m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
        m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
    );
}

Matrix3 Matrix3::transpose() const {
    Matrix3 res;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            res.m[i][j] = m[j][i];
    return res;
}

// ----- 与 Matrix4 互转 -----
Matrix4 Matrix3::toMatrix4() const {
    Matrix4 result;
    // 先将结果设为单位矩阵（第四行[0,0,0,1]）
    result.setIdentity();
    // 复制左上角 3x3
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            result.m[i][j] = m[i][j];
    return result;
}

void Matrix3::fromMatrix4(const Matrix4& mat4) {
    // 提取左上角 3x3
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            m[i][j] = mat4.m[i][j];
}

// ----- 工具 -----
bool Matrix3::isApprox(const Matrix3& other, double eps) const {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (std::fabs(m[i][j] - other.m[i][j]) > eps)
                return false;
    return true;
}

double Matrix3::determinant() const {
    // 按第一行展开
    long double a = m[0][0], b = m[0][1], c = m[0][2];
    long double d = m[1][0], e = m[1][1], f = m[1][2];
    long double g = m[2][0], h = m[2][1], i = m[2][2];
    long double a1 = a * (e * i - f * h);
    long double a2 = -b * (d * i - f * g);
    long double a3 = c * (d * h - e * g);
    long double det1 = a1+a2;
    long double det = det1 +a3;
    //long double det = a * (e * i - f * h)
    //    - b * (d * i - f * g)
    //    + c * (d * h - e * g);
    return static_cast<double>(det);
}

Matrix3 Matrix3::inverse() const {
    double det = determinant();
    // 假设 det != 0，实际使用时请检查
    double invDet = 1.0 / det;
    // 计算伴随矩阵（余子式矩阵的转置）
    double a = m[0][0], b = m[0][1], c = m[0][2];
    double d = m[1][0], e = m[1][1], f = m[1][2];
    double g = m[2][0], h = m[2][1], i = m[2][2];

    Matrix3 adj;
    adj.m[0][0] = (e * i - f * h);
    adj.m[0][1] = -(d * i - f * g);
    adj.m[0][2] = (d * h - e * g);
    adj.m[1][0] = -(b * i - c * h);
    adj.m[1][1] = (a * i - c * g);
    adj.m[1][2] = -(a * h - b * g);
    adj.m[2][0] = (b * f - c * e);
    adj.m[2][1] = -(a * f - c * d);
    adj.m[2][2] = (a * e - b * d);

    // 乘以 1/det 得到逆矩阵
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            adj.m[i][j] *= invDet;
    return adj;
}

// 欧拉角转正交矩阵
Matrix3 Matrix3::fromEulerAngles(const EulerAngles angles) {

    Matrix3 my = Matrix3::rotationY(angles.yaw);
    Matrix3 mx = Matrix3::rotationX(angles.pitch);
    Matrix3 mz = Matrix3::rotationZ(angles.roll);

    return mz * mx * my;

}

// 正交矩阵转欧拉角
EulerAngles Matrix3::toEulerAngles() const {
    EulerAngles angles;
    // 提取矩阵元素（按你的 Matrix3 布局，假设 m[row][col]）
    double r00 = m[0][0], r01 = m[0][1], r02 = m[0][2];
    double r10 = m[1][0], r11 = m[1][1], r12 = m[1][2];
    double r20 = m[2][0], r21 = m[2][1], r22 = m[2][2];

    // 俯仰角 pitch = asin(r21)  （范围 -π/2 ～ π/2）
    double pitch = asin(r21);
    angles.pitch = pitch;

    // 处理万向锁（pitch 接近 ±π/2）
    const double eps = 1e-9;
    if (fabs(fabs(pitch) - PI / 2.0) < eps) {
        // 万向锁：令 roll = 0，从矩阵其他元素计算 yaw
        angles.roll = 0.0;
        // 此时 r20 = -cos(pitch)*sin(yaw) ≈ 0, r22 = cos(pitch)*cos(yaw) ≈ 0，需要另寻公式
        // 使用 r00 和 r10 来计算 yaw：
        // 当 pitch = +π/2 时: R = [ cos(yaw+roll), -sin(yaw+roll), 0; sin(yaw+roll), cos(yaw+roll), 0; 0,0,1 ]
        // 可以用 atan2(r10, r00) 得到 yaw+roll
        double yaw_roll = atan2(r10, r00);
        angles.yaw = yaw_roll;  // 因为 roll = 0
    }
    else {
        // 正常情况
        angles.roll = atan2(-r01, r11);
        angles.yaw = atan2(-r20, r22);
    }

    // 将角度归一化到 [-π, π] 范围（可选）
    angles.yaw = fmod(angles.yaw + PI, 2 * PI) - PI;
    angles.roll = fmod(angles.roll + PI, 2 * PI) - PI;
    // pitch 已在 [-π/2, π/2] 内，无需归一化

    return angles;
}