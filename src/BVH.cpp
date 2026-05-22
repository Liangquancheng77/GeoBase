#include "../include/GeoBase/BVH.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>

AABB computeAABB(const std::vector<Triangle3>& triangles, int start, int end){
	if (start >= end) return AABB();
	AABB box;
	for (int i = start; i < end; i++)
	{
		box.expand(triangles[i].v0);
		box.expand(triangles[i].v1);
		box.expand(triangles[i].v2);
	}
	return box;

}

Vector3 triangleCenter(const Triangle3& triangle) {
	return (triangle.v0 + triangle.v1 + triangle.v2) / 3.0;
}

// 构建BVH树的递归主函数
BVHNode* buildBVH(std::vector<Triangle3>& triangles, int start, int end, int depth, int maxDepth) {
	// 1.计算当前集合的AABB
	AABB box = computeAABB(triangles, start, end);

	const int triangleCount = end - start;
	// 2.判断是否为叶子节点：三角形数量≤4 或 深度超过限制
	if (triangleCount <= 4 || depth >= maxDepth)
	{
		return new BVHNode(box, true, start, triangleCount);
	}
	// 3.选择最长轴作为分割轴
	const double dx = box.max.x - box.min.x;
	const double dy = box.max.y - box.min.y;
	const double dz = box.max.z - box.min.z;
	int axis = 0;
	if (dy > dx && dy > dz) axis = 1;
	if (dz > dx && dz > dy) axis = 2;
	
	// 4.对三角形集合的中心按照分割轴进行排序
	auto compare = [axis](const Triangle3& a, const Triangle3& b) {
		Vector3 centerA = triangleCenter(a);
		Vector3 centerB = triangleCenter(b);
		if (axis == 0) return centerA.x < centerB.x;
		if (axis == 1) return centerA.y < centerB.y;
		return centerA.z < centerB.z;
		};
	std::sort(triangles.begin() + start, triangles.begin() + end, compare);

	const int mid = start + triangleCount / 2;

	// 5.递归构建左右子树
	//AABB parentBox;
	BVHNode* left = buildBVH(triangles, start, mid, depth +1 , maxDepth);
	BVHNode* right = buildBVH(triangles, mid, end, depth +1 , maxDepth);
	//parentBox.min = Point3(
	//	std::min(left->aabb.min.x, right->aabb.min.x),
	//	std::min(left->aabb.min.y, right->aabb.min.y),
	//	std::min(left->aabb.min.z, right->aabb.min.z));
	//parentBox.max = Point3(
	//	std::max(left->aabb.max.x, right->aabb.max.x),
	//	std::max(left->aabb.max.y, right->aabb.max.y),
	//	std::max(left->aabb.max.z, right->aabb.max.z));
	return new BVHNode(box, false, left, right);

}

// 计算AABB的表面积
inline double computeSurfaceArea(const AABB& box) {
	return 2.0 * ((box.max.x - box.min.x) * (box.max.y - box.min.y) +
		(box.max.y - box.min.y) * (box.max.z - box.min.z) +
		(box.max.z - box.min.z) * (box.max.x - box.min.x));
}

