#include "../include/GeoBase/RationalBezierCurve.h"

// 有理曲线求值
Point3 RationalBezierCurve::evaluate(double t) const {
	std::vector<Point3> points = m_controlPoints;
	std::vector<double> weights = m_weights;
	int n = m_controlPoints.size() - 1;
	for (int i = 0; i <= n; i++)
	{
		points[i] = points[i] * m_weights[i];
	}
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n - i; ++j)
		{
			points[j] = points[j] * (1.0 - t) + points[j + 1] * t;
			weights[j] = weights[j] * (1.0 - t) + weights[j + 1] * t;
		}
	}
	return points[0] / weights[0];
}

// 访问器
const std::vector<Point3>& RationalBezierCurve::controlPoints() const {
	return m_controlPoints;
}
const std::vector<double>& RationalBezierCurve::weights() const {
	return m_weights;
}