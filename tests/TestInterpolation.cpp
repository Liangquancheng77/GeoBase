//#include <gtest/gtest.h>
//#include "../include/GeoBase/Interpolation.h"
//#include <cmath>
//#include <vector>
//
//using namespace GeoBase;
//
//// 龙格函数（经典的插值测试函数）
//double RungeFunc(double x) {
//    return 1.0 / (1.0 + 25.0 * x * x);
//}
//
//// 1. 测试节点生成器：验证生成的节点数量和范围是否正确
//TEST(InterpolationTest, NodeGenerator) {
//    int n = 11;
//    auto x_eq = NodeGenerator::Equidistant(n, -1.0, 1.0);
//    auto x_cheb = NodeGenerator::Chebyshev(n, -1.0, 1.0);
//
//    ASSERT_EQ(x_eq.size(), n);
//    ASSERT_EQ(x_cheb.size(), n);
//
//    // 验证等距节点的间距
//    double expected_step = 2.0 / (n - 1);
//    for (size_t i = 1; i < n; ++i) {
//        EXPECT_NEAR(x_eq[i] - x_eq[i - 1], expected_step, 1e-9);
//    }
//
//    // 验证切比雪夫零点必须落在 [-1, 1] 内
//    for (double x : x_cheb) {
//        EXPECT_GE(x, -1.0);
//        EXPECT_LE(x, 1.0);
//    }
//}
//
//// 2. 测试重心拉格朗日插值器：验证它必须精确穿过给定的节点
//TEST(InterpolationTest, ExactNodeMatch) {
//    std::vector<double> x_nodes = { -1.0, 0.0, 1.0 };
//    std::vector<double> y_values = { RungeFunc(-1.0), RungeFunc(0.0), RungeFunc(1.0) };
//
//    BarycentricInterpolator interp(x_nodes, y_values);
//
//    // 在节点上的值必须完全等于原函数值（容差 1e-9）
//    for (size_t i = 0; i < x_nodes.size(); ++i) {
//        EXPECT_NEAR(interp.Evaluate(x_nodes[i]), y_values[i], 1e-9);
//    }
//}
//
//// 3. 核心测试：对比等距节点和切比雪夫节点在边界处的震荡情况（龙格现象）
//TEST(InterpolationTest, EdgeOscillationComparison) {
//    int n = 11;
//    double x_test = 0.95; // 靠近边缘的危险区域
//
//    // 生成节点和函数值
//    auto x_eq = NodeGenerator::Equidistant(n, -1.0, 1.0);
//    auto x_cheb = NodeGenerator::Chebyshev(n, -1.0, 1.0);
//
//    std::vector<double> y_eq(n), y_cheb(n);
//    for (int i = 0; i < n; ++i) {
//        y_eq[i] = RungeFunc(x_eq[i]);
//        y_cheb[i] = RungeFunc(x_cheb[i]);
//    }
//
//    BarycentricInterpolator interp_eq(x_eq, y_eq);
//    BarycentricInterpolator interp_cheb(x_cheb, y_cheb);
//
//    // 计算真实值
//    double true_val = RungeFunc(x_test);
//
//    // 计算各自插值的误差
//    double err_eq = std::abs(interp_eq.Evaluate(x_test) - true_val);
//    double err_cheb = std::abs(interp_cheb.Evaluate(x_test) - true_val);
//
//    std::cout << "\n[Edge Test at x=" << x_test << "]" << std::endl;
//    std::cout << "Equidistant interpolation error: " << err_eq << std::endl;
//    std::cout << "Chebyshev interpolation error: " << err_cheb << std::endl;
//
//    // 核心断言：切比雪夫节点的精度必须严格优于等距节点（这是龙格现象的铁证）
//    EXPECT_LT(err_cheb, err_eq) << "Warning: Chebyshev nodes should be more stable than equidistant nodes!";
//
//    // 额外验证：等距节点在这里大概率会爆炸（误差大于 1.0）
//    // 根据不同的插值算法实现，这个值可能会略有偏差，但通常都会非常大
//    EXPECT_GT(err_eq, 1.0) << "Edge oscillation for equidistant nodes did not occur as expected, check implementation.";
//    // 而切比雪夫节点应该保持非常小的误差
//    EXPECT_LT(err_cheb, 0.1) << "Error at edges for Chebyshev nodes is too large, check formula!";
//}#include <gtest/gtest.h>
//#include "../include/GeoBase/Interpolation.h"
//#include <cmath>
//#include <vector>
//
//using namespace GeoBase;
//
//// 龙格函数（经典的插值测试函数）
//double RungeFunc(double x) {
//    return 1.0 / (1.0 + 25.0 * x * x);
//}
//
//// 1. 测试节点生成器：验证生成的节点数量和范围是否正确
//TEST(InterpolationTest, NodeGenerator) {
//    int n = 11;
//    auto x_eq = NodeGenerator::Equidistant(n, -1.0, 1.0);
//    auto x_cheb = NodeGenerator::Chebyshev(n, -1.0, 1.0);
//
//    ASSERT_EQ(x_eq.size(), n);
//    ASSERT_EQ(x_cheb.size(), n);
//
//    // 验证等距节点的间距
//    double expected_step = 2.0 / (n - 1);
//    for (size_t i = 1; i < n; ++i) {
//        EXPECT_NEAR(x_eq[i] - x_eq[i - 1], expected_step, 1e-9);
//    }
//
//    // 验证切比雪夫零点必须落在 [-1, 1] 内
//    for (double x : x_cheb) {
//        EXPECT_GE(x, -1.0);
//        EXPECT_LE(x, 1.0);
//    }
//}
//
//// 2. 测试重心拉格朗日插值器：验证它必须精确穿过给定的节点
//TEST(InterpolationTest, ExactNodeMatch) {
//    std::vector<double> x_nodes = { -1.0, 0.0, 1.0 };
//    std::vector<double> y_values = { RungeFunc(-1.0), RungeFunc(0.0), RungeFunc(1.0) };
//
//    BarycentricInterpolator interp(x_nodes, y_values);
//
//    // 在节点上的值必须完全等于原函数值（容差 1e-9）
//    for (size_t i = 0; i < x_nodes.size(); ++i) {
//        EXPECT_NEAR(interp.Evaluate(x_nodes[i]), y_values[i], 1e-9);
//    }
//}
//
//// 3. 核心测试：对比等距节点和切比雪夫节点在边界处的震荡情况（龙格现象）
//TEST(InterpolationTest, EdgeOscillationComparison) {
//    int n = 11;
//    double x_test = 0.95; // 靠近边缘的危险区域
//
//    // 生成节点和函数值
//    auto x_eq = NodeGenerator::Equidistant(n, -1.0, 1.0);
//    auto x_cheb = NodeGenerator::Chebyshev(n, -1.0, 1.0);
//
//    std::vector<double> y_eq(n), y_cheb(n);
//    for (int i = 0; i < n; ++i) {
//        y_eq[i] = RungeFunc(x_eq[i]);
//        y_cheb[i] = RungeFunc(x_cheb[i]);
//    }
//
//    BarycentricInterpolator interp_eq(x_eq, y_eq);
//    BarycentricInterpolator interp_cheb(x_cheb, y_cheb);
//
//    // 计算真实值
//    double true_val = RungeFunc(x_test);
//
//    // 计算各自插值的误差
//    double err_eq = std::abs(interp_eq.Evaluate(x_test) - true_val);
//    double err_cheb = std::abs(interp_cheb.Evaluate(x_test) - true_val);
//
//    std::cout << "\n[Edge Test at x=" << x_test << "]" << std::endl;
//    std::cout << "Equidistant interpolation error: " << err_eq << std::endl;
//    std::cout << "Chebyshev interpolation error: " << err_cheb << std::endl;
//
//    // 核心断言：切比雪夫节点的精度必须严格优于等距节点（这是龙格现象的铁证）
//    EXPECT_LT(err_cheb, err_eq) << "Warning: Chebyshev nodes should be more stable than equidistant nodes!";
//
//    // 额外验证：等距节点在这里大概率会爆炸（误差大于 1.0）
//    // 根据不同的插值算法实现，这个值可能会略有偏差，但通常都会非常大
//    EXPECT_GT(err_eq, 1.0) << "Edge oscillation for equidistant nodes did not occur as expected, check implementation.";
//    // 而切比雪夫节点应该保持非常小的误差
//    EXPECT_LT(err_cheb, 0.1) << "Error at edges for Chebyshev nodes is too large, check formula!";
//}