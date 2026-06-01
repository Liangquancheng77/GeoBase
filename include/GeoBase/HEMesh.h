#pragma once
#include "Vector3.h"
#include <vector>
#include <unordered_map>
#include <functional>
#include <ostream>
#include "QEM.h"
#include "EdgeCollapse.h"
#include <queue>

struct HEHalfEdge;

// 顶点
struct HEVert {

	Point3 position;
	HEHalfEdge* edge;
	Quadric quadric;
	int index;
	HEVert() {}
	HEVert(const Point3& pos) : position(pos), edge(nullptr), index(-1){}

	HEVert(const Point3& p, int idx)
		: position(p), edge(nullptr), index(idx) {
	}

};

// 全局重载 <<
inline std::ostream& operator<<(std::ostream& os, const HEVert& v) {
	os << "vertex[" << v.index << "]: (" << v.position.x << ", " << v.position.y << ", " << v.position.z << ")";
	return os;
}

// 面
struct HEFace {
	HEHalfEdge* edge;
	int index;

	HEFace() : edge(nullptr), index(-1) {}

};

// 半边
struct HEHalfEdge {
	HEVert* vertex;
	HEFace* face;
	HEHalfEdge* pair;
	HEHalfEdge* next;
	EdgeCollapse* ec;
	int index;
	HEHalfEdge() {}
	HEHalfEdge(HEVert* vertex) : vertex(vertex), face(nullptr), pair(nullptr), next(nullptr), ec(nullptr), index(-1){}

};

// 网格主体
class HEMesh {
public:
	HEMesh() = default;
	~HEMesh();
	HEMesh(const HEMesh&) = delete;
	HEMesh& operator=(const HEMesh&) = delete;

	// 基本元素添加
	// 创建新顶点
	HEVert* findOrCreateVertex(const Point3& pos);
	// 添加三角形
	void addTriangle(const Point3& v0, const Point3& v1, const Point3& v2);
	

	// 从OBJ构建
	bool loadOBJ(const std::string& filename);

	// 访问器
	size_t numVerts() const { return m_verts.size(); }
	size_t numHalfEdges() const { return m_edges.size(); }
	size_t numFaces() const { return m_faces.size(); }

	const std::vector<HEVert*>& getVertices() const { return m_verts; }
	const std::vector<HEHalfEdge*>& getHalfEdges() const { return m_edges; }
	const std::vector<HEFace*>& getFaces() const { return m_faces; }

	// 面半边遍历
	void forEachHalfEdgeInFace(HEFace* face,
		std::function<void(HEHalfEdge*)> callback);

	// 顶点邻边遍历
	void forEachOutgoingHalfEdge(HEVert* vert,
		std::function<bool(HEHalfEdge*)> callback);

	// 顶点邻点遍历
	void forEachNeighborVertex(HEVert* vert,
		std::function<bool(HEVert*)> callback);

	// 顶点邻面遍历
	void forEachFaceAroundVertex(HEVert* vert,
		std::function<bool(HEFace*)> callback);

	// 边界边判断
	bool isBoundary(const HEHalfEdge* he) const;

	// 获取网格的所有边界半边
	std::vector<HEHalfEdge*> getBoundaryEdges() const;

	// 提取边界环
	std::vector<std::vector<HEVert*>> getBoundaryLoops() const;

	// 判断顶点是否在边界上
	bool isBoundaryVertex(const HEVert* vert) const;

	// 判断网格是否封闭
	bool isClosed() const;

	// 网格完整性验证
	bool validate() const;

	// 更新m_edgeMap，删除he原来的key，新增from、to的key
	void updateEdgeMap(HEHalfEdge* he, HEVert* from, HEVert* to);

	// 将半边的起点修改为vert
	void updateEdgeVertex(HEHalfEdge* he, HEVert* vert);

	// 边翻转
	bool flipEdge(HEHalfEdge* he);

	// 边折叠
	bool collapseEdge(HEHalfEdge* he, Point3* newPosition = nullptr);

	// 创建顶点X并且添加相关的半边
	HEVert* creatVertXAndAddEdges(HEVert* A, HEVert* B, HEVert* C, double t, HEHalfEdge*& XC, HEHalfEdge*& XB);

	// 边分割
	HEVert* splitEdge(HEHalfEdge* he, double t = 0.5);

	// 获取顶点的所有邻点
	std::vector<HEVert*> get_neighbors(HEVert* vert);

	// 获取顶点的所有出边
	std::vector<HEHalfEdge*> get_outgoing_halfedges(HEVert* vert);

	// 计算三角形的面积
	double triangleArea(const HEFace* face) const;

	// 最长边与最短边的比值
	double triangleAspectRatio(const HEFace* face) const;

	// 最小内角
	double minAngle(const HEFace* face) const;

	// 网格质量统计报告
	void meshQualityReport() const;

	// 计算边折叠成本
	EdgeCollapse* computeCollapse(HEHalfEdge* he);

	// 主简化循环
	void simplify(int targetFaces);

	// 边界惩罚
	void penalizeBoundaries();

	// 特征边保护
	bool isFeatureEdge(HEHalfEdge* he, double angleThreshold = 60.0);

private:
	std::vector<HEVert*> m_verts;
	std::vector<HEHalfEdge*> m_edges;
	std::vector<HEFace*> m_faces;

	// 顶点去重
	struct Point3Hash {
		size_t operator() (const Point3& p) const {
			constexpr double invEps = 1.0 / EPS_ABS; 
			auto h = std::hash<long long>()(static_cast<long long>(p.x * invEps));
			h ^= std::hash<long long>()(static_cast<long long>(p.y * invEps)) << 1;
			h ^= std::hash<long long>()(static_cast<long long>(p.z * invEps)) << 2;
			return h;
		}
	};
	struct Point3Equal {
		bool operator() (const Point3& a, const Point3& b) const {
			return std::abs(a.x - b.x) < EPS_ABS &&
				std::abs(a.y - b.y) < EPS_ABS &&
				std::abs(a.z - b.z) < EPS_ABS;
		}
	};
	std::unordered_map<Point3, HEVert*, Point3Hash, Point3Equal> m_vertMap;
	// 边去重
	using EdgeKey = std::pair<int, int>;
	struct EdgeKeyHash {
		size_t operator() (const EdgeKey& p) const {
			return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1 );
		}
	};
	std::unordered_map<EdgeKey, HEHalfEdge*, EdgeKeyHash> m_edgeMap;

	// 创建一对方向相反的半边
	HEHalfEdge* findOrCreateHalfEdge(HEVert* from,HEVert* to);

	// 辅助函数:创建一个面
	HEFace* createFace(HEHalfEdge* he);

	// 顶点Q矩阵初始化
	void computeInitialQMatrices();

	// 获取所有边界边
	std::vector<HEHalfEdge*> getBoundaryEdges();

};