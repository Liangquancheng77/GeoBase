#include "../include/GeoBase/BezierCurve.h"


// de Casteljau算法求曲线上参数t处的点
Point3 BezierCurve::evaluate(double t) const {
	std::vector<Point3> points = m_controlPoints;
	int len = points.size();
	for (int k = 1; k < len; ++k)
	{
		for (int i = 0; i < len - k; ++i)
		{
			points[i] = points[i] * (1.0 - t) + points[i + 1] * t;
		}
	}

	return points[0];

}

// 升阶：增加一个控制点，曲线形状不变
void BezierCurve::elevateDegree() {
	int len = m_controlPoints.size();
	std::vector<Point3> newPoints(len + 1);
	newPoints[0] = m_controlPoints[0];
	newPoints[len] = m_controlPoints[len - 1];
	for (size_t i = 1; i < len; i++)
	{
		newPoints[i] = m_controlPoints[i - 1] * (i / (len + 1)) + m_controlPoints[i] * (1 - i * (len + 1));
	}
	m_controlPoints = std::move(newPoints);
}

// 降阶（近似）
//void BezierCurve::reduceDegree() {
//
//}

// 分裂：在t处将曲线分成两条独立曲线
//std::pair<BezierCurve, BezierCurve> BezierCurve::split(double t) const {
//
//}

// 访问器
const std::vector<Point3>& BezierCurve::controlPoints() const {
	return m_controlPoints;
}

// 阶数 = 控制点数 - 1
size_t BezierCurve::degree() const {
	return m_controlPoints.size() - 1;
}

// 计算组合数 C(n,k) 用浮点避免整数溢出
double binomial(int n, int k) {
	if (k < 0 || k > n) return 0.0;
	if (k == 0 || k == n) return 1.0;
	double num = 1.0; // 分子
	double den = 1.0; // 分母
	for (int i = 0; i < k; ++i)
	{
		num *= n - i;
		den *= k - i;
	}
	return num / den;
}

// 直接计算伯恩斯坦多项式
Point3 BezierCurve::directBernstein(double t) const {
	Vector3 result;
	int len = m_controlPoints.size();
	int n = len - 1;
	for (int k = 0; k < len; ++k)
	{
		result = result + m_controlPoints[k] * binomial(n, k) * std::pow(t, k) * std::pow(1 - t, n - k);
	}
	return result;
}

// 曲线导数
Vector3 BezierCurve::derivative(double t) const {
	int n = m_controlPoints.size() - 1;
	if (n < 1) return Vector3(0, 0, 0);
	std::vector<Point3> derivControlPoints(n);
	for (int i = 0; i < n; ++i)
	{
		derivControlPoints.push_back((m_controlPoints[i + 1] - m_controlPoints[i]) * n);
	}
	return BezierCurve(derivControlPoints).evaluate(t);
}
