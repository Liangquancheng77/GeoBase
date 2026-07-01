#include "../include/GeoBase/BSplineSurface.h"

Point3 BSplineSurface::evaluate(double u, double v) const {

	int spanU = findUSpan(u);
	int spanV = findUSpan(v);

	std::vector<Point3> temp;
	temp.reserve(m_controlGrid.size());

	for (int i = 0; i < spanV - m_degreeV; ++i)
	{
		temp.push_back(Point3(0, 0, 0));
	}

	for (int i = spanV - m_degreeV; i <= spanV; i++)
	{
		BSplineCurve bcU(m_controlGrid[i], m_knotsU, m_degreeU);
		temp.push_back(bcU.evaluate(u));
	}
	for (int i = spanV + 1; i < m_controlGrid.size(); i++)
	{
		temp.push_back(Point3(0, 0, 0));
	}
	BSplineCurve bcV(temp, m_knotsV, m_degreeV);

	return bcV.evaluate(v);

}

// 节点区间查找
int BSplineSurface::findUSpan(double u) const {
	int n = m_controlGrid[0].size();
	if (u >= m_knotsU[n]) return n - 1;
	if (u <= m_knotsU[m_degreeU]) return m_degreeU;
	// 二分搜索
	int low = m_degreeU, high = n;
	int mid = (low + high) / 2;
	while (u >= m_knotsU[mid + 1] || u < m_knotsU[mid]) {
		if (u < m_knotsU[mid]) high = mid;
		else low = mid;
		mid = (low + high) / 2;
	}
	return mid;
}
int BSplineSurface::findVSpan(double v) const {
	int n = m_controlGrid.size();
	if (v >= m_knotsV[n]) return n - 1;
	if (v <= m_knotsV[m_degreeV]) return m_degreeV;
	// 二分搜索
	int low = m_degreeV, high = n;
	int mid = (low + high) / 2;
	while (v >= m_knotsV[mid + 1] || v < m_knotsV[mid]) {
		if (v < m_knotsV[mid]) high = mid;
		else low = mid;
		mid = (low + high) / 2;
	}
	return mid;
}