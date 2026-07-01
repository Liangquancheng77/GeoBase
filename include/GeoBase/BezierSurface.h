#pragma once
#include "BezierCurve.h"

class BezierSurface {

public:

	BezierSurface(const std::vector<std::vector<Point3>> controlGrid);

	// 张量积曲面求值
	Point3 evaluate(double u, double v) const;

private:

	std::vector<std::vector<Point3>> m_controlGrid;

	int m_rows;  // 行数（u方向控制点数）
	int m_cols;  // 列数（v方向控制点数）

};
