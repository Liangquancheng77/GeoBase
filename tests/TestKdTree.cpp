#include <gtest/gtest.h>
#include "../include/GeoBase/KdTree.h"
#include "../include/GeoBase/Octree.h"
#include "../include/GeoBase/Common.h"

// 插入3个点，查询最近邻，验证返回正确的点
TEST(KdTreeTest, KdTreeTest1) {

	std::vector<Point3> points = { {0, 0, 0}, {1, 1, 1}, {2, 2, 2} };
	KdTree kdTree(points);
	Point3 point = kdTree.nearestNeighbor(Point3(1.1, 1.1, 1.1));
	EXPECT_EQ(point, points[1]);
}

// 插入100个随机点，暴力验证最近邻结果
TEST(KdTreeTest, KdTreeTest2) {
	std::vector<Point3> points;
	for (int i = 0; i < 100; ++i)
	{
		points.emplace_back(randomDouble(0, 100), randomDouble(0, 100), randomDouble(0, 100));
	}
	KdTree kdTree(points);
	Point3 query(randomDouble(0, 100), randomDouble(0, 100), randomDouble(0, 100));
	Point3 nn = kdTree.nearestNeighbor(query);
	double bestDistSq = (nn - query).lengthSquared();
	for (const Point3& p : points)
	{
		double distSq = (p - query).lengthSquared();
		EXPECT_GE(distSq, bestDistSq);
	}
}

// 对比Kd-tree和Octree和暴力查询在10000个随机点上的最近邻查询耗时,同时对比构建Kd-tree和Octree的耗时
//TEST(KdTreeTest, KdTreeVsOctree) {
//	std::vector<Point3> points;
//	for (int i = 0; i < 10000; ++i)
//	{
//		points.emplace_back(randomDouble(0, 100), randomDouble(0, 100), randomDouble(0, 100));
//	}
//
//	Timer kdBuildTimer;
//	KdTree kdTree(points);
//	double kdBuildTime = kdBuildTimer.elapsedMs();
//
//	Timer octreeBuildTimer;
//	Octree octree(points);
//	double octreeBuildTime = octreeBuildTimer.elapsedMs();
//
//	Point3 query(randomDouble(0, 100), randomDouble(0, 100), randomDouble(0, 100));
//	Timer timer;
//	Point3 nnKd = kdTree.nearestNeighbor(query);
//	double kdTime = timer.elapsedMs();
//	timer.reset();
//	int idxOctree = octree.nearestK(query, 1)[0];
//	Point3 nnOctree = points[idxOctree];
//	double octreeTime = timer.elapsedMs();
//	Timer  bruteTimer;
//	Point3 nnBrute;
//	double bestDistSq = std::numeric_limits<double>::max();
//	for (const Point3& p : points)
//	{
//		double distSq = (p - query).lengthSquared();
//		if (distSq < bestDistSq)
//		{
//			bestDistSq = distSq;
//			nnBrute = p;
//		}
//	}
//	double bruteTime = bruteTimer.elapsedMs();
//	EXPECT_EQ(nnKd, nnOctree);
//	EXPECT_EQ(nnKd, nnBrute);
//	std::cout << "Kd-tree build time: " << kdBuildTime << " ms\n";
//	std::cout << "Octree build time: " << octreeBuildTime << " ms\n";
//	std::cout << "Kd-tree NN time: " << kdTime << " ms\n";
//	std::cout << "Octree NN time: " << octreeTime << " ms\n";
//	std::cout << "Brute-force NN time: " << bruteTime << " ms\n";
//}

// 测试K=5查询，验证结果按距离排序正确
TEST(KdTreeTest, KdTreeTest3) {
	std::vector<Point3> points;
	for (int i = 0; i < 100; ++i)
	{
		points.emplace_back(randomDouble(0, 100), randomDouble(0, 100), randomDouble(0, 100));
	}
	KdTree kdTree(points);
	Point3 query(randomDouble(0, 100), randomDouble(0, 100), randomDouble(0, 100));
	std::vector<Point3> knn = kdTree.nearestK(query, 5);
	std::vector<std::pair<double, Point3>> distPoints;
	for (const Point3& p : points)
	{
		double distSq = (p - query).lengthSquared();
		distPoints.emplace_back(distSq, p);
	}
	std::sort(distPoints.begin(), distPoints.end(), [](const auto& a, const auto& b) {
		return a.first < b.first;
		});
	for (size_t i = 0; i < knn.size(); ++i)
	{
		EXPECT_EQ(knn[i], distPoints[i].second);
	}


}

