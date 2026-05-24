#pragma once
#include "Vector3.h"
#include <vector>
#include <unordered_map>
#include <functional>
#include <ostream>

struct HEHalfEdge;

// 顶点
struct HEVert {

	Point3 position;
	HEHalfEdge* edge;
	int index;

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
	int index;
	//HEHalfEdge() {}
	HEHalfEdge(HEVert* vertex) : vertex(vertex), face(nullptr), pair(nullptr), next(nullptr), index(-1){}

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
		std::function<void(HEHalfEdge*)> callback);

	// 顶点邻点遍历
	void forEachNeighborVertex(HEVert* vert,
		std::function<void(HEVert*)> callback);

	// 顶点邻面遍历
	void forEachFaceAroundVertex(HEVert* vert,
		std::function<void(HEFace*)> callback);

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

private:
	std::vector<HEVert*> m_verts;
	std::vector<HEHalfEdge*> m_edges;
	std::vector<HEFace*> m_faces;

	// 顶点去重
	struct Point3Hash {
		size_t operator() (const Point3& p) const {
			const double scale = 1e4;
			auto h = std::hash<long long>()(static_cast<long long>(p.x * scale));
			h ^= std::hash<long long>()(static_cast<long long>(p.y * scale)) << 1;
			h ^= std::hash<long long>()(static_cast<long long>(p.z * scale)) << 2;
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

	// 辅助函数:创建一对方向相反的半边
	HEHalfEdge* findOrCreateHalfEdge(HEVert* from,HEVert* to);

};