// 构建BVH树的递归主函数(SAH版本)
BVHNode* buildBVH_SAH(std::vector<Triangle3>& triangles, int start, int end, int depth, int maxDepth) {
	// 1.计算当前集合的AABB
	AABB box = computeAABB(triangles, start, end);

	const int triangleCount = end - start;
	// 2.判断是否为叶子节点：三角形数量≤4 或 深度超过限制
	if (triangleCount <= 4 || depth >= maxDepth)
	{
		return new BVHNode(box, true, start, triangleCount);
	}
	// 3.选择最长轴作为分割轴
	const double dx = box.max.x - box.min.x;
	const double dy = box.max.y - box.min.y;
	const double dz = box.max.z - box.min.z;
	int axis = 0;
	if (dy > dx && dy > dz) axis = 1;
	if (dz > dx && dz > dy) axis = 2;

	// 4.对三角形集合的中心按照分割轴进行排序
	auto compare = [axis](const Triangle3& a, const Triangle3& b) {
		Vector3 centerA = triangleCenter(a);
		Vector3 centerB = triangleCenter(b);
		if (axis == 0) return centerA.x < centerB.x;
		if (axis == 1) return centerA.y < centerB.y;
		return centerA.z < centerB.z;
		};
	std::sort(triangles.begin() + start, triangles.begin() + end, compare);
	// 5.SAH核心：遍历所有分割点，找成本最低的
	double parentArea = computeSurfaceArea(box);
	int bestSplit = start + triangleCount / 2;

	std::vector<AABB> leftBoxes(triangleCount + 1);
	std::vector<AABB> rightBoxes(triangleCount + 1);

	leftBoxes[0] = AABB();
	rightBoxes[triangleCount] = AABB();

	for (int i = 0; i < triangleCount; i++)
	{
		leftBoxes[i + 1] = AABB::merge(leftBoxes[i], triangles[start+i].getBoundingBox());
	}
	for (int i = triangleCount - 1; i >= 0; i--)
	{
		rightBoxes[i] = AABB::merge(rightBoxes[i+1], triangles[start+i].getBoundingBox());
	}

	double minCost = std::numeric_limits<double>::max();

	for (int i = 1; i < triangleCount; i++)
	{
		double leftArea = computeSurfaceArea(leftBoxes[i]);
		double rightArea = computeSurfaceArea(rightBoxes[i]);
		// 计算SAH成本
		double cost = (leftArea/parentArea) * i + (rightArea/parentArea) * (triangleCount - i);
		if (cost < minCost)
		{
			bestSplit = start + i;
			minCost = cost;
		}
	}


	// 6.递归构建左右子树
	//AABB parentBox;
	BVHNode* left = buildBVH_SAH(triangles, start, bestSplit, depth + 1, maxDepth);
	BVHNode* right = buildBVH_SAH(triangles, bestSplit, end, depth + 1, maxDepth);
	//parentBox.min = Point3(
	//	std::min(left->aabb.min.x, right->aabb.min.x),
	//	std::min(left->aabb.min.y, right->aabb.min.y),
	//	std::min(left->aabb.min.z, right->aabb.min.z));
	//parentBox.max = Point3(
	//	std::max(left->aabb.max.x, right->aabb.max.x),
	//	std::max(left->aabb.max.y, right->aabb.max.y),
	//	std::max(left->aabb.max.z, right->aabb.max.z));
	return new BVHNode(box, false, left, right);

}

struct Bucket {
	int count = 0;
	AABB box;
};

