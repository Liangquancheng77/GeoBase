#pragma once
#include "Matrix3.h"
#include "Matrix4.h"

// w实部 xyz虚部，单位四元数代表纯旋转
struct Quaternion
{
    double w;
    double x;
    double y;
    double z;

    Quaternion() : w(0), x(0), y(0), z(0){}

    Quaternion(double _w, double _x, double _y, double _z) :w(_w), x(_x), y(_y), z(_z) {}



    // 转3x3旋转矩阵
    Matrix3 toMatrix3() const;

    // 转欧拉角
    EulerAngles toEulerAngles() const;

    // ===== 静态工厂 =====
    // 从欧拉角创建 Y-X-Z
    static Quaternion fromEuler(const EulerAngles& angles);

    // 从3x3旋转矩阵创建
    static Quaternion fromMatrix3(const Matrix3& mat);

    // 四元数乘法 旋转组合
    Quaternion operator*(const Quaternion& other) const;

    // 共轭
    Quaternion conjugate() const;

    // 求逆 单位四元数逆 = 共轭
    Quaternion inverse() const;

    // 归一化 变成单位四元数
    Quaternion& normalize();

    // 从轴和角度创建四元数（解决相机里的错误用法）
    static Quaternion fromAxisAngle(const Vector3& axis, double angle);

    // 用四元数旋转向量（解决 rotate 错误）
    Vector3 rotate(const Vector3& v) const;


};