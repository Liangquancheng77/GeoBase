#pragma once
#include "Vector3.h"

class BSplineCurve {

public:

	BSplineCurve(std::vector<Point3> controlPoints,
		std::vector<double> knots,
		int degree) :m_controlPoints(controlPoints), m_knots(knots), m_degree(degree){}

	// B样条求值(de Boor 算法)
	Point3 evaluate(double t) const;

	// B样条求值(递归基函数)
	Point3 evaluate2(double t) const;

	// 基函数计算
	double basisFunction(int i, int p, double t) const;

	// 节点区间查找
	int findSpan(double t) const;

	// 节点插入
	void insertKnot(double t);

private:

	std::vector<Point3> m_controlPoints;

	std::vector<double> m_knots;

	int m_degree;

};