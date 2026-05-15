
#pragma once
#include "Vector3.h"
#include "Common.h"

struct EulerAngles;

struct Matrix4
{

    double m[4][4];

    Matrix4();

    // 静态工厂：平移矩阵
    static Matrix4 createTranslation(double tx, double ty, double tz);

    // 静态工厂：绕X轴旋转矩阵
    static Matrix4 createRotationX(double rad);

    // 静态工厂：绕Y轴旋转矩阵
    static Matrix4 createRotationY(double rad);

    // 静态工厂：绕Z轴旋转矩阵
    static Matrix4 createRotationZ(double rad);

    // 静态工厂：缩放矩阵
    static Matrix4 createScaling(double sx, double sy, double sz);


    // 设置为单位矩阵
    void setIdentity();

    // 设置为平移矩阵
    void setTranslation(double tx, double ty, double tz);

    // 设置为绕X轴旋转矩阵
    void setRotationX(double rad);

    // 设置为绕Y轴旋转矩阵
    void setRotationY(double rad);

    // 设置为绕Z轴旋转矩阵
    void setRotationZ(double rad);

    // 设置为缩放矩阵
    void setScaling(double sx, double sy, double sz);


    // 矩阵乘法
    Matrix4 multiply(const Matrix4& other) const;


    // 变换点 w=1  
    Vector3 transformPoint(const Vector3& vec) const;

    // 变换方向向量 w=0
    Vector3 transformVector(const Vector3& vec) const;


    // 矩阵转置
    Matrix4 transpose() const;

    // 近似相等判断
    bool isEqualApprox(const Matrix4& other, double eps = 1e-9) const;

    // 全局重载 ==（使用近似相等）
    bool operator==(const Matrix4& other) const;

    //创建观察矩阵
    static Matrix4 createLookAt(const Vector3& eye, const Vector3& target, const Vector3& up = Vector3(0,1,0));

    //仿射矩阵的逆
    Matrix4 inverseAffine() const;

    //欧拉角转正交矩阵
    static Matrix4 fromEulerAngles(const EulerAngles angles);

    //正交矩阵转欧拉角
    EulerAngles toEulerAngles() const;


};
