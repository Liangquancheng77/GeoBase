#include <gtest/gtest.h>
#include "../include/GeoBase/Octree.h"
#include "../include/GeoBase/Common.h"
using namespace std;

// 插入100个随机点，验证半径查询返回的点都在指定半径内，数量正确
TEST(OctreeTest, OctreeTest1) {
	std::vector<Point3> points;
	for (int i = 0; i < 100; ++i) {
		points.push_back(Point3(randomDouble(0, 100), randomDouble(0, 100), randomDouble(0, 100)));
	}
	Octree octree(points);
	Point3 query(50, 50, 50);
	double radius = 20.0;
	std::vector<int> results = octree.radiusQuery(query, radius);
	for (int idx : results) {
		EXPECT_LE((points[idx] - query).lengthSquared(), radius * radius);
	}
}

// 插入3个明显分离的点，查询最近邻（K=1），验证返回最近的那个
TEST(OctreeTest, OctreeTest2) {
	std::vector<Point3> points;
	points.push_back(Point3(0, 0, 0));
	points.push_back(Point3(10, 0, 0));
	points.push_back(Point3(0, 10, 0));
	Octree octree(points);
	std::vector<int> result = octree.nearestK(Point3(10, 1, 0), 1);
	EXPECT_EQ(result.size(), 1);
	EXPECT_EQ(result[0], 1);
}

// 插入一个点，K=1查询返回自身，距离为0
TEST(OctreeTest, OctreeTest3) {
	std::vector<Point3> points;
	points.push_back(Point3(0, 0, 0));
	Octree octree(points);
	std::vector<int> result = octree.nearestK(Point3(0, 0, 0), 1);
	EXPECT_EQ(result.size(), 1);
	EXPECT_EQ(result[0], 0);
}

// 插入1000个随机点，对比暴力搜索和八叉树半径查询的耗时，记录加速比
TEST(OctreeTest, OctreeTest4) {

	std::vector<Point3> points;
	for (int i = 0; i < 1000; ++i) {
		points.push_back(Point3(randomDouble(0, 100), randomDouble(0, 100), randomDouble(0, 100)));
	}
	Octree octree(points, 8, 16);
	Point3 query(50, 50, 50);
	double radius = 20.0;
	Timer  bruteTimer;

	std::vector<int> bruteResults;
	for (int i = 0; i < points.size(); ++i) {
		if ((points[i] - query).lengthSquared() <= radius * radius) {
			bruteResults.push_back(i);
		}
	}
	double bruteTimeMs = bruteTimer.elapsedMs();
	cout << "暴力遍历耗时: " << bruteTimeMs << endl;
	Timer octreeTimer;
	std::vector<int> octreeResults = octree.radiusQuery(query, radius);
	double octreeTimeMs = octreeTimer.elapsedMs();
	cout << "八叉树查询耗时: " << octreeTimeMs << endl;
	double speedup = bruteTimeMs / octreeTimeMs;
	cout << "加速比: " << speedup << endl;
	EXPECT_EQ(bruteResults.size(), octreeResults.size());

}

// 在密集点云中查询K=10近邻，验证结果与暴力搜索完全一致
TEST(OctreeTest, OctreeTest5) {

	std::vector<Point3> points;
	for (int i = 0; i < 100000; ++i) {
		points.push_back(Point3(randomDouble(0, 100), randomDouble(0, 100), randomDouble(0, 100)));
	}
	Octree octree(points, 8, 16);
	Point3 query(50, 50, 50);
	double radius = 20.0;
	std::vector<int> bruteResults;
	for (int i = 0; i < points.size(); ++i) {
		if ((points[i] - query).lengthSquared() <= radius * radius) {
			bruteResults.push_back(i);
		}
	}
	std::vector<int> octreeResults = octree.radiusQuery(query, radius);
	EXPECT_EQ(bruteResults.size(), octreeResults.size());
	std::sort(bruteResults.begin(), bruteResults.end());
	std::sort(octreeResults.begin(), octreeResults.end());
	EXPECT_EQ(bruteResults, octreeResults);

}

// 空树查询不崩溃，返回空结果
TEST(OctreeTest, OctreeTest6) {
	std::vector<Point3> points;
	Octree octree(points, 8, 16);
	Point3 query(50, 50, 50);
	double radius = 20.0;
	std::vector<int> results = octree.radiusQuery(query, radius);
	EXPECT_TRUE(results.empty());
}

// 退化1：空点云执行K近邻查询，不崩溃且返回空
TEST(OctreeTest, Degenerate_EmptyCloud_NearestK) {
	std::vector<Point3> points;
	Octree octree(points);
	// 任意K查询都不崩溃
	auto result = octree.nearestK(Point3(0, 0, 0), 5);
	EXPECT_TRUE(result.empty());
}

// 退化2：单个点 + 半径0查询，必须返回自身
TEST(OctreeTest, Degenerate_SinglePoint_ZeroRadius) {
	std::vector<Point3> points = { Point3(10, 20, 30) };
	Octree octree(points);
	auto res = octree.radiusQuery(Point3(10, 20, 30), 0.0);
	EXPECT_EQ(res.size(), 1);
	EXPECT_EQ(res[0], 0);
}

// 退化3：所有点都落在查询半径内，验证返回全部点
TEST(OctreeTest, Degenerate_AllPoints_InRadius) {
	std::vector<Point3> points = {
		Point3(1,1,1), Point3(2,2,2), Point3(3,3,3), Point3(4,4,4)
	};
	Octree octree(points);
	// 超大半径，覆盖所有点
	auto res = octree.radiusQuery(Point3(0, 0, 0), 100.0);
	EXPECT_EQ(res.size(), points.size());
}

// 退化4：无任何点在半径内，返回空集合
TEST(OctreeTest, Degenerate_NoPoints_InRadius) {
	std::vector<Point3> points = { Point3(0,0,0), Point3(1,1,1) };
	Octree octree(points);
	// 查询极远位置，无点匹配
	auto res = octree.radiusQuery(Point3(999, 999, 999), 1.0);
	EXPECT_TRUE(res.empty());
}

// 退化5：重复点云（多个完全相同的点），半径查询返回所有重复点索引
TEST(OctreeTest, Degenerate_DuplicatePoints) {
	std::vector<Point3> points;
	// 插入5个完全一样的点
	for (int i = 0; i < 5; ++i) {
		points.emplace_back(5, 5, 5);
	}
	Octree octree(points);
	auto res = octree.radiusQuery(Point3(5, 5, 5), 0.1);
	EXPECT_EQ(res.size(), 5);
}

// 退化6：最大深度=1（不分割，整棵树只有根节点），查询功能正常
TEST(OctreeTest, Degenerate_MaxDepth1_NoSplit) {
	std::vector<Point3> points = {
		Point3(0,0,0), Point3(10,0,0), Point3(0,10,0)
	};
	// 强制最大深度1，永远不分割子节点
	Octree octree(points, 1, 100);
	auto res = octree.nearestK(Point3(1, 0, 0), 1);
	EXPECT_EQ(res[0], 0);
}