// 构建BVH树的递归主函数(SAH桶式结构版本)
BVHNode* buildBVH_Bucket_SAH(std::vector<Triangle3>& triangles, int start, int end, int depth, int maxDepth) {
	// 1.计算当前集合的AABB
	const int triangleCount = end - start;
	AABB box = computeAABB(triangles, start, end);
	// 2.递归结束条件：三角形数量≤4 或 深度超过限制
	if (triangleCount <= 4 || depth >= maxDepth)
	{
		return new BVHNode(box, true, start, triangleCount);
	}
	// 3.选择最长轴作为分割轴
	const double dx = box.max.x - box.min.x;
	const double dy = box.max.y - box.min.y;
	const double dz = box.max.z - box.min.z;
	
	int axis = 0;
	if (dy > dx && dy > dz) axis = 1;
	if (dz > dx && dz > dy) axis = 2;
	// 如果最长轴长度为0，说明所有三角形在该轴上重叠，无法继续分割，直接创建叶子节点
	const double axisLength = (axis == 0) ? dx : (axis == 1) ? dy : dz;
	if (axisLength < EPS_ABS) {
		return new BVHNode(box, true, start, triangleCount);
	}
	const double invAxisLength = 1.0 / axisLength;

	// 4.将三角形按重心分到桶中
	const int BUCKET_COUNT = 12;
	Bucket buckets[BUCKET_COUNT];
	for (int i = start; i < end; i++)
	{
		Triangle3& tri = triangles[i];
		Point3 center = triangleCenter(tri);
		// 计算重心在分割轴上的归一化位置
		double normalizedPos;
		if (axis == 0) normalizedPos = (center.x - box.min.x) * invAxisLength;
		else if (axis == 1) normalizedPos = (center.y - box.min.y) * invAxisLength;
		else normalizedPos = (center.z - box.min.z) * invAxisLength;
		int bucketIndex = static_cast<int>(normalizedPos * BUCKET_COUNT);
		// 防止边界溢出
		if (bucketIndex >= BUCKET_COUNT) bucketIndex = BUCKET_COUNT - 1;
		buckets[bucketIndex].box = AABB::merge(buckets[bucketIndex].box, triangles[i].getBoundingBox());
		buckets[bucketIndex].count++;
	}

	// 5.预计算前缀和后缀桶的AABB和数量
	AABB prefixBoxes[BUCKET_COUNT];
	AABB suffixBoxes[BUCKET_COUNT];
	int prefixCounts[BUCKET_COUNT] = {0};
	int suffixCounts[BUCKET_COUNT] = {0};
	prefixBoxes[0] = buckets[0].box;
	prefixCounts[0] = buckets[0].count;
	suffixBoxes[BUCKET_COUNT - 1] = buckets[BUCKET_COUNT - 1].box;
	suffixCounts[BUCKET_COUNT - 1] = buckets[BUCKET_COUNT - 1].count;
	for (int i = 1; i < BUCKET_COUNT - 1; i++)
	{
		prefixBoxes[i] = AABB::merge(prefixBoxes[i - 1], buckets[i].box);
		prefixCounts[i] = prefixCounts[i - 1] + buckets[i].count;
		suffixBoxes[BUCKET_COUNT - i - 1] = AABB::merge(suffixBoxes[BUCKET_COUNT - i], buckets[BUCKET_COUNT - i - 1].box);
		suffixCounts[BUCKET_COUNT - i - 1] = suffixCounts[BUCKET_COUNT - i] + buckets[BUCKET_COUNT - i - 1].count;
	}
	// 6.计算成本并找到最佳桶分割点
	double parentArea = computeSurfaceArea(box);
	double minCost = std::numeric_limits<double>::max();
	int bestSplitBucket = 0;
	for (int i = 0; i < BUCKET_COUNT - 1; i++)
	{
		// 分割点在第i个桶和第i+1个桶之间
		double cost = (computeSurfaceArea(prefixBoxes[i]) * prefixCounts[i] + computeSurfaceArea(suffixBoxes[i+1]) * suffixCounts[i+1]) / parentArea;
		if (cost < minCost) {
			minCost = cost;
			bestSplitBucket = i;
		}
	}
	// 7.优化:如果成本大于等于三角形数量，说明分割没有意义，直接创建叶子节点
	if (minCost >= triangleCount) {
		return new BVHNode(box, true, start, triangleCount);
	}
	// 8.重新排序三角形
	auto splitPredicate = [&](const Triangle3& tri) {
		Point3 center = triangleCenter(tri);
		double normalizedPos;
		if (axis == 0) normalizedPos = (center.x - box.min.x) * invAxisLength;
		else if (axis == 1) normalizedPos = (center.y - box.min.y) * invAxisLength;
		else normalizedPos = (center.z - box.min.z) * invAxisLength;
		int bucketIndex = static_cast<int>(normalizedPos * BUCKET_COUNT);
		// 防止边界溢出
		if (bucketIndex >= BUCKET_COUNT) bucketIndex = BUCKET_COUNT - 1;
		return bucketIndex <= bestSplitBucket;
		};

	auto midPtr = std::partition(triangles.begin() + start, triangles.begin() + end, splitPredicate);
	int mid = midPtr - triangles.begin();
	if (mid == start || mid == end) {
		// 划分无效，一侧为空，强制转为叶子
		return new BVHNode(box, true, start, triangleCount);
	}
	// 9.递归构建左右子树
	BVHNode* left = buildBVH_Bucket_SAH(triangles, start, mid, depth + 1, maxDepth);
	BVHNode* right = buildBVH_Bucket_SAH(triangles, mid, end, depth + 1, maxDepth);
	return new BVHNode(box, false, left, right);
}
// 对比两个叶子节点的三角形对，找出所有相交的三角形对
inline void collideLeafNodes(const BVHNode* a, const BVHNode* b, std::vector<Triangle3>& trianglesA, std::vector<Triangle3>& trianglesB, std::vector<std::pair<int, int>>& collisionPairs) {
	for (int i = 0; i < a->leaf.count; i++)
	{
		const Triangle3& triA = trianglesA[a->leaf.start + i];
		const AABB boxA = triA.getBoundingBox();
		for (int j = 0; j < b->leaf.count; j++)
		{
			const Triangle3& triB = trianglesB[b->leaf.start + j];
			const AABB boxB = triB.getBoundingBox();
			// 首先用AABB快速排除不相交的三角形对
			if (!boxA.intersects(boxB)) continue;
			if (triA.intersect_sat(triB))
			{
				collisionPairs.emplace_back(a->leaf.start + i, b->leaf.start + j);
			}
		}
	}
}

