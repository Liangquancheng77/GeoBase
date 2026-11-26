#include "Interpolation.h"

namespace GeoBase {

    std::vector<double> NodeGenerator::Equidistant(int n, double a, double b) {
        std::vector<double> nodes(n);
        for (int i = 0; i < n; ++i) {
            nodes[i] = a + (b - a) * i / (n - 1);
        }
        return nodes;
    }

    std::vector<double> NodeGenerator::Chebyshev(int n, double a, double b) {
        std::vector<double> nodes(n);
        double mid = 0.5 * (a + b);
        double half_range = 0.5 * (b - a);
        for (int k = 0; k < n; ++k) {
            // 映射公式：x_k = (a+b)/2 + (b-a)/2 * cos((2k+1)*pi / (2n))
            nodes[k] = mid + half_range * std::cos((2.0 * k + 1.0) * PI / (2.0 * n));
        }
        return nodes;
    }

    BarycentricInterpolator::BarycentricInterpolator(const std::vector<double>& x_nodes, const std::vector<double>& y_values)
        : m_x(x_nodes), m_y(y_values), m_w(x_nodes.size(), 1.0) {
        int n = m_x.size();
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                if (j != k) {
                    m_w[j] *= (m_x[j] - m_x[k]);
                }
            }
            m_w[j] = 1.0 / m_w[j];
        }
    }

    double BarycentricInterpolator::Evaluate(double x_val) const {
        int n = m_x.size();
        double numerator = 0.0, denominator = 0.0;

        for (int j = 0; j < n; ++j) {
            // 极其重要：防止当 x_val 等于某个节点时，分母产生 0 导致程序崩溃
            if (std::abs(x_val - m_x[j]) < 1e-12) {
                return m_y[j];
            }
            double term = m_w[j] / (x_val - m_x[j]);
            numerator += term * m_y[j];
            denominator += term;
        }
        return numerator / denominator;
    }

} // namespace GeoBase