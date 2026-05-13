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