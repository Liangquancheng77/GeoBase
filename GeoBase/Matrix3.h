#pragma once
#include "Vector3.h"

struct Matrix4;   // 前向声明，用于互转

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
};