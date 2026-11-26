//#include<gtest/gtest.h>
//#include <math_Matrix.hxx>
//#include <math_Vector.hxx>
//#include <iostream>
//#define private public
//#include <math_SVD.hxx>
//#undef private
//
////#include <Eigen/Dense>
//
//
//TEST(MathTest, SVDTest1) {
//    // 1. 构造 5×3 矩阵 A（索引从1开始）
//    math_Matrix A(1, 5, 1, 3);
//    // 第一列
//    A(1, 1) = 1.0; A(2, 1) = -1.0; A(3, 1) = 0.0; A(4, 1) = -1.0; A(5, 1) = 1.0;
//    // 第二列
//    A(1, 2) = 0.0; A(2, 2) = 0.0; A(3, 2) = 1.0; A(4, 2) = -1.0; A(5, 2) = 1.0;
//    // 第三列
//    A(1, 3) = -1.0; A(2, 3) = 1.0; A(3, 3) = -1.0; A(4, 3) = 2.0; A(5, 3) = -2.0;
//
//    // 2. 执行 SVD 分解
//    math_SVD svd(A);
//    if (!svd.IsDone()) {
//        std::cerr << "SVD failed." << std::endl;
//        return ;
//    }
//
//    // 3. 获取分解结果
//    const math_Matrix& U = svd.U;    // 左奇异矩阵 (5×5)
//    const math_Matrix& V = svd.V;    // 右奇异矩阵 (3×3)
//    const math_Vector& Diag = svd.Diag; // 奇异值向量 (长度3)
//
//    std::cout << "U = " << U << std::endl;
//    std::cout << "V = " << V << std::endl;
//    std::cout << "Singular values = " << Diag << std::endl;
//    //math_Matrix A_plus(1, 3, 1, 5);
//    //svd.PseudoInverse(A_plus, 1.0e-6);
//
//
//}
//
//TEST(MathTest, SVDTest2) {
//    // 1. 构造 2×2 矩阵 A（索引从1开始）
//    math_Matrix A(1, 2, 1, 2);
//    // 第一列
//    A(1, 1) = 2.0; A(2, 1) = 1.0;
//    // 第二列
//    A(1, 2) = 1.0; A(2, 2) = 2.0;
//
//    // 2. 执行 SVD 分解
//    math_SVD svd(A);
//    if (!svd.IsDone()) {
//        std::cerr << "SVD failed." << std::endl;
//        return;
//    }
//
//    // 3. 获取分解结果
//    const math_Matrix& U = svd.U;    
//    const math_Matrix& V = svd.V;    
//    const math_Vector& Diag = svd.Diag; 
//
//    std::cout << "U = " << U << std::endl;
//    std::cout << "V = " << V << std::endl;
//    std::cout << "Singular values = " << Diag << std::endl;
//
//}