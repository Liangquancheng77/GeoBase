//#include <gtest/gtest.h>
//#include "../include/GeoBase/AABB.h"
//#include "../include/GeoBase/BVH.h"
//#include "../include/GeoBase/Triangle3.h"
//#include <vector>
//#include <limits>
//#include <ctime>
//#include <random>
//#include <fstream>
//#include <cmath>
//#include <iostream>
//#include <iomanip>
//using namespace std;
//
//
//
//
//// 创建少量三角形，手动验证BVH的包围盒层次是否正确。
//// 验证叶子节点的三角形索引数量不超过阈值。
//// 验证根节点的AABB包含所有三角形。
//TEST(BVHTest, PrintBVHHierarchy_ManualCheck) {
//	std::vector<Triangle3> triangles = {
//		Triangle3(Point3(3,0,1), Point3(3,0,2), Point3(3,1,1)),
//		Triangle3(Point3(2.1,0,1), Point3(2.2,0,1), Point3(2.3,1,1)),
//		Triangle3(Point3(0,0,0), Point3(1,0,0), Point3(0,1,0)),
//		Triangle3(Point3(2,0,1), Point3(2,0,1), Point3(2,1,1)),
//		Triangle3(Point3(3.1,0,1), Point3(3.2,0,1), Point3(3.3,1,1)),
//		Triangle3(Point3(1.2,0,0), Point3(1.1,1,0), Point3(1.3,1,0))
//	};
//
//	BVHNode* root = buildBVH(triangles, 0, triangles.size(), 0, 10);
//
//	cout << "BVH Hierarchy:" << endl;
//}
//
//
//
//
//
//
//// 生成单位球面上的随机点
////Point3 randomPointOnSphere(float radius = 1.0f) {
////    float theta = randomFloat(0, 2 * PI);
////    float phi = randomFloat(0, PI);
////    float x = radius * sin(phi) * cos(theta);
////    float y = radius * sin(phi) * sin(theta);
////    float z = radius * cos(phi);
////    return Point3(x, y, z);
////}
////
//// // ---------------------- 2. 生成测试场景：随机球体三角形 ----------------------
////std::vector<Triangle3> generateSphereTriangles(int triangleCount, float radius = 1.0f) {
////    std::vector<Triangle3> tris;
////    tris.reserve(triangleCount);
////
////    for (int i = 0; i < triangleCount; ++i) {
////        // 生成三个随机点在球面上，组成三角形
////        Point3 v0 = randomPointOnSphere(radius);
////        Point3 v1 = randomPointOnSphere(radius);
////        Point3 v2 = randomPointOnSphere(radius);
////        tris.emplace_back(v0, v1, v2);
////    }
////    return tris;
////}
////
////// ---------------------- 3. 生成随机射线（从原点出发，方向随机） ----------------------
////std::vector<Ray> generateRandomRays(int rayCount) {
////    std::vector<Ray> rays;
////    rays.reserve(rayCount);
////
////    for (int i = 0; i < rayCount; ++i) {
////        Point3 origin(0, 0, 0); // 射线起点（相机位置，可改）
////        Vector3 dir = randomPointOnSphere(1.0f); // 随机方向
////        rays.emplace_back(origin, dir);
////    }
////    return rays;
////}
////
////// ---------------------- 4. 暴力遍历：检查所有三角形 ----------------------
////bool intersectBruteForce(const Ray& ray, HitInfo& hit, const std::vector<Triangle3>& tris) {
////    bool hitAny = false;
////    hit.t = std::numeric_limits<double>::max(); // 重置为无穷大
////
////    for (const auto& tri : tris) {
////        HitInfo tempHit;
////        if (tri.intersect(ray, tempHit)) {
////            if (tempHit.t < hit.t) {
////                hit = tempHit;
////                hitAny = true;
////            }
////        }
////    }
////    return hitAny;
////}
////
////// ---------------------- 5. 性能对比测试（核心） ----------------------
////TEST(BVHPerformanceTest, CompareBruteForceVsBVH) {
////    // 测试参数（按你的要求）
////    const int triangleCount = 8000;  // 5000-10000，选中间值
////    const int rayCount = 1000;       // 1000条射线
////    const float sphereRadius = 5.0f;
////
////    // 1. 生成场景和射线
////    std::cout << "生成测试场景:" << triangleCount << "个三角形..." << std::endl;
////    auto tris = generateSphereTriangles(triangleCount, sphereRadius);
////    auto rays = generateRandomRays(rayCount);
////
////    // 2. 构建BVH树
////    std::cout << "构建BVH树..." << std::endl;
////    Timer buildTimer;
////    BVHNode* bvhRoot = buildBVH(tris, 0, tris.size(),0);
////    double buildTimeMs = buildTimer.elapsedMs();
////    std::cout << "BVH构建耗时: " << buildTimeMs << " ms" << std::endl;
////
////    // 3. 暴力遍历计时
////    std::cout << "开始暴力遍历测试..." << std::endl;
////    Timer bruteTimer;
////    int bruteHitCount = 0;
////    for (const auto& ray : rays) {
////        HitInfo hit;
////        if (intersectBruteForce(ray, hit, tris)) {
////            bruteHitCount++;
////        }
////    }
////    double bruteTimeMs = bruteTimer.elapsedMs();
////    std::cout << "暴力遍历耗时: " << bruteTimeMs << " ms, 命中数: " << bruteHitCount << std::endl;
////
////    // 4. BVH遍历计时
////    std::cout << "开始BVH遍历测试..." << std::endl;
////    Timer bvhTimer;
////    int bvhHitCount = 0;
////    for (const auto& ray : rays) {
////        HitInfo hit;
////        hit.t = std::numeric_limits<double>::max(); // 每次都要重置！
////        if (bvhRoot->intersect(ray, hit, tris)) {
////            bvhHitCount++;
////        }
////    }
////    double bvhTimeMs = bvhTimer.elapsedMs();
////    std::cout << "BVH遍历耗时: " << bvhTimeMs << " ms, 命中数: " << bvhHitCount << std::endl;
////
////    // 5. 验证结果正确性（命中数必须一致！）
////    //ASSERT_EQ(bruteHitCount, bvhHitCount) << "暴力和BVH命中数不一致！BVH有BUG！";
////
////    // 6. 计算加速比
////    double speedup = bruteTimeMs / bvhTimeMs;
////    std::cout << "加速比: " << std::fixed << std::setprecision(2) << speedup << "x" << std::endl;
////
////    // 7. 写入性能文档（自动生成perf_bvh.md）
////    //std::ofstream mdFile("docs/perf_bvh.md");
////    //if (mdFile.is_open()) {
////    //    mdFile << "# BVH 性能测试报告\n\n";
////    //    mdFile << "## 测试环境\n";
////    //    mdFile << "- 硬件: CPU: " << "你的CPU型号（可手动改）" << "\n";
////    //    mdFile << "- 系统: Windows 11 / Visual Studio 2022\n";
////    //    mdFile << "- 编译模式: Release / O2 优化\n\n";
////
////    //    mdFile << "## 测试参数\n";
////    //    mdFile << "- 三角形数量: " << triangleCount << "\n";
////    //    mdFile << "- 射线数量: " << rayCount << "\n";
////    //    mdFile << "- 场景: 随机球体\n\n";
////
////    //    mdFile << "## 测试结果\n";
////    //    mdFile << "| 算法       | 耗时 (ms) | 命中数 | 加速比 |\n";
////    //    mdFile << "|------------|-----------|--------|--------|\n";
////    //    mdFile << "| 暴力遍历   | " << bruteTimeMs << " | " << bruteHitCount << " | 1x |\n";
////    //    mdFile << "| BVH 遍历   | " << bvhTimeMs << " | " << bvhHitCount << " | " << speedup << "x |\n\n";
////
////    //    mdFile << "## 结论\n";
////    //    mdFile << "- BVH 构建耗时: " << buildTimeMs << " ms\n";
////    //    mdFile << "- BVH 遍历速度比暴力遍历快 **" << std::fixed << std::setprecision(0) << speedup << " 倍**，符合预期的2个数量级提升。\n";
////    //    mdFile.close();
////    //    std::cout << "性能报告已生成: docs/perf_bvh.md" << std::endl;
////    //}
////
////    // 释放内存
////    deleteBVH(bvhRoot);
////}
//
// //生成单位球面上的随机点
//Point3 randomPointOnSphere(double r = 1.0) {
//	double theta = randomDouble(0, 2 *PI);
//	double phi = randomDouble(0, PI);
//	return Point3(
//		sin(phi) * cos(theta) * r,
//		sin(phi) * sin(theta) * r,
//		cos(phi) * r
//	);
//}
//
//// 正确的随机三角形生成（分布在 [-50,50] 立方体）
//std::vector<Triangle3> generateRandomTriangles(int triangleCount) {
//	std::vector<Triangle3> tris;
//	tris.reserve(triangleCount);
//	for (int i = 0; i < triangleCount; i++) {
//		tris.emplace_back(
//			Point3(randomDouble(-50, 50), randomDouble(-50, 50), randomDouble(-50, 50)),
//			Point3(randomDouble(-50, 50), randomDouble(-50, 50), randomDouble(-50, 50)),
//			Point3(randomDouble(-50, 50), randomDouble(-50, 50), randomDouble(-50, 50))
//		);
//	}
//	return tris;
//}
//
//// 随机球体三角形
//std::vector<Triangle3> generateSphereTriangles(int triangleCount, double radius = 1.0) {
//	std::vector<Triangle3> tris;
//	for (int i = 0; i < triangleCount; i++)
//	{
//		tris.emplace_back(randomPointOnSphere(radius), randomPointOnSphere(radius), randomPointOnSphere(radius));
//	}
//	return tris;
//}
//
//// 随机射线（从原点出发，方向随机）
//std::vector<Ray> generateRandomRays(int rayCount) {
//	std::vector<Ray> rays;
//	for (int i = 0; i < rayCount; i++)
//	{
//		rays.emplace_back(Point3(0.1, 0, 0), randomPointOnSphere(1.0));
//	}
//	return rays;
//}
//
//
////程序化生成一个包含5000 - 10000个随机三角形的球体或兔子。
////发射1000条随机射线。
////分别用暴力遍历（每个三角形都检查）和BVH遍历计时。
////预期结果：BVH比暴力快两个数量级
//TEST(BVHPerformanceTest, CompareBruteForceVsBVH) {
//	std::vector<Triangle3> tris = generateSphereTriangles(10000);
//	std::vector<Ray> rays = generateRandomRays(1000);
//	// 构建BVH树计时
//	Timer buildTimer;
//	BVHNode* bvhRoot = buildBVH(tris, 0, tris.size(), 0, 32);
//	double buildTimeMs = buildTimer.elapsedMs();
//	std::cout << "BVH构建耗时: " << buildTimeMs << " ms" << std::endl;
//	Timer buildTimer2;
//	BVHNode* bvhRoot2 = buildBVH_SAH(tris, 0, tris.size(), 0, 32);
//	double buildTimeMs2 = buildTimer2.elapsedMs();
//	std::cout << "BVH_SAH构建耗时: " << buildTimeMs2 << " ms" << std::endl;
//	Timer buildTimer3;
//	BVHNode* bvhRoot3 = buildBVH_Bucket_SAH(tris, 0, tris.size(), 0, 32);
//	double buildTimeMs3 = buildTimer3.elapsedMs();
//	std::cout << "BVH_Bucket_SAH构建耗时: " << buildTimeMs3 << " ms" << std::endl;
//	// 暴力遍历计时
//	Timer bruteTimer;
//	int bruteHitCount = 0;
//	for (const auto& ray : rays) {
//		for (const auto& tri : tris) {
//			double t, u, v;
//			if (tri.intersect(ray, t, u, v))
//			{
//				bruteHitCount++;
//			}
//		}
//	}
//	double bruteTimeMs = bruteTimer.elapsedMs();
//	std::cout << "暴力遍历耗时: " << bruteTimeMs << " ms, 命中数: " << bruteHitCount << std::endl;
//
//	// BVH1遍历计时
//	Timer bvhTimer;
//	int bvhHitCount = 0;
//	for (const auto& ray : rays)
//	{
//		HitInfo hit;
//		hit.t = std::numeric_limits<double>::max(); // 每次都要重置！
//		if (bvhRoot->intersect(ray, hit, tris)) {
//			bvhHitCount++;
//		}
//	}
//	double bvhTimeMs = bvhTimer.elapsedMs();
//	std::cout << "BVH遍历耗时: " << bvhTimeMs << " ms, 命中数: " << bvhHitCount << std::endl;
//
//	// BVH2遍历计时
//	Timer bvhTimer2;
//	int bvhHitCount2 = 0;
//	for (const auto& ray : rays)
//	{
//		HitInfo hit;
//		hit.t = std::numeric_limits<double>::max(); // 每次都要重置！
//		if (bvhRoot2->intersect(ray, hit, tris)) {
//			bvhHitCount2++;
//		}
//	}
//	double bvhTimeMs2 = bvhTimer2.elapsedMs();
//	std::cout << "BVH_SAH遍历耗时: " << bvhTimeMs2 << " ms, 命中数: " << bvhHitCount2 << std::endl;
//
//	// BVH3遍历计时
//	Timer bvhTimer3;
//	int bvhHitCount3 = 0;
//	for (const auto& ray : rays)
//	{
//		HitInfo hit;
//		hit.t = std::numeric_limits<double>::max(); // 每次都要重置！
//		if (bvhRoot3->intersect(ray, hit, tris)) {
//			bvhHitCount3++;
//		}
//	}
//	double bvhTimeMs3 = bvhTimer3.elapsedMs();
//	std::cout << "BVH_Bucket_SAH遍历耗时: " << bvhTimeMs3 << " ms, 命中数: " << bvhHitCount3 << std::endl;
//	
//	// 释放内存
//	deleteBVH(bvhRoot);
//	deleteBVH(bvhRoot2);
//	deleteBVH(bvhRoot3);
//}