//#include <gtest/gtest.h>
//#include <cmath>
//#include <vector>
//#include <iostream>
//#include <fstream>
//#include <iomanip>
//#include "../include/GeoBase/Interpolation.h" 
//using namespace GeoBase;
//using namespace std;
//
//
//
//// 龙格函数（经典的插值测试函数）
//double RungeFunc(double x) {
//    return 1.0 / (1.0 + 25.0 * x * x);
//}
//
//TEST(TestInterpolationPlotter, TestInterpolationPlotter1) {
//
//    int n = 11;
//    double a = -1.0, b = 1.0;
//
//    // 1. 生成节点
//    auto x_eq = NodeGenerator::Equidistant(n, a, b);
//    auto x_cheb = NodeGenerator::Chebyshev(n, a, b);
//
//    // 2. 采集函数值
//    std::vector<double> y_eq(n), y_cheb(n);
//    for (int i = 0; i < n; ++i) {
//        y_eq[i] = RungeFunc(x_eq[i]);
//        y_cheb[i] = RungeFunc(x_cheb[i]);
//    }
//
//    // 3. 初始化插值器
//    BarycentricInterpolator interp_eq(x_eq, y_eq);
//    BarycentricInterpolator interp_cheb(x_cheb, y_cheb);
//
//    // 4. 导出数据到 CSV
//    std::ofstream file("interpolation_compare.csv");
//    if (!file.is_open()) {
//        std::cerr << "Failed to open file for writing!" << std::endl;
//        return ;
//    }
//
//    // 写入表头
//    file << "x,true_value,equidistant,chebyshev\n";
//
//    // 生成 1000 个采样点用于画图
//    int num_points = 1000;
//    for (int i = 0; i <= num_points; ++i) {
//        double x_val = a + (b - a) * i / num_points;
//        file << std::fixed << std::setprecision(6)
//            << x_val << ","
//            << RungeFunc(x_val) << ","
//            << interp_eq.Evaluate(x_val) << ","
//            << interp_cheb.Evaluate(x_val) << "\n";
//    }
//
//    file.close();
//    std::cout << "Data successfully exported to interpolation_compare.csv" << std::endl;
//
//}
//