// 所有点坐标相同，验证查询不崩溃，返回距离0
TEST(KdTreeTest, Degenerate_AllPointsSame) {
	std::vector<Point3> points(100, Point3(1, 1, 1));
	KdTree kdTree(points);
	Point3 query(1, 1, 1);
	Point3 nn = kdTree.nearestNeighbor(query);
	EXPECT_EQ(nn, Point3(1, 1, 1));
}

// 均匀分布点云下分别用暴力、Octree、Kd-tree执行最近邻查询的耗时对比，
// 验证Kd-tree和Octree相较于暴力查询的加速比，同时验证Kd-tree和Octree的查询结果完全一致
TEST(KdTreeTest, KdTreeTest4) {
	std::vector<Point3> points;
	// 均匀分布点云
	for (int i = 0; i < 10000; ++i)
	{
		points.emplace_back(randomDouble(0, 100), randomDouble(0, 100), randomDouble(0, 100));
	}
	KdTree kdTree(points);
	Octree octree(points);
	Point3 query(randomDouble(0, 100), randomDouble(0, 100), randomDouble(0, 100));
	Timer timer;
	Point3 nnKd = kdTree.nearestNeighbor(query);
	double kdTime = timer.elapsedMs();
	timer.reset();
	int idxOctree = octree.nearestK(query, 1)[0];
	Point3 nnOctree = points[idxOctree];
	double octreeTime = timer.elapsedMs();
	Timer  bruteTimer;
	Point3 nnBrute;
	double bestDistSq = std::numeric_limits<double>::max();
	for (const Point3& p : points)
	{
		double distSq = (p - query).lengthSquared();
		if (distSq < bestDistSq)
		{
			bestDistSq = distSq;
			nnBrute = p;
		}
	}
	double bruteTime = bruteTimer.elapsedMs();
	EXPECT_EQ(nnKd, nnOctree);
	EXPECT_EQ(nnKd, nnBrute);
	std::cout << "Kd-tree NN time: " << kdTime << " ms\n";
	std::cout << "Octree NN time: " << octreeTime << " ms\n";
	std::cout << "Brute-force NN time: " << bruteTime << " ms\n";
}

// 聚簇分布点云下分别用暴力、Octree、Kd-tree执行最近邻查询的耗时对比，
// 验证Kd-tree和Octree相较于暴力查询的加速比，同时验证Kd-tree和Octree的查询结果完全一致
TEST(KdTreeTest, KdTreeTest5) {
	// 聚簇分布点云
	std::vector<Point3> points;
	for (int i = 0; i < 10; ++i) {
		Point3 clusterCenter(randomDouble(0, 100), randomDouble(0, 100), randomDouble(0, 100));
		for (int j = 0; j < 1000; ++j) {
			points.emplace_back(clusterCenter.x + randomDouble(-1, 1), clusterCenter.y + randomDouble(-1, 1), clusterCenter.z + randomDouble(-1, 1));
		}
	}
	KdTree kdTree(points);
	Octree octree(points);
	Point3 query(randomDouble(0, 100), randomDouble(0, 100), randomDouble(0, 100));
	Timer timer;
	Point3 nnKd = kdTree.nearestNeighbor(query);
	double kdTime = timer.elapsedMs();
	timer.reset();
	int idxOctree = octree.nearestK(query, 1)[0];
	Point3 nnOctree = points[idxOctree];
	double octreeTime = timer.elapsedMs();
	Timer  bruteTimer;
	Point3 nnBrute;
	double bestDistSq = std::numeric_limits<double>::max();
	for (const Point3& p : points)
	{
		double distSq = (p - query).lengthSquared();
		if (distSq < bestDistSq)
		{
			bestDistSq = distSq;
			nnBrute = p;
		}
	}
	double bruteTime = bruteTimer.elapsedMs();
	EXPECT_EQ(nnKd, nnOctree);
	EXPECT_EQ(nnKd, nnBrute);
	std::cout << "Kd-tree NN time: " << kdTime << " ms\n";
	std::cout << "Octree NN time: " << octreeTime << " ms\n";
	std::cout << "Brute-force NN time: " << bruteTime << " ms\n";
}

