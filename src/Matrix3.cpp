#include "../include/GeoBase/Matrix3.h"
#include "../include/GeoBase/Matrix4.h"   // 需要 Matrix4 定义
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

// 转置
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

// 求行列式
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

// 求逆
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
//EulerAngles Matrix3::toEulerAngles() const {
//
//    double r11 = m[0][0], r12 = m[0][1], r13 = m[0][2];
//    double r21 = m[1][0], r22 = m[1][1]; 
//    double r31 = m[2][0], r32 = m[2][1], r33 = m[2][2];
//
//    //double pitch = atan2(-r31,sqrt(r11*r11+r21*r21));
//
//    // 鲁棒性：防止浮点误差导致根号内为负数
//    double sqrtVal = r11 * r11 + r21 * r21;
//    sqrtVal = std::fmax(sqrtVal, 0.0);
//    double pitch = atan2(-r31, std::sqrt(sqrtVal));
//
//    EulerAngles angles;
//    angles.pitch = pitch;
//    
//    if (std::abs(std::abs(pitch)-(PI/2.0)) > EPS_ABS) {
//        //非万向锁
//        double r32 = m[2][1];
//        double r33 = m[2][2];
//        double yaw = atan2(r21, r11);
//        double roll = atan2(r32, r33);
//        angles.yaw = yaw;
//        angles.roll = roll;
//    }
//    else {
//        //万向锁,令其中之一等于0
//        double yaw = 0.0;
//        double roll;
//        if (pitch > 0.0) {
//            roll = atan2(m[0][1], m[0][2]);
//        }
//        else {
//            roll = atan2(-m[0][1], -m[0][2]);
//        }
//        angles.yaw = yaw;
//        angles.roll = roll;
//    }
//
//    // 角度归一化
//    if (angles.yaw > PI) angles.yaw -= 2 * PI;
//    if (angles.yaw < -PI) angles.yaw += 2 * PI;
//    if (angles.roll > PI) angles.roll -= 2 * PI;
//    if (angles.roll < -PI) angles.roll += 2 * PI;
//
//    return angles;
//
//
//}

EulerAngles Matrix3::toEulerAngles() const {
    double r11 = m[0][0], r12 = m[0][1], r13 = m[0][2];
    double r21 = m[1][0], r22 = m[1][1]; // r21, r23 未使用
    double r31 = m[2][0], r32 = m[2][1], r33 = m[2][2];

    // pitch = β ∈ [-π/2, π/2]
    double cosPitch = std::sqrt(r31 * r31 + r33 * r33);
    double pitch = std::atan2(r32, cosPitch);

    EulerAngles angles;
    angles.pitch = pitch;

    const double EPS = 1e-12;
    if (std::abs(cosPitch) > EPS) {   // 等价于 cosβ ≠ 0
        double yaw = std::atan2(-r31, r33);   // α
        double roll = std::atan2(-r12, r22);   // γ
        angles.yaw = yaw;
        angles.roll = roll;
    }
    else {
        // 万向锁：强制 roll = 0，求 yaw
        double yaw = std::atan2(r13, r11);
        angles.yaw = yaw;
        angles.roll = 0.0;
    }

    // 如需归一化到 (-π, π]，可保留原有代码（实际 atan2 已满足）
    return angles;
}

// 是否是正交矩阵
bool Matrix3::isOrthogonal() const {
    //Matrix3 trans = transpose();
    //Matrix3 result = trans * *this;
    //for (size_t i = 0; i < 3; i++)
    //{
    //    for (size_t j = 0; j < 3; j++)
    //    {
    //        if (i==j)
    //        {
    //            if (std::abs(result.m[i][j] - 1.0) > EPS_ABS)
    //            {
    //                return false;
    //            }
    //        }
    //        else
    //        {
    //            if (std::abs(result.m[i][j]) > EPS_ABS)
    //            {
    //                return false;
    //            }
    //        }
    //    }
    //}

    for (size_t i = 0; i < 3; i++)
    {
        double len = m[i][0] * m[i][0] + m[i][1] * m[i][1] + m[i][2] * m[i][2];
        if (std::abs(len - 1.0) > EPS_ABS) {
            return false;
        }
        // 验证行向量的正交性
        for (size_t j = i + 1; j < 3; j++)
        {
            if (std::abs(m[i][0] * m[j][0] + m[i][1] * m[j][1] + m[i][2] * m[j][2]) > EPS_ABS) {
                return false;
            }
        }
    }

    return true;
}