#include "../include/GeoBase/NURBSCurve.h"

// NURBS求值
Point3 NURBSCurve::evaluate(double t) const {
	int p = m_degree;
	// 参数钳制（避免越界）
	double tMin = m_knots[p];
	double tMax = m_knots[m_controlPoints.size()];
	t = std::max(tMin, std::min(tMax, t));

	int k = findSpan(t);   // u ∈ [u_k, u_{k+1})
	std::vector<Point3> d(p + 1);
	std::vector<double> weights(p + 1);
	// 取出影响该区间的 p+1 个控制点
	for (int j = 0; j <= p; ++j) {
		double weight = m_weights[k - p + j];
		d[j] = m_controlPoints[k - p + j] * weight;
		weights[j] = weight;
	}
		
	// 递推插值
	for (int r = 1; r <= p; ++r) {
		for (int j = p; j >= r; --j) {
			double a = m_knots[k - p + j];
			double b = m_knots[k + j - r + 1];
			double alpha = 0.0;
			if (std::abs(b - a) > EPS_ABS)   // 防止除零
				alpha = (t - a) / (b - a);
			d[j] = d[j - 1] * (1.0 - alpha) + d[j] * alpha;
			weights[j] = weights[j - 1] * (1.0 - alpha) + weights[j] * alpha;
		}
	}
	return d[p] / weights[p];
}

// 节点区间查找
int NURBSCurve::findSpan(double t) const {
	int n = m_controlPoints.size();
	if (t >= m_knots[n]) return n - 1;
	if (t <= m_knots[m_degree]) return m_degree;
	// 二分搜索
	int low = m_degree, high = n;
	int mid = (low + high) / 2;
	while (t >= m_knots[mid + 1] || t < m_knots[mid]) {
		if (t < m_knots[mid]) high = mid;
		else low = mid;
		mid = (low + high) / 2;
	}
	return mid;
}