// BVH树碰撞检测：递归遍历两棵BVH树，找出所有相交的三角形对
void collideBVH(const BVHNode* a, const BVHNode* b, std::vector<Triangle3>& trianglesA, std::vector<Triangle3>& trianglesB, std::vector<std::pair<int, int>>& collisionPairs) {
	// 递归结束条件：如果两个节点的AABB不相交，直接返回
	if (!a->aabb.intersects(b->aabb)) return;
	// 情况1：如果两个节点都是叶子节点，执行三角形两两碰撞检测
	if (a->isLeaf && b->isLeaf)
	{
		collideLeafNodes(a, b, trianglesA, trianglesB, collisionPairs);
		return;
	}
	// 情况2：a是叶子，b不是
	if (a->isLeaf)
	{
		collideBVH(a, b->children.left, trianglesA, trianglesB, collisionPairs);
		collideBVH(a, b->children.right, trianglesA, trianglesB, collisionPairs);
		return;
	}
	// 情况3：b是叶子，a不是
	if (b->isLeaf)
	{
		collideBVH(a->children.left, b, trianglesA, trianglesB, collisionPairs);
		collideBVH(a->children.right, b, trianglesA, trianglesB, collisionPairs);
		return;
	}
	// 情况4：都不是
	collideBVH(a->children.left, b->children.left, trianglesA, trianglesB, collisionPairs);
	collideBVH(a->children.left, b->children.right, trianglesA, trianglesB, collisionPairs);
	collideBVH(a->children.right, b->children.left, trianglesA, trianglesB, collisionPairs);
	collideBVH(a->children.right, b->children.right, trianglesA, trianglesB, collisionPairs);
}



// 释放BVH树内存（避免内存泄漏）
void deleteBVH(BVHNode* node) {
	if (!node) return;
	if (!node->isLeaf) {
		deleteBVH(node->children.left);
		deleteBVH(node->children.right);
	}
	delete node;
}

// 外部接口:BVH节点射线相交
bool BVHNode::intersect(const Ray& ray, HitInfo& hit, std::vector<Triangle3>& triangles) const {
	double tMinOut, tMaxOut;
	if (!aabb.intersect(ray, tMinOut, tMaxOut)) return false;
	return intersectRecursive(ray, hit, triangles);
}

// 内部递归函数:BVH节点射线相交
bool BVHNode::intersectRecursive(const Ray& ray, HitInfo& hit, std::vector<Triangle3>& triangles) const {
	if (isLeaf) {
		bool hitSomething = false;
		for (int i = leaf.start; i < leaf.start + leaf.count; i++)
		{
			double t, u, v;
			if (triangles[i].intersect(ray, t, u, v)) {
				if (t < hit.t) {
					hit.t = t;
					hit.point = ray.pointAt(t);
					hit.normal = triangles[i].getNormal();
					if (ray.direction.dot(hit.normal) > 0) hit.normal = hit.normal * (-1);
				}
				hitSomething = true;
			}
		}
		return hitSomething;
	}
	else {
		BVHNode* first = nullptr;
		BVHNode* second = nullptr;
		double tLeftMin, tLeftMax, tRightMin, tRightMax;
		bool hitLeft = children.left->aabb.intersect(ray, tLeftMin, tLeftMax);
		bool hitRight = children.right->aabb.intersect(ray, tRightMin, tRightMax);
		double tSecondEntry = std::numeric_limits<double>::max();
		if (hitLeft && hitRight)
		{
			
			if (tLeftMin > tRightMin) {
				first = children.right;
				second = children.left;
				tSecondEntry = tLeftMin;
			} else {
				first = children.left;
				second = children.right;
				tSecondEntry = tRightMin;
			}
		}
		else if (hitLeft && !hitRight) {
			first = children.left;
		}
		else if (!hitLeft && hitRight) {
			first = children.right;
		}
		else {
			return false; // 两个子树的AABB都没有相交，直接返回false
		}
		bool hitAny = false;
		if (first) {
			hitAny = first->intersectRecursive(ray, hit, triangles);
		}
		if (hitAny && hit.t < tSecondEntry)
		{
			return true; // 已经命中第一个子树，并且交点在第二个子树入口之前，无需检查第二个子树
		}
		if (second)
		{
			if (second->intersectRecursive(ray, hit, triangles)) hitAny = true;
		}
		return hitAny;
	}
}


// OBJ加载函数
bool loadOBJ(const std::string& path, std::vector<Triangle3>& triangles) {
	std::vector<Point3> vertices;
	std::ifstream file(path);
	if (!file.is_open()) {
		std::cerr << "无法打开文件: " << path << std::endl;
		return false;
	}
	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string type;
		iss >> type;
		if (type == "v") {
			double x, y, z;
			iss >> x >> y >> z;
			vertices.emplace_back(x, y, z);
		}
		else if (type == "f") {
			int i1, i2, i3;
			iss >> i1 >> i2 >> i3;
			i1--;i2--;i3--;
			triangles.emplace_back(vertices[i1], vertices[i2], vertices[i3]);
		}
	}
	file.close();
	return true;
}


