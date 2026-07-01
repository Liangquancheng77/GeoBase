#pragma once
#include "BSplineCurve.h"

class BSplineSurface {

public:

	BSplineSurface(const std::vector<std::vector<Point3>>& controlGrid,
		const std::vector<double>& knotsU,
		const std::vector<double>& knotsV,
		int degreeU, int degreeV) : 
		m_controlGrid(controlGrid),
		m_knotsU(knotsU), m_knotsV(knotsV),
		m_degreeU(degreeU), m_degreeV(degreeV){}

	Point3 evaluate(double u, double v) const;

private:

	std::vector<std::vector<Point3>> m_controlGrid;
	std::vector<double> m_knotsU;
	std::vector<double> m_knotsV;
	int m_degreeU;
	int m_degreeV;

	// 节点区间查找
	int findUSpan(double u) const;
	int findVSpan(double v) const;

};
