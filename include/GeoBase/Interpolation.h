#pragma once

#include <vector>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include "Common.h"

namespace GeoBase {

    // 插值节点生成器
    class NodeGenerator {
    public:
        // 生成等距节点
        static std::vector<double> Equidistant(int n, double a = -1.0, double b = 1.0);
        // 生成切比雪夫零点节点
        static std::vector<double> Chebyshev(int n, double a = -1.0, double b = 1.0);
    };

    // 重心拉格朗日插值器（工业级实现）
    class BarycentricInterpolator {
    private:
        std::vector<double> m_x;
        std::vector<double> m_y;
        std::vector<double> m_w; // 重心权重

    public:
        BarycentricInterpolator(const std::vector<double>& x_nodes, const std::vector<double>& y_values);

        // 评估插值多项式的值
        double Evaluate(double x_val) const;
    };

} // namespace GeoBase