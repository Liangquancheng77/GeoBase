#pragma once
#include "Vector3.h"

class NURBSCurve {

public:

	NURBSCurve(std::vector<Point3> controlPoints,
		std::vector<double> knots,
		int degree,
		std::vector<double> weights) :m_controlPoints(controlPoints), m_knots(knots), m_degree(degree), m_weights(weights){
	}

	// NURBS求值
	Point3 evaluate(double t) const;

	// 节点区间查找
	int findSpan(double t) const;


private:

	std::vector<Point3> m_controlPoints;

	std::vector<double> m_knots;

	int m_degree;

	std::vector<double> m_weights;

};

