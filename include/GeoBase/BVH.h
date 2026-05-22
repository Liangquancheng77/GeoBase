#pragma once
#include "AABB.h"
#include "Triangle3.h"
#include <vector>

struct BVHNode {

	AABB aabb;
	bool isLeaf; // 标记是否为叶子节点

	union {

		struct {
			BVHNode* left;
			BVHNode* right;
		} children; // 内部节点存左右孩子

		struct {
			int start; // 叶子节点在三角形数组中的起始索引
			int count; // 叶子节点包含的三角形数量
		} leaf; // 叶子节点存储三角形信息


	};

	// 构造函数：内部节点
	BVHNode(AABB& aabb, bool isLeaf, BVHNode* left, BVHNode* right) :aabb(aabb), isLeaf(false) {
		children.left = left;
		children.right = right;
	}

	// 构造函数：叶子节点
	BVHNode(AABB& aabb, bool isLeaf, int start, int count) :aabb(aabb), isLeaf(true) {
		leaf.start = start;
		leaf.count = count;
	}

	bool intersect(const Ray& ray, HitInfo& hit, std::vector<Triangle3>& triangles) const;

private:
	bool intersectRecursive(const Ray& ray, HitInfo& hit, std::vector<Triangle3>& triangles) const;

};

// 构建BVH树的递归主函数
BVHNode* buildBVH(std::vector<Triangle3>& triangles, int start, int end, int depth, int maxDepth = 32);

// 构建BVH树的递归主函数(SAH普通版本)
BVHNode* buildBVH_SAH(std::vector<Triangle3>& triangles, int start, int end, int depth, int maxDepth = 32);

// 构建BVH树的递归主函数(SAH桶式结构版本)
BVHNode* buildBVH_Bucket_SAH(std::vector<Triangle3>& triangles, int start, int end, int depth, int maxDepth = 32);

// BVH树碰撞检测：递归遍历两棵BVH树，找出所有相交的三角形对
void collideBVH(const BVHNode* a, const BVHNode* b, std::vector<Triangle3>& trianglesA, std::vector<Triangle3>& trianglesB, std::vector<std::pair<int, int>>& collisionPairs);

// 释放BVH树内存（避免内存泄漏）
void deleteBVH(BVHNode* node);

// OBJ加载函数
bool loadOBJ(const std::string& path, std::vector<Triangle3>& triangles);



