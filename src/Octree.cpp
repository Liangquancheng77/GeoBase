#include "../include/GeoBase/Octree.h"
#include <queue>


AABB computeGlobalBBox(std::vector<Point3>& points){
	AABB globalBox;
	for (const Point3& p : points) {
		globalBox.expand(p);
	}
	return globalBox;
}

// 判断点落在哪个子节点
int Octree::getChildIndex(const OctreeNode* node, const Point3& point) {
	int idx = 0;
	Point3 center = node->box.getCenter();
	idx |= point.x > center.x ? 1 : 0;
	idx |= point.y > center.y ? 2 : 0;
	idx |= point.z > center.z ? 4 : 0;
	return idx;
}

// 分裂节点
void Octree::split(OctreeNode* node) {
	Point3 center = node->box.getCenter();
	// 八个子节点的包围盒
	for (int i = 0; i < 8; ++i)
	{
		AABB childBox;
		childBox.min.x = i & 1 ? center.x : node->box.min.x;
		childBox.max.x = i & 1 ? node->box.max.x : center.x;
		childBox.min.y = i & 2 ? center.y : node->box.min.y;
		childBox.max.y = i & 2 ? node->box.max.y : center.y;
		childBox.min.z = i & 4 ? center.z : node->box.min.z;
		childBox.max.z = i & 4 ? node->box.max.z : center.z;
		node->children[i] = new OctreeNode(childBox, node->depth + 1, true);
	}
	// 将当前节点的点重新分配到子节点
	for (int idx : node->pointIndices) {
		insert(node->children[getChildIndex(node, points[idx])], points[idx], idx);
	}
	node->pointIndices.clear(); // 清空当前节点的点索引
	node->isLeaf = false; // 当前节点不再是叶子节点

}


// 插入函数：将点插入八叉树
void Octree::insert(OctreeNode* node, const Point3& point, int index) {
	if (node->isLeaf)
	{
		node->pointIndices.push_back(index);
		if (node->pointIndices.size() > maxPointsPerLeaf && node->depth < maxDepth)
		{
			split(node);
		}
	}
	else {
		insert(node->children[getChildIndex(node, point)], point, index);
	}
}

// 构建函数：构建八叉树
Octree::Octree(std::vector<Point3> points, int maxDepth, int maxPointsPerLeaf) : points(points) {
	AABB globalBox = computeGlobalBBox(points);
	root = new OctreeNode(globalBox, 0, true);
	this->maxDepth = maxDepth;
	this->maxPointsPerLeaf = maxPointsPerLeaf;
	for (int i = 0; i < points.size(); ++i)
		insert(root, points[i], i);
}


void Octree::radiusSearchRecursive(std::vector<int>& results, OctreeNode* node, const Point3& center, double radius) {
	if (!AABB::boxIntersectsSphere(node->box, center, radius)) return; // 如果节点包围盒与查询球体不相交，直接剪枝
	if (node->isLeaf) {
		for (int idx : node->pointIndices)
		{
			if ((points[idx] - center).lengthSquared() <= radius * radius) results.push_back(idx);
		}
	}
	else {
		for (int i = 0; i < 8; ++i)
		{
			radiusSearchRecursive(results, node->children[i], center, radius);
		}
	}
}

// 半径查询
std::vector<int> Octree::radiusQuery(const Point3& center, double radius) {
	std::vector<int> results;
	radiusSearchRecursive(results, root, center, radius);
	return results;
}

// K近邻查询
std::vector<int> Octree::nearestK(const Point3& query, int k) const {
	std::priority_queue<std::pair<double, int>> knnHeap;
	std::priority_queue<SearchEntry, std::vector<SearchEntry>, std::greater<SearchEntry>> nodeQueue;
	nodeQueue.push({ root, AABB::distSqToPoint(root->box, query) });
	// 记录当前第k近的距离，初始为无穷大，方便后续比较和剪枝
	double worstDist = std::numeric_limits<double>::max();
	while (!nodeQueue.empty()) {
		SearchEntry entry = nodeQueue.top();
		nodeQueue.pop();
		if (entry.node->isLeaf)
		{
			// 如果当前节点包围盒距离查询点的最小距离已经超过当前第k近的距离，剪枝
			double distSq = AABB::distSqToPoint(entry.node->box, query);
			if (distSq >= worstDist) continue; 
			for (int idx : entry.node->pointIndices)
			{
				double pointDistSq = (points[idx] - query).lengthSquared();
				if (pointDistSq < worstDist) {
					knnHeap.push({ pointDistSq, idx });
					if (knnHeap.size() > k)
					{
						knnHeap.pop(); // 移除最远的点
						worstDist = knnHeap.top().first; // 更新第k近的距离
					}
					else if (knnHeap.size() == k) {
						worstDist = knnHeap.top().first; // 第一次达到k个点时，更新第k近的距离
					}
				}
			}

		}
		else
		{
			for (int i = 0; i < 8; ++i)
			{
				OctreeNode* child = entry.node->children[i];
				if (!child) continue;
				double childDistSq = AABB::distSqToPoint(child->box, query);
				if (childDistSq < worstDist) {
					nodeQueue.push({ child, childDistSq });
				}
			}
		}
	}
	std::vector<int> results;
	while (!knnHeap.empty()) {
		results.push_back(knnHeap.top().second);
		knnHeap.pop();
	}
	std::reverse(results.begin(), results.end());
	return results;
}

Octree::~Octree() {
	std::queue<OctreeNode*> nodeQueue;
	nodeQueue.push(root);
	while (!nodeQueue.empty()) {
		OctreeNode* node = nodeQueue.front();
		nodeQueue.pop();
		for (int i = 0; i < 8; ++i)
		{
			if (node->children[i]) nodeQueue.push(node->children[i]);
		}
		delete node;
	}
}