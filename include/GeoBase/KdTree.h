#pragma once
#include "Vector3.h"
#include "AABB.h"

class KdTree {


public:

	std::vector<Point3> points; // 存储原始点集

	struct KdTreeNode {

		Point3& point; // 存储点的引用
		AABB box; // 当前节点的包围盒
		KdTreeNode* left = nullptr; // 左子树指针
		KdTreeNode* right = nullptr; // 右子树指针
		int splitAxis; // 分割轴（0=x, 1=y, 2=z）

		KdTreeNode(Point3& point, int splitAxis) : point(point), splitAxis(splitAxis), box(AABB(point, point)) {}

	};

	KdTree(std::vector<Point3> points);

	// 半径查询
	std::vector<Point3> radiusQuery(const Point3& center, double radius);

	// K近邻查询
	std::vector<Point3> nearestK(const Point3& query, int k) const;

	// 最近邻搜索
	Point3 nearestNeighbor(const Point3& query) const;

private :

	KdTreeNode* root = nullptr;
	

	// 构建Kd树的递归函数
	KdTreeNode* buildKdTree(int start, int end, int depth);

	// 递归半径查询函数
	void radiusSearchRecursive(std::vector<Point3>& results, KdTree::KdTreeNode* node, const Point3& center, double radius);

	// 递归最近邻搜索函数
	void searchNN(KdTree::KdTreeNode* node, const Point3& query, Point3& bestPoint, double& bestDist) const;

	struct DistPointPair {
		double distSq; // 点与查询点的距离平方
		KdTreeNode* node; // 对应的Kd树节点
		DistPointPair(double distSq, KdTreeNode* node) : distSq(distSq), node(node) {}
		bool operator>(const DistPointPair& other) const {
			return distSq > other.distSq; // 距离较小的点优先
		}
	};

};