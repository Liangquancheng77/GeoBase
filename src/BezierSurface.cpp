#include "../include/GeoBase/BezierSurface.h"

// 张量积曲面求值
Point3 BezierSurface::evaluate(double u, double v) const {
	std::vector<Point3> temp;
	temp.reserve(m_rows);

	for (size_t i = 0; i < m_rows; ++i)
	{
		BezierCurve rowCurve(m_controlGrid[i]);
		temp.emplace_back(rowCurve.evaluate(u));
	}
	BezierCurve colCurve(temp);
	return colCurve.evaluate(v);
}