// 稀疏分布点云下分别用暴力、Octree、Kd-tree执行最近邻查询的耗时对比，
// 验证Kd-tree和Octree相较于暴力查询的加速比，同时验证Kd-tree和Octree的查询结果完全一致
TEST(KdTreeTest, KdTreeTest6) {
	// 稀疏分布点云
	std::vector<Point3> points;
	for (int i = 0; i < 100; ++i) {
		points.emplace_back(randomDouble(0, 100), randomDouble(0, 100), randomDouble(0, 100));
	}
	KdTree kdTree(points);
	Octree octree(points);
	Point3 query(randomDouble(0, 100), randomDouble(0, 100), randomDouble(0, 100));
	Timer timer;
	Point3 nnKd = kdTree.nearestNeighbor(query);
	double kdTime = timer.elapsedMs();
	timer.reset();
	int idxOctree = octree.nearestK(query, 1)[0];
	Point3 nnOctree = points[idxOctree];
	double octreeTime = timer.elapsedMs();
	Timer  bruteTimer;
	Point3 nnBrute;
	double bestDistSq = std::numeric_limits<double>::max();
	for (const Point3& p : points)
	{
		double distSq = (p - query).lengthSquared();
		if (distSq < bestDistSq)
		{
			bestDistSq = distSq;
			nnBrute = p;
		}
	}
	double bruteTime = bruteTimer.elapsedMs();
	EXPECT_EQ(nnKd, nnOctree);
	EXPECT_EQ(nnKd, nnBrute);
	std::cout << "Kd-tree NN time: " << kdTime << " ms\n";
	std::cout << "Octree NN time: " << octreeTime << " ms\n";
	std::cout << "Brute-force NN time: " << bruteTime << " ms\n";
}

// 均匀分布点云下分别用暴力、Octree、Kd-tree执行半径查询的耗时对比，
// 验证Kd-tree和Octree相较于暴力查询的加速比，同时验证Kd-tree和Octree的查询结果完全一致
TEST(KdTreeTest, KdTreeTest7) {
	std::vector<Point3> points;
	// 均匀分布点云
	for (int i = 0; i < 10000; ++i)
	{
		points.emplace_back(randomDouble(0, 100), randomDouble(0, 100), randomDouble(0, 100));
	}
	KdTree kdTree(points);
	Octree octree(points);
	Point3 query(randomDouble(0, 100), randomDouble(0, 100), randomDouble(0, 100));
	double radius = 10.0; // 设置查询半径
	Timer timer;
	std::vector<Point3> kdResults = kdTree.radiusQuery(query, radius);
	double kdTime = timer.elapsedMs();
	timer.reset();
	std::vector<int> octreeIndices = octree.radiusQuery(query, radius);
	std::vector<Point3> octreeResults;
	for (int idx : octreeIndices) {
		octreeResults.push_back(points[idx]);
	}
	double octreeTime = timer.elapsedMs();
	Timer  bruteTimer;
	std::vector<Point3> bruteResults;
	for (const Point3& p : points)
	{
		double distSq = (p - query).lengthSquared();
		if (distSq <= radius * radius)
		{
			bruteResults.push_back(p);
		}
	}
	double bruteTime = bruteTimer.elapsedMs();
	// 验证结果完全一致（不要求顺序）
	std::sort(kdResults.begin(), kdResults.end(), [](const Point3& a, const Point3& b) {
		if (a.x != b.x) return a.x < b.x;
		if (a.y != b.y) return a.y < b.y;
		return a.z < b.z;
		});
	std::sort(octreeResults.begin(), octreeResults.end(), [](const Point3& a, const Point3& b) {
		if (a.x != b.x) return a.x < b.x;
		if (a.y != b.y) return a.y < b.y;
		return a.z < b.z;
		});
	std::sort(bruteResults.begin(), bruteResults.end(), [](const Point3& a, const Point3& b) {
		if (a.x != b.x) return a.x < b.x;
		if (a.y != b.y) return a.y < b.y;
		return a.z < b.z;
		});
	EXPECT_EQ(kdResults, octreeResults);
	EXPECT_EQ(kdResults, bruteResults);
	std::cout << "Kd-tree radius query time: " << kdTime << " ms\n";
	std::cout << "Octree radius query time: " << octreeTime << " ms\n";
	std::cout << "Brute-force radius query time: " << bruteTime << " ms\n";
}

