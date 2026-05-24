#include "../include/GeoBase/HEMesh.h"
#include <unordered_map>
#include <iostream>
#include <fstream>   
#include <string>
#include <unordered_set>

using namespace std;

//using EdgeKey = std::pair<uint64_t, uint64_t>;
//std::unordered_map<EdgeKey, HEHalfEdge*> s_edgeMap;
//std::unordered_map<uint64_t, HEVert*> s_vertMap;

//EdgeKey makeKey(HEVert* a, HEVert* b) {
//	uint64_t addA = (uint64_t) a;
//	uint64_t addB = (uint64_t) b;
//	if (addA < addB) return { addA, addB };
//	return { addB, addA };
//}

HEMesh::~HEMesh() {
	for (auto m_face : m_faces) delete m_face;
	for (auto m_edge : m_edges) delete m_edge;
	for (auto m_vert : m_verts) delete m_vert;

	m_faces.clear();
	m_edges.clear();
	m_verts.clear();

	m_vertMap.clear();
	m_edgeMap.clear();

}

// 创建新顶点
HEVert* HEMesh::findOrCreateVertex(const Point3& pos) {
	auto it = m_vertMap.find(pos);
	if (it != m_vertMap.end()) return it->second;
	int idx = static_cast<int> (m_verts.size());
	HEVert* vert = new HEVert(pos, idx);
	m_verts.push_back(vert);
	m_vertMap[pos] = vert;
	return vert;
}


// 辅助函数:获取或创建一对半边
HEHalfEdge* HEMesh::findOrCreateHalfEdge(HEVert* from, HEVert* to) {

	int idxF = from->index;
	int idxT = to->index;
	EdgeKey key = idxF < idxT ? EdgeKey(idxF, idxT) : EdgeKey(idxT, idxF);
	auto it = m_edgeMap.find(key);
	if (it == m_edgeMap.end()) {
		int size = static_cast<int>(m_edges.size());
		HEHalfEdge* edge1 = new HEHalfEdge(from);
		edge1->index = size;
		HEHalfEdge* edge2 = new HEHalfEdge(to);
		edge2->index = size + 1;
		edge1->pair = edge2;
		edge2->pair = edge1;
		m_edges.push_back(edge1);
		m_edges.push_back(edge2);
		m_edgeMap[key] = idxF < idxT ? edge1 : edge2;
		return edge1;
	} else {
		HEHalfEdge* edge = it->second;
		HEHalfEdge* he_from_to = edge->vertex->index == idxF ? edge : edge->pair;
		if (he_from_to->face != nullptr)
		{
			// 非流形
			throw std::runtime_error("Non-manifold edge: duplicate face on the same edge direction");
		}
		return he_from_to;
	}

}

// 添加三角形
void HEMesh::addTriangle(const Point3& v0, const Point3& v1, const Point3& v2) {

	HEVert* vert0 = findOrCreateVertex(v0);
	HEVert* vert1 = findOrCreateVertex(v1);
	HEVert* vert2 = findOrCreateVertex(v2);

	auto he01 = findOrCreateHalfEdge(vert0, vert1);
	auto he12 = findOrCreateHalfEdge(vert1, vert2);
	auto he20 = findOrCreateHalfEdge(vert2, vert0);

	if (vert0->edge == nullptr) vert0->edge = he01;
	if (vert1->edge == nullptr) vert1->edge = he12;
	if (vert2->edge == nullptr) vert2->edge = he20;

	he01->next = he12;
	he12->next = he20;
	he20->next = he01;

	HEFace* face = new HEFace();
	face->index = m_faces.size();
	m_faces.push_back(face);
	face->edge = he01;

	he01->face = face;
	he12->face = face;
	he20->face = face;

}

// 从OBJ文件构建半边网格
bool HEMesh::loadOBJ(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Error: Failed to open OBJ file " << filename << std::endl;
		return false;
	}

	std::vector<Point3> tempVertices; // 临时存储所有顶点坐标
	std::string line;

	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string type;
		iss >> type;

		// 1. 解析顶点坐标
		if (type == "v") {
			double x, y, z;
			iss >> x >> y >> z;
			tempVertices.emplace_back(x, y, z);
		}
		// 2. 解析面
		else if (type == "f") {
			std::vector<int> indices;
			std::string token;

			// 解析所有顶点索引（自动忽略纹理和法线索引）
			while (iss >> token) {
				size_t slashPos = token.find('/');
				int idx = std::stoi(token.substr(0, slashPos));

				// 处理OBJ负数索引（相对于末尾的索引）
				if (idx < 0) {
					idx += static_cast<int>(tempVertices.size()) + 1;
				}

				indices.push_back(idx);
			}

			// 处理三角形面
			if (indices.size() == 3) {
				int i0 = indices[0] - 1;
				int i1 = indices[1] - 1;
				int i2 = indices[2] - 1;
				addTriangle(tempVertices[i0], tempVertices[i1], tempVertices[i2]);
			}
			// 处理四边形面（按要求拆分为两个三角形）
			else if (indices.size() == 4) {
				int i0 = indices[0] - 1;
				int i1 = indices[1] - 1;
				int i2 = indices[2] - 1;
				int i3 = indices[3] - 1;
				addTriangle(tempVertices[i0], tempVertices[i1], tempVertices[i2]);
				addTriangle(tempVertices[i0], tempVertices[i2], tempVertices[i3]);
			}
			// 处理其他多边形面（输出警告）
			else {
				std::cerr << "Warning: Unsupported polygon face (vertex count: " << indices.size() << "), skipped" << std::endl;
			}
		}
		// 自动忽略其他行（vn, vt, g, usemtl 等）
	}

	file.close();
	std::cout << "OBJ loaded successfully: Vertices " << numVerts() << ", Faces " << numFaces() << std::endl;
	return true;
}

