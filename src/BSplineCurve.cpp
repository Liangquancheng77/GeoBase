#include "../include/GeoBase/BSplineCurve.h"

// B样条求值(de Boor 算法)
Point3 BSplineCurve::evaluate(double t) const {
	int p = m_degree;
	// 参数钳制（避免越界）
	double tMin = m_knots[p];
	double tMax = m_knots[m_controlPoints.size()];
	t = std::max(tMin, std::min(tMax, t));

	int k = findSpan(t);   // u ∈ [u_k, u_{k+1})
	std::vector<Point3> d(p + 1);
	// 取出影响该区间的 p+1 个控制点
	for (int j = 0; j <= p; ++j)
		d[j] = m_controlPoints[k - p + j];

	// 递推插值
	for (int r = 1; r <= p; ++r) {
		for (int j = p; j >= r; --j) {
			double a = m_knots[k - p + j];
			double b = m_knots[k + j - r + 1];
			double alpha = 0.0;
			if (std::abs(b - a) > EPS_ABS)   // 防止除零
				alpha = (t - a) / (b - a);
			d[j] = d[j - 1] * (1.0 - alpha) + d[j] * alpha;
		}
	}
	return d[p];
}

// B样条求值(递归基函数)
Point3 BSplineCurve::evaluate2(double t) const {
	int n = m_controlPoints.size() - 1; // 控制点最大下标
	double tMin = m_knots[m_degree];
	double tMax = m_knots[n + 1];

	// 左边界：直接返回第一个控制点
	if (t <= tMin) {
		return m_controlPoints[0];
	}
	// 右边界：直接返回最后一个控制点
	if (t >= tMax) {
		return m_controlPoints.back();
	}
	// 找到节点区间
	int span = findSpan(t);
	// 递归插值
	Point3 result(0, 0, 0);
	for (int i = span - m_degree; i <= span; ++i)
	{
		double N = basisFunction(i, m_degree, t);
		result = result + m_controlPoints[i] * N;
	}
	return result;
}

// 基函数计算
double BSplineCurve::basisFunction(int i, int p, double t) const {
	if (p == 0) {
		if (t >= m_knots[i] && t < m_knots[i + 1]) return 1.0;
		if (t == m_knots.back() && i + 1 == m_knots.size() - 1) return 1.0;
		return 0.0;
	}
	double result = 0.0;
	// 第一项分母
	double denom1 = m_knots[i + p] - m_knots[i];
	if (denom1 > EPS_ABS)
	{
		result += (t - m_knots[i]) / denom1 * basisFunction(i, p - 1, t);
	}
	// 第二项分母
	double denom2 = m_knots[i + p + 1] - m_knots[i + 1];
	if (denom2 > EPS_ABS)
	{
		result += (m_knots[i + p + 1] - t) / denom2 * basisFunction(i + 1, p - 1, t);
	}
	return result;
}

// 节点区间查找
int BSplineCurve::findSpan(double t) const {
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

// 节点插入
void BSplineCurve::insertKnot(double t) {

	const int p = m_degree;
	int k = findSpan(t);
	// 1. 被影响的旧控制点范围：i = k-p+1 ... k
	int start = k - p + 1;
	int end = k;
	// 2. 创建新控制点数组，容量 = 原数量 + 1
	std::vector<Point3> newCPs;
	newCPs.reserve(m_controlPoints.size() + 1);
	// 3. 保留 [0, start-1] 的旧控制点不变
	for (int i = 0; i <= start - 1; ++i)
	{
		newCPs.push_back(m_controlPoints[i]);
	}
	// 4. 生成 p+1 个新控制点，替换 [start, end] 的旧点
    // 先计算前 p 个新点 Q_i (i = start ... end)
	for (int i = start; i <= end; ++i)
	{
		double denom = m_knots[i + p] - m_knots[i];
		double alpha = denom > EPS_ABS ? (t - m_knots[i]) / denom : 0.0;
		newCPs.push_back(m_controlPoints[i - 1] * (1 - alpha) + m_controlPoints[i] * alpha);
	}
	// 再追加最后一个新点 Q_{k+1}，它等于旧点 P_k
	newCPs.push_back(m_controlPoints[end]);
	// 5. 保留 [end+1, n] 的旧控制点
	for (int i = end + 1; i < m_controlPoints.size(); ++i)
	{
		newCPs.push_back(m_controlPoints[i]);
	}

	// 6. 替换控制点数组
	m_controlPoints = std::move(newCPs);

	// 7. 将新节点 t 插入节点向量中正确的位置
    //    插入位置应为 k+1（位于 u_k 和 u_{k+1} 之间）
	auto it = m_knots.begin() + k + 1;
	m_knots.insert(it, t);

}