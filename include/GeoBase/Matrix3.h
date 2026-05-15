#pragma once
#include "Vector3.h"
#include "Common.h"

struct Matrix4;   // 前向声明，用于互转
struct EulerAngles;

struct Matrix3 {
public:
    double m[3][3];

    // 构造
    Matrix3();                              // 单位矩阵
    Matrix3(double val);                    // 所有元素填充 val
    Matrix3(const double arr[3][3]);        // 从二维数组构造

    // 静态工厂
    static Matrix3 identity();
    static Matrix3 rotationX(double rad);
    static Matrix3 rotationY(double rad);
    static Matrix3 rotationZ(double rad);

    // 运算
    Matrix3 operator*(const Matrix3& other) const;
    Vector3 operator*(const Vector3& v) const;   // 变换向量
    // 转置
    Matrix3 transpose() const;

    // 设置
    void setIdentity();
    void setRotationX(double rad);
    void setRotationY(double rad);
    void setRotationZ(double rad);

    // 与 Matrix4 互转
    Matrix4 toMatrix4() const;               // 补全第四行第四列为 [0,0,0,1]
    void fromMatrix4(const Matrix4& mat4);   // 提取左上角 3x3 子矩阵

    // 工具
    bool isApprox(const Matrix3& other, double eps = 1e-9) const;

    // 求行列式
    double determinant() const;

    // 求逆
    Matrix3 inverse() const;   // 假设行列式不为零

    //欧拉角转正交矩阵
    static Matrix3 fromEulerAngles(const EulerAngles angles);

    //正交矩阵转欧拉角
    EulerAngles toEulerAngles() const;

    // 是否是正交矩阵
    bool isOrthogonal() const;

};

//欧拉角
struct EulerAngles {
    double yaw;   // 绕Y轴（偏航），弧度制
    double pitch; // 绕X轴（俯仰），弧度制
    double roll;  // 绕Z轴（滚转），弧度制

    EulerAngles() : yaw(0),pitch(0),roll(0) {}
    EulerAngles(double _yaw, double _pitch, double _roll) : yaw(_yaw),pitch(_pitch),roll(_roll) {}

    // 转角度制方便调试
    Vector3 toDegrees() {
        return Vector3(yaw * 180 / PI, pitch * 180 / PI, roll * 180 / PI);
    }

    //判断是否接近万向锁
    bool isGimbalLocked(double eps=1e-6) {
        return fabs(fabs(pitch) - PI / 2) < eps;
    }
};