// 面半边遍历
void HEMesh::forEachHalfEdgeInFace(HEFace* face,
	std::function<void(HEHalfEdge*)> callback) {
	if (!face || !face->edge) return;
	HEHalfEdge* start = face->edge;
	HEHalfEdge* current = start;
	do {
		callback(current);
		current = current->next;
	} while (current != start);
}

// 顶点邻边遍历
void HEMesh::forEachOutgoingHalfEdge(HEVert* vert,
	std::function<void(HEHalfEdge*)> callback) {
	HEHalfEdge* start = vert->edge;
	HEHalfEdge* current = start;
	do {
		callback(current);
		current = current->pair->next;
	} while (current != start);
}

// 顶点邻点遍历
void HEMesh::forEachNeighborVertex(HEVert* vert,
	std::function<void(HEVert*)> callback) {
	forEachOutgoingHalfEdge(vert, [&](HEHalfEdge* he) {
		callback(he->vertex);
		});
}

// 顶点邻面遍历
void HEMesh::forEachFaceAroundVertex(HEVert* vert,
	std::function<void(HEFace*)> callback) {
	forEachOutgoingHalfEdge(vert, [&](HEHalfEdge* he) {
		if (he->face != nullptr) callback(he->face);
		});
}

// 边界边判断
bool HEMesh::isBoundary(const HEHalfEdge* he) const {
	if (he == nullptr) return false;
	return he->face == nullptr;
}

// 获取网格的所有边界半边
std::vector<HEHalfEdge*> HEMesh::getBoundaryEdges() const {
	std::vector<HEHalfEdge*> boundaryEdges;
	auto edges = getHalfEdges();
	for (HEHalfEdge* edge : edges)
	{
		if (edge->face == nullptr)
		{
			boundaryEdges.emplace_back(edge);
		}
	}
	return boundaryEdges;
}

// 提取边界环
std::vector<std::vector<HEVert*>> HEMesh::getBoundaryLoops() const {

	std::unordered_set<HEHalfEdge*> visited;
	std::vector<std::vector<HEVert*>> result;
	for (HEHalfEdge* edge :getHalfEdges()) {
		if (!isBoundary(edge) || visited.count(edge)) continue;
		HEHalfEdge* start = edge->pair;
		HEHalfEdge* current = start;
		std::vector<HEVert*> loop;
		do {
			loop.push_back(current->vertex);
			visited.insert(current);
			current = current->next;
		} while (current != start);
		result.push_back(loop);
	}
	return result;
}

// 判断顶点是否在边界上
bool HEMesh::isBoundaryVertex(const HEVert* vert) const {

	HEHalfEdge* start = vert->edge;
	HEHalfEdge* current = start;
	do {
		if (current == nullptr || current->face == nullptr) return true;
		current = current->pair->next;
	} while (current != start);
	return false;
}

// 判断网格是否封闭
bool HEMesh::isClosed() const {
	for (HEHalfEdge* edge : getHalfEdges()) {
		if (isBoundary(edge)) return false;
	}
	return true;
}

// 网格完整性验证
bool HEMesh::validate() const {
	// 1. 每条半边的 next 不为空
	for (HEHalfEdge* edge : getHalfEdges()) {
		if (edge->next == nullptr) return false;
	}

	// 2. 每条半边的 face 不为空
	for (HEHalfEdge* edge : getHalfEdges()) {
		if (edge->face == nullptr) return false;
	}

	// 3. 三角形面的 next 链长度为 3 
	for (HEFace* face : getFaces()) {
		HEHalfEdge* start = face->edge;
		HEHalfEdge* current = start;
		int count = 0;
		do {
			count++;
			current = current->next;
		} while (current != start && count <= 3);
		if (count > 3) return false;
	}

	// 4. 每条半边的 pair->pair == 自身
	for (HEHalfEdge* edge : getHalfEdges()) {
		if (edge->pair->pair != edge) return false;
	}

	// 5. 每条半边的 vertex（起点）不为空
	for (HEHalfEdge* edge : getHalfEdges()) {
		if (edge->vertex == nullptr) return false;
	}
	// 6. 每个顶点的 edge 是以该顶点为起点的半边
	for (HEVert* vert : getVertices())
	{
		if (vert->edge->vertex != vert) return false;
	}

	// 7. 欧拉公式检查（封闭网格）：V - E/2 + F ≈ 2
	const size_t H = numHalfEdges();
	if (H % 2 != 0) return false;
	const size_t V = numVerts();
	const size_t F = numFaces();
	const size_t E = H / 2;
	if (static_cast<long long>(V) - static_cast<long long>(E) + static_cast<long long>(F) != 2) return false;
	// 全部通过
	return true;
}
