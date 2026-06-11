#pragma once
#include "Vector3.h"

class BezierCurve {

public:

	BezierCurve(std::vector<Point3> controlPoints) : m_controlPoints(controlPoints){}

	// de Casteljau算法求曲线上参数t处的点
	Point3 evaluate(double t) const;

	// 升阶：增加一个控制点，曲线形状不变
	void elevateDegree();

    // 降阶（近似）
    //void reduceDegree();

    // 分裂：在t处将曲线分成两条独立曲线
    //std::pair<BezierCurve, BezierCurve> split(double t) const;

    // 访问器
    const std::vector<Point3>& controlPoints() const;
    size_t degree() const;  // 阶数 = 控制点数 - 1

private:

	std::vector<Point3> m_controlPoints;

};
