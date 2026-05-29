#pragma once
#include "Vector3.h"
#include "Matrix3.h"

struct Quadric {

	double m[10];

	Quadric();  // 零矩阵

	Quadric(const Vector3& normal, const Point3& point);  // 从平面构建Q矩阵

	Quadric(double a, double b, double c, double d);// 直接从平面的四个参数构建

	Quadric& operator+=(const Quadric& other);

	Quadric operator+(const Quadric& other) const;

	// 计算在点p处的误差：p^T * Q * p
	double evaluate(const Point3& p) const;

	// 求解最优位置，返回是否可解
	bool solveOptimal(Point3& result) const;

	// 访问底层矩阵元素
	double& operator()(int row, int col);

	double operator()(int row, int col) const;

};