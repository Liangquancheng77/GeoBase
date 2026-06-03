#include "../include/GeoBase/KdTree.h"
#include <queue>
using namespace std;

// 构建Kd树的递归函数
KdTree::KdTreeNode* KdTree::buildKdTree(int start, int end, int depth) {
	if (start >= end) return nullptr;

	int axis = depth % 3; // 分割轴：0=x, 1=y, 2=z
	int mid = start + (end - start) / 2;
	std::nth_element(points.begin() + start, points.begin() + mid, points.begin() + end, [axis](const Point3& a, const Point3& b) {
		return axis == 0 ? a.x < b.x : (axis == 1 ? a.y < b.y : a.z < b.z);
		});
	KdTreeNode* node = new KdTreeNode(points[mid], axis);
	node->left = buildKdTree(start, mid, depth + 1);
	node->right = buildKdTree(mid + 1, end, depth + 1);
	if (node->left) node->box.merge(node->left->box);
	if (node->right) node->box.merge(node->right->box);
	return node;
}

// 构造函数：接受点集，构建Kd树
KdTree::KdTree(std::vector<Point3> points) : points(std::move(points)) {
	root = buildKdTree(0, this->points.size(), 0);
}

// 递归半径查询函数
void KdTree::radiusSearchRecursive(std::vector<Point3>& results, KdTree::KdTreeNode* node, const Point3& center, double radius) {
	if (!node) return;
	if (AABB::distSqToPoint(node->box, center) > radius * radius) return; // 包围盒距离大于半径，剪枝
	if ((node->point - center).lengthSquared() <= radius * radius) results.push_back(node->point);
	radiusSearchRecursive(results, node->left, center, radius);
	radiusSearchRecursive(results, node->right, center, radius);
}

// 半径查询
std::vector<Point3> KdTree::radiusQuery(const Point3& center, double radius) {
	std::vector<Point3> results;
	radiusSearchRecursive(results, root, center, radius);
	return results;
}

// K近邻查询
std::vector<Point3> KdTree::nearestK(const Point3& query, int k) const {
	// 使用一个最大堆来维护当前找到的K个最近邻
	using KnnHeapPair = std::pair<double, Point3>;
	auto knnCmp = [](const KnnHeapPair& a, const KnnHeapPair& b) {
		// 只比较距离，完全忽略Point3
		return a.first < b.first;
		};
	std::priority_queue<KnnHeapPair, std::vector<KnnHeapPair>, decltype(knnCmp)> knnHeap(knnCmp);
	// 使用一个最小堆来遍历节点，优先访问距离查询点最近的节点
	std::priority_queue<DistPointPair, std::vector<DistPointPair>, std::greater<DistPointPair>> nodeQueue;
	nodeQueue.push({(root->point - query).lengthSquared(), root});
	double worstDist = std::numeric_limits<double>::max(); // 当前第k近的距离，初始为无穷大
	while (!nodeQueue.empty()) {
		DistPointPair pair = nodeQueue.top();nodeQueue.pop();
		if (pair.distSq < worstDist)
		{
			knnHeap.push({pair.distSq, pair.node->point });
			if (knnHeap.size() > k)
			{
				knnHeap.pop();
				worstDist = knnHeap.top().first;
			}
			else if (knnHeap.size() == k) worstDist = knnHeap.top().first;	
		}
		int axis = pair.node->splitAxis;
		double diff = axis == 0 ? query.x - pair.node->point.x : (axis == 1 ? query.y - pair.node->point.y : query.z - pair.node->point.z);
		KdTreeNode* nearChild = diff < 0 ? pair.node->left : pair.node->right;
		KdTreeNode* farChild = diff < 0 ? pair.node->right : pair.node->left;
		if (nearChild)  nodeQueue.push({(nearChild->point - query).lengthSquared(), nearChild});
		if (farChild && diff * diff < worstDist)  nodeQueue.push({(farChild->point - query).lengthSquared(), farChild });
	}
	std::vector<Point3> results;
	while (!knnHeap.empty()) {
		results.push_back(knnHeap.top().second);
		knnHeap.pop();
	}
	std::reverse(results.begin(), results.end()); // 从最近到最远
	return results;
}

// 递归最近邻搜索函数
void KdTree::searchNN(KdTree::KdTreeNode* node, const Point3& query, Point3& bestPoint, double& bestDist) const {
	if (!node) return;
	double distSq = (node->point - query).lengthSquared();
	if (distSq < bestDist) {
		bestDist = distSq;
		bestPoint = node->point;
	}
	int axis = node->splitAxis;
	double diff = axis == 0 ? query.x - node->point.x : (axis == 1 ? query.y - node->point.y : query.z - node->point.z);
	KdTreeNode* nearNode = diff < 0 ? node->left : node->right;
	KdTreeNode* farNode = diff < 0 ? node->right : node->left;
	// 优先搜索距离查询点更近的子树
	searchNN(nearNode, query, bestPoint, bestDist);
	// 如果当前分割轴上的距离平方小于bestDist，说明远侧子树可能包含更近的点，需要搜索
	if (diff * diff < bestDist) {
		searchNN(farNode, query, bestPoint, bestDist);
	}
}

// 最近邻搜索
Point3 KdTree::nearestNeighbor(const Point3& query) const {
	if (!root) throw std::runtime_error("KdTree is empty");
	double bestDist = std::numeric_limits<double>::max();
	Point3 bestPoint;
	searchNN(root, query, bestPoint, bestDist);
	return bestPoint;
}
