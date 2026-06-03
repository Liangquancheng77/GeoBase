#pragma once
#include "AABB.h"

class Octree {

public :

	// 构建函数：构建八叉树
	Octree(std::vector<Point3> points, int maxDepth = 8, int maxPointsPerLeaf = 16);

	// 半径查询
	std::vector<int> radiusQuery(const Point3& center, double radius);

	// K近邻查询
	std::vector<int> nearestK(const Point3& query, int k) const;
	//const AABB& getBounds() const;

	~Octree();


private:

	struct OctreeNode {
		AABB box;
		std::vector<int> pointIndices; // 叶子节点存储点索引
		OctreeNode* children[8] = { nullptr }; // 八个子节点指针
		int depth; // 当前节点深度
		bool isLeaf; // 是否为叶子节点

		OctreeNode(AABB b, int d, bool leaf) : box(b), depth(d), isLeaf(leaf) {}

	};
	struct SearchEntry {
		OctreeNode* node;
		double minDistSq; // 节点与查询点的最小平方距离

		SearchEntry(OctreeNode* node, double minDistSq) : node(node), minDistSq(minDistSq) {}

		bool operator>(const SearchEntry& other) const {
			return minDistSq > other.minDistSq;
		}

	};


	OctreeNode* root; // 八叉树根节点
	int maxDepth; // 八叉树最大深度
	int maxPointsPerLeaf; // 叶子容量阈值
	// 存储原始点集的引用（索引对应这里的点）
	std::vector<Point3> points;

	// 判断点落在哪个子节点
	int getChildIndex(const OctreeNode* node, const Point3& point);

	// 分裂节点
	void split(OctreeNode* node);

	// 插入函数：将点插入八叉树
	void insert(OctreeNode* node, const Point3& point, int index);

	void radiusSearchRecursive(std::vector<int>& results, OctreeNode* node, const Point3& center, double radius);
};