// 聚簇分布点云下分别用暴力、Octree、Kd-tree执行半径查询的耗时对比，
// 验证Kd-tree和Octree相较于暴力查询的加速比，同时验证Kd-tree和Octree的查询结果完全一致
TEST(KdTreeTest, KdTreeTest8) {
	// 聚簇分布点云
	std::vector<Point3> points;
	for (int i = 0; i < 10; ++i) {
		Point3 clusterCenter(randomDouble(0, 100), randomDouble(0, 100), randomDouble(0, 100));
		for (int j = 0; j < 1000; ++j) {
			points.emplace_back(clusterCenter.x + randomDouble(-1, 1), clusterCenter.y + randomDouble(-1, 1), clusterCenter.z + randomDouble(-1, 1));
		}
	}
	KdTree kdTree(points);
	Octree octree(points);
	Point3 query(randomDouble(0, 100), randomDouble(0, 100), randomDouble(0, 100));
	double radius = 10.0; // 设置查询半径
	Timer timer;
	std::vector<Point3> kdResults = kdTree.radiusQuery(query, radius);
	double kdTime = timer.elapsedMs();
	timer.reset();
	std::vector<int> octreeIndices = octree.radiusQuery(query, radius);
	std::vector<Point3> octreeResults;
	for (int idx : octreeIndices) {
		octreeResults.push_back(points[idx]);
	}
	double octreeTime = timer.elapsedMs();
	Timer  bruteTimer;
	std::vector<Point3> bruteResults;
	for (const Point3& p : points)
	{
		double distSq = (p - query).lengthSquared();
		if (distSq <= radius * radius)
		{
			bruteResults.push_back(p);
		}
	}
	double bruteTime = bruteTimer.elapsedMs();
	// 验证结果完全一致（不要求顺序）
	std::sort(kdResults.begin(), kdResults.end(), [](const Point3& a, const Point3& b) {
		if (a.x != b.x) return a.x < b.x;
		if (a.y != b.y) return a.y < b.y;
		return a.z < b.z;
		});
	std::sort(octreeResults.begin(), octreeResults.end(), [](const Point3& a, const Point3& b) {
		if (a.x != b.x) return a.x < b.x;
		if (a.y != b.y) return a.y < b.y;
		return a.z < b.z;
		});
	std::sort(bruteResults.begin(), bruteResults.end(), [](const Point3& a, const Point3& b) {
		if (a.x != b.x) return a.x < b.x;
		if (a.y != b.y) return a.y < b.y;
		return a.z < b.z;
		});
	EXPECT_EQ(kdResults, octreeResults);
	EXPECT_EQ(kdResults, bruteResults);
	std::cout << "Kd-tree radius query time: " << kdTime << " ms\n";
	std::cout << "Octree radius query time: " << octreeTime << " ms\n";
	std::cout << "Brute-force radius query time: " << bruteTime << " ms\n";
}

// 稀疏分布点云下分别用暴力、Octree、Kd-tree执行半径查询的耗时对比，
// 验证Kd-tree和Octree相较于暴力查询的加速比，同时验证Kd-tree和Octree的查询结果完全一致
TEST(KdTreeTest, KdTreeTest9) {
	// 稀疏分布点云
	std::vector<Point3> points;
	for (int i = 0; i < 100; ++i) {
		points.emplace_back(randomDouble(0, 100), randomDouble(0, 100), randomDouble(0, 100));
	}
	KdTree kdTree(points);
	Octree octree(points);
	Point3 query(randomDouble(0, 100), randomDouble(0, 100), randomDouble(0, 100));
	double radius = 10.0; // 设置查询半径
	Timer timer;
	std::vector<Point3> kdResults = kdTree.radiusQuery(query, radius);
	double kdTime = timer.elapsedMs();
	timer.reset();
	std::vector<int> octreeIndices = octree.radiusQuery(query, radius);
	std::vector<Point3> octreeResults;
	for (int idx : octreeIndices) {
		octreeResults.push_back(points[idx]);
	}
	double octreeTime = timer.elapsedMs();
	Timer  bruteTimer;
	std::vector<Point3> bruteResults;
	for (const Point3& p : points)
	{
		double distSq = (p - query).lengthSquared();
		if (distSq <= radius * radius)
		{
			bruteResults.push_back(p);
		}
	}
	double bruteTime = bruteTimer.elapsedMs();
	// 验证结果完全一致（不要求顺序）
	std::sort(kdResults.begin(), kdResults.end(), [](const Point3& a, const Point3& b) {
		if (a.x != b.x) return a.x < b.x;
		if (a.y != b.y) return a.y < b.y;
		return a.z < b.z;
		});
	std::sort(octreeResults.begin(), octreeResults.end(), [](const Point3& a, const Point3& b) {
		if (a.x != b.x) return a.x < b.x;
		if (a.y != b.y) return a.y < b.y;
		return a.z < b.z;
		});
	std::sort(bruteResults.begin(), bruteResults.end(), [](const Point3& a, const Point3& b) {
		if (a.x != b.x) return a.x < b.x;
		if (a.y != b.y) return a.y < b.y;
		return a.z < b.z;
		});
	EXPECT_EQ(kdResults, octreeResults);
	EXPECT_EQ(kdResults, bruteResults);
	std::cout << "Kd-tree radius query time: " << kdTime << " ms\n";
	std::cout << "Octree radius query time: " << octreeTime << " ms\n";
	std::cout << "Brute-force radius query time: " << bruteTime << " ms\n";
}