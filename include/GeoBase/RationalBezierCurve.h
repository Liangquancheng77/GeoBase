#pragma once
#include "Vector3.h"

class RationalBezierCurve {

public:
	RationalBezierCurve(const std::vector<Point3>& controlPoints,
		const std::vector<double>& weights) : m_controlPoints(controlPoints), m_weights(weights){}


	// 有理曲线求值
	Point3 evaluate(double t) const;

	// 访问器
	const std::vector<Point3>& controlPoints() const;
	const std::vector<double>& weights() const;

private:
	std::vector<Point3> m_controlPoints;
	std::vector<double> m_weights;
};