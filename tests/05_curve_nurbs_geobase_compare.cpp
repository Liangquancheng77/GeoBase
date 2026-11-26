//#include<gtest/gtest.h>
//#include "../include/GeoBase/NURBSCurve.h"
//#include <Geom_BSplineSurface.hxx>
//#include <gp_Pnt.hxx>
//#include <TColgp_Array2OfPnt.hxx>
//#include <TColStd_Array2OfReal.hxx>
//#include <TColStd_Array1OfReal.hxx>
//#include <vector>
//#include <cmath>
//
//// OCC 曲面求值与自研 NURBS 曲面求值对比
//TEST(curve_nurbs_geobase_compareTest, curve_nurbs_geobase_compareTest1) {
//    // ========== 第1步：准备控制点和节点向量（OCC 和 GeoBase 共用同一套数据）==========
//
//    const int nbUPoles = 4;  // u 方向控制点数量
//    const int nbVPoles = 4;  // v 方向控制点数量
//    const int degU = 2;      // u 方向阶数
//    const int degV = 2;      // v 方向阶数
//
//    // u 方向节点向量：[0, 0, 0, 0.5, 1, 1, 1]
//    std::vector<double> knotsU = { 0.0, 0.0, 0.0, 0.5, 1.0, 1.0, 1.0 };
//    // v 方向节点向量：[0, 0, 0, 0.5, 1, 1, 1]
//    std::vector<double> knotsV = { 0.0, 0.0, 0.0, 0.5, 1.0, 1.0, 1.0 };
//
//    // 控制点网格：u 方向沿圆弧（半径5），v 方向沿高度（0到10）
//    // m 行 = u 方向 = 4，n 列 = v 方向 = 4
//    std::vector<std::vector<Point3>> ctrlGrid(4, std::vector<Point3>(4));
//    std::vector<std::vector<double>> weights(4, std::vector<double>(4, 1.0));
//
//    for (int i = 0; i < 4; ++i) {
//        double angle = i * (3.141592653589793 / 2.0) / 3.0;  // 0 到 π/2
//        double x = 5.0 * cos(angle);
//        double y = 5.0 * sin(angle);
//        for (int j = 0; j < 4; ++j) {
//            double z = j * 10.0 / 3.0;
//            ctrlGrid[i][j] = Point3(x, y, z);
//        }
//    }
//
//    // ========== 第2步：用 OCC 构建 NURBS 曲面 ==========
//
//    TColgp_Array2OfPnt occPoles(1, nbUPoles, 1, nbVPoles);
//    TColStd_Array2OfReal occWeights(1, nbUPoles, 1, nbVPoles);
//
//    for (int i = 0; i < nbUPoles; ++i) {
//        for (int j = 0; j < nbVPoles; ++j) {
//            occPoles.SetValue(i + 1, j + 1,
//                gp_Pnt(ctrlGrid[i][j].x, ctrlGrid[i][j].y, ctrlGrid[i][j].z));
//            occWeights.SetValue(i + 1, j + 1, weights[i][j]);
//        }
//    }
//
//    // 不重复的节点序列：0，0.5，1
//    std::vector<double> knotsU_unique = { 0.0, 0.5, 1.0 };
//    std::vector<double> knotsV_unique = { 0.0, 0.5, 1.0 };
//
//    // 对应的多重度：第一个0出现3次，0.5出现1次，最后一个1出现3次
//    TColStd_Array1OfInteger occUMults(1, 3);
//    occUMults.SetValue(1, 3);
//    occUMults.SetValue(2, 1);
//    occUMults.SetValue(3, 3);
//
//    TColStd_Array1OfInteger occVMults(1, 3);
//    occVMults.SetValue(1, 3);
//    occVMults.SetValue(2, 1);
//    occVMults.SetValue(3, 3);
//
//    // 向 OCC 传递不重复节点
//    TColStd_Array1OfReal occKnotsU(1, static_cast<int>(knotsU_unique.size()));
//    TColStd_Array1OfReal occKnotsV(1, static_cast<int>(knotsV_unique.size()));
//    for (size_t i = 0; i < knotsU_unique.size(); ++i) occKnotsU.SetValue(i + 1, knotsU_unique[i]);
//    for (size_t i = 0; i < knotsV_unique.size(); ++i) occKnotsV.SetValue(i + 1, knotsV_unique[i]);
//
//    // 构造曲面
//    Handle(Geom_BSplineSurface) occSurface = new Geom_BSplineSurface(
//        occPoles, occWeights,
//        occKnotsU, occKnotsV,
//        occUMults, occVMults,
//        degU, degV, Standard_False, Standard_False);
//
//    // ========== 第3步：采样对比 ==========
//
//    const int numSamples = 20;
//    
//
//    double maxError = 0.0;
//    double avgError = 0.0;
//    int sampleCount = 0;
//
//    for (int iu = 0; iu < numSamples; ++iu) {
//        double u = static_cast<double>(iu) / (numSamples - 1); // 直接 0 到 1
//
//        for (int iv = 0; iv < numSamples; ++iv) {
//            double v = static_cast<double>(iv) / (numSamples - 1); // 直接 0 到 1
//
//            // ---- 用你的 GeoBase 求值（嵌套调用 NURBSCurve）----
//            // 第1步：对每一行在 u 方向求值，得到 m 个中间点
//            int m = static_cast<int>(ctrlGrid.size());  // 行数 = 4
//            int n = static_cast<int>(ctrlGrid[0].size());  // v方向控制点数
//
//            // 第1步：对每一列（固定v）沿u方向求值，得到n个中间点
//            std::vector<Point3> tempPoints(n);
//            for (int j = 0; j < n; ++j) {
//                // 提取第j列的控制点和权重
//                std::vector<Point3> colPoles(m);
//                std::vector<double> colWeights(m);
//                for (int i = 0; i < m; ++i) {
//                    colPoles[i] = ctrlGrid[i][j];
//                    colWeights[i] = weights[i][j];
//                }
//                NURBSCurve uCurve(colPoles, knotsU, degU, colWeights);
//                tempPoints[j] = uCurve.evaluate(u);
//            }
//
//            // 第2步：用中间点作为控制点，沿v方向求值
//            std::vector<double> unitWeights(n, 1.0);
//            NURBSCurve vCurve(tempPoints, knotsV, degV, unitWeights);
//            Point3 geoPt = vCurve.evaluate(v);
//
//            // ---- 用 OCC 求值 ----
//            gp_Pnt occPt = occSurface->Value(u, v);
//
//            // ---- 计算距离误差 ----
//            double dx = geoPt.x - occPt.X();
//            double dy = geoPt.y - occPt.Y();
//            double dz = geoPt.z - occPt.Z();
//            double error = std::sqrt(dx * dx + dy * dy + dz * dz);
//
//            maxError = std::max(maxError, error);
//            avgError += error;
//            sampleCount++;
//        }
//    }
//
//    avgError /= sampleCount;
//
//    // ========== 第4步：输出结果 ==========
//
//    std::cout << "=== NURBS Surface: GeoBase vs OCC ===" << std::endl;
//    std::cout << "Samples: " << sampleCount << std::endl;
//    std::cout << "Max error: " << maxError << std::endl;
//    std::cout << "Avg error: " << avgError << std::endl;
//
//    // 验证误差在合理范围内
//    EXPECT_LT(maxError, 1e-4);
//    EXPECT_LT(avgError, 1e-6);
//}
//
