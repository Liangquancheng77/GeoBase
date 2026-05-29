#include "../include/GeoBase/HEMesh.h"
#include <unordered_map>
#include <iostream>
#include <fstream>   
#include <string>
#include <unordered_set>
#include <assert.h>

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


// 获取或创建一对半边
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
	}
	else {
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

// 辅助函数:创建一个面
HEFace* HEMesh::createFace(HEHalfEdge* he) {
	HEFace* f = new HEFace();
	f->edge = he;
	f->index = numFaces();
	m_faces.push_back(f);
	return f;
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
	if (!face || numVerts() == 0 || numFaces() == 0) return;
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
	std::function<bool(HEHalfEdge*)> callback) {
	if (!vert || numVerts() == 0 || numFaces() == 0) return;
	HEHalfEdge* start = vert->edge;
	HEHalfEdge* current = start;
	do {
		if (!callback(current)) {  // 一旦返回 false，立刻退出
			break;
		}
		current = current->pair->next;
	} while (current != start);
}

// 顶点邻点遍历
void HEMesh::forEachNeighborVertex(HEVert* vert,
	std::function<bool(HEVert*)> callback) {
	if (!vert || numVerts() == 0 || numFaces() == 0) return;
	forEachOutgoingHalfEdge(vert, [&](HEHalfEdge* he) {
		if (!callback(he->pair->vertex)) {
			return false;
		}
		});
}

// 顶点邻面遍历
void HEMesh::forEachFaceAroundVertex(HEVert* vert,
	std::function<bool(HEFace*)> callback) {
	if (!vert || numVerts() == 0 || numFaces() == 0) return;
	forEachOutgoingHalfEdge(vert, [&](HEHalfEdge* he) {
		if (he->face != nullptr)
		{
			if (!callback(he->face)) {
				return false;
			}
		}
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
	for (HEHalfEdge* edge : getHalfEdges()) {
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
	//for (HEFace* face : getFaces()) {
	//	HEHalfEdge* start = face->edge;
	//	HEHalfEdge* current = start;
	//	int count = 0;
	//	do {
	//		count++;
	//		current = current->next;
	//	} while (current != start && count <= 3);
	//	if (count > 3) return false;
	//}

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

// 更新m_edgeMap，删除he原来的key，新增from、to的key
void HEMesh::updateEdgeMap(HEHalfEdge* he, HEVert* from, HEVert* to) {
	int idxF1 = he->vertex->index;
	int idxF2 = from->index;
	int idxT1 = he->pair->vertex->index;
	int idxT2 = to->index;
	
	EdgeKey key2 = idxF2 < idxT2 ? EdgeKey(idxF2, idxT2) : EdgeKey(idxT2, idxF2);
	auto it2 = m_edgeMap.find(key2);
	if (it2 == m_edgeMap.end()) {
		m_edgeMap[key2] = idxF2 < idxT2 ? he : he->pair;
	}
	// 移除旧的缓存（也可以考虑不移除）
	EdgeKey key1 = idxF1 < idxT1 ? EdgeKey(idxF1, idxT1) : EdgeKey(idxT1, idxF1);
	auto it1 = m_edgeMap.find(key1);
	if (it1 != m_edgeMap.end()) {
		m_edgeMap.erase(key1);
	}
}

// 将半边的起点修改为vert
void HEMesh::updateEdgeVertex(HEHalfEdge* he, HEVert* vert) {
	int idxF1 = he->vertex->index;
	int idxF2 = vert->index;
	int idxT = he->pair->vertex->index;
	he->vertex = vert;
	EdgeKey key = idxF2 < idxT ? EdgeKey(idxF2, idxT) : EdgeKey(idxT, idxF2);
	auto it = m_edgeMap.find(key);
	if (it == m_edgeMap.end()) {
		m_edgeMap[key] = idxF2 < idxT ? he : he->pair;
	}
	// 移除旧的缓存（也可以考虑不移除）
	EdgeKey key2 = idxF1 < idxT ? EdgeKey(idxF1, idxT) : EdgeKey(idxT, idxF1);
	auto it2 = m_edgeMap.find(key2);
	if (it2 != m_edgeMap.end()) {
		m_edgeMap.erase(key2);
	}

}


// 边翻转
bool HEMesh::flipEdge(HEHalfEdge* he) {
	HEHalfEdge* pair = he->pair;
	HEFace* f1 = he->face;
	HEFace* f2 = pair->face;
	// 判断两个半边是否都是内部边
	if (f1 == nullptr || f2 == nullptr) return false;
	// 判断两个是否都是三角形
	HEHalfEdge* AB = he->next;
	if (AB == nullptr) return false;
	HEHalfEdge* BC = AB->next;
	if (BC == nullptr || BC->next != he) return false;
	HEHalfEdge* CD = pair->next;
	if (CD == nullptr) return false;
	HEHalfEdge* DA = CD->next;
	if (DA == nullptr || DA->next != pair) return false;

	HEVert* A = AB->vertex;
	HEVert* B = BC->vertex;
	HEVert* C = he->vertex;
	HEVert* D = DA->vertex;

	// 判断两个三角形合并后是否是凸四边形
	// A、C是否在BD两侧
	Vector3 crossA_BD = Vector3(D->position - B->position).cross(A->position - B->position);
	Vector3 crossC_BD = Vector3(D->position - B->position).cross(C->position - B->position);
	// 大于0表示同侧，等于0表示四边形退化为大三角形
	if (crossA_BD.dot(crossC_BD) >= -EPS_ABS) return false;
	// B、D是否在AC两侧
	Vector3 crossB_AC = Vector3(C->position - A->position).cross(B->position - A->position);
	Vector3 crossD_AC = Vector3(C->position - A->position).cross(D->position - A->position);
	if (crossB_AC.dot(crossD_AC) >= -EPS_ABS) return false;

	// 更新map
	updateEdgeMap(he, D, B);

	// A、C的半边可能是内边，避免出错所以需要更新
	A->edge = AB;
	C->edge = CD;

	// 两个面的半边避免出错也要更新
	f1->edge = he;
	f2->edge = pair;

	// 两个对角线的内边：起点改变、pair不变、next改变、face不变
	he->vertex = D;
	he->next = BC;
	pair->vertex = B;
	pair->next = DA;


	// AB、CD：起点不变、pair不变、next改变、face改变
	AB->next = pair;
	AB->face = f2;
	CD->next = he;
	CD->face = f1;

	// BC、DA：起点不变、pair不变、next改变、face不变
	BC->next = CD;
	DA->next = AB;
	return true;

}

// 获取顶点的所有邻点
std::vector<HEVert*> HEMesh::get_neighbors(HEVert* vert) {
	std::vector<HEVert*> results;
	forEachNeighborVertex(vert, [&](HEVert* nVert) {
		results.push_back(nVert);
		return true;
		});
	return results;
}

// 边折叠
bool HEMesh::collapseEdge(HEHalfEdge* he, Point3* newPosition) {
	// 1.1 校验能否折叠
	HEVert* from = he->vertex;
	HEVert* to;
	HEHalfEdge* pair = he->pair;
	if (newPosition == nullptr)
	{
		HEHalfEdge* pair = he->pair;
		if (pair == nullptr)
		{
			return false;
		}
		to = pair->vertex;
	}
	else
	{
		to = findOrCreateVertex(*newPosition);
	}

	// 1.2 判断顶点类型
	bool toIsBround = false;
	bool fromIsBround = false;
	if (newPosition == nullptr) {
		forEachOutgoingHalfEdge(to, [&](HEHalfEdge* he) {
			if (he->face == nullptr || he->pair->face == nullptr)
			{
				toIsBround = true;
			}
			return true;
			});
	}

	// 记录需要执行修改起点的半边
	std::vector<HEHalfEdge*> toUpdateEdges;
	forEachOutgoingHalfEdge(from, [&](HEHalfEdge* he) {
		if (he->face == nullptr || he->pair->face == nullptr)
		{
			fromIsBround = true;
		}
		toUpdateEdges.push_back(he);
		return true;
		});

	// 1.3 都是边界点
	// 记录需要执行操作的面、半边
	// 要删除的from临边的半边
	HEHalfEdge* n = nullptr;
	HEHalfEdge* pairN = nullptr;


	// 要删除的面
	HEFace* f1 = he->face;
	HEFace* f2 = pair->face;

	if (fromIsBround && toIsBround)
	{
		// 两个点是不是邻点
		bool flag = false;
		forEachNeighborVertex(from, [&](HEVert* vert) {
			if (vert == to)
			{
				flag = true;
				return false;
			}
			return true;
			});
		// 不是邻点，非法折叠
		if (!flag) return false;
		// 至少存在一个面不为空，且只能是三角形
		if (f1 == nullptr && f2 == nullptr) return false;


	}
	else {
		// 不全是边界的点，两个面都要非空且都是三角形
		if (f1 == nullptr || f2 == nullptr) return false;
	}

	if (f1 != nullptr && he->next != nullptr && he->next->next != nullptr) {
		n = he->next->next;
		if (n->next != he) {
			// 非三角形
			return false;
		}
	}
	if (f2 != nullptr && pair->next != nullptr && pair->next->next != nullptr) {
		pairN = pair->next;
		if (pairN->next->next != pair) {
			// 非三角形
			return false;
		}
	}

	// 1.4 如果起点是边界点、终点的内部点，非法折叠
	if (fromIsBround && !toIsBround) return false;
	// 1.5 其余的情况
	if (newPosition == nullptr && !fromIsBround) {
		// 链接条件检查：内部边折叠要求 link(from) ∩ link(to) == 2
		auto f = get_neighbors(from);
		auto t = get_neighbors(to);
		std::unordered_set<HEVert*> setF(f.begin(), f.end());
		int common = 0;
		for (HEVert* tVert : t) {
			if (setF.count(tVert)) common++;
		}
		if (common != 2) return false;
	}
	// 2. 折叠合法,执行修改和删除操作
	// 2.1 可能要修改的点,删除的半边中以from为终点的半边的起点
	pair->vertex->edge = he->next;
	pairN->pair ->vertex->edge = pairN->next;
	n->vertex->edge = n->pair->next;

	// 2.2 可能要修改的面，n的pair和pairN的pair所在的面（如果存在的话）
	if (n->pair->face != nullptr) {
		n->pair->face->edge = he->next->pair;
	}
	if (pairN->pair->face != nullptr) {
		pairN->pair->face->edge = pairN->next;
	}

	// 2.3 以from为起点的半边的起点修改为to,同时更新map
	for (HEHalfEdge* toUpdateEdge : toUpdateEdges)
	{
		if (toUpdateEdge == he) continue;
		toUpdateEdge->vertex = to;
		updateEdgeVertex(toUpdateEdge, to);
	}
	// 2.4 删除from
	m_vertMap.erase(he->vertex->position);
	auto it1 = std::find(m_verts.begin(), m_verts.end(), he->vertex);
	if (it1 != m_verts.end()) {
		m_verts.erase(it1);
	}

	delete he->vertex;

	// 2.5 删除两个face（如果不为空）
	if (f1 != nullptr) {
		auto it5 = std::find(m_faces.begin(), m_faces.end(), f1);
		if (it5 != m_faces.end()) {
			m_faces.erase(it5);
		}

		delete f1;
	}
	if (f2 != nullptr) {
		auto it5 = std::find(m_faces.begin(), m_faces.end(), f2);
		if (it5 != m_faces.end()) {
			m_faces.erase(it5);
		}

		delete f2;
	}

	// 2.6 删除半边(折叠半边与其pair、from相邻的半边与其pair)

	// 折叠半边与其pair
	int fromIdx = from->index;
	int toIdx = to->index;
	EdgeKey k1 = fromIdx < toIdx ? EdgeKey(fromIdx, toIdx) : EdgeKey(fromIdx, toIdx);
	m_edgeMap.erase(k1);
	auto it2 = std::find(m_edges.begin(), m_edges.end(), he);
	if (it2 != m_edges.end()) {
		m_edges.erase(it2);
	}
	auto it3 = std::find(m_edges.begin(), m_edges.end(), pair);
	if (it3 != m_edges.end()) {
		m_edges.erase(it3);
	}
	delete he;
	delete pair;

	// from相邻的半边与其pair
	if (n != nullptr) {
		int fIdx = n->index;
		int tIdx = n->pair->index;
		EdgeKey k2 = fIdx < tIdx ? EdgeKey(fIdx, tIdx) : EdgeKey(fIdx, tIdx);
		m_edgeMap.erase(k2);
		auto it3 = std::find(m_edges.begin(), m_edges.end(), n);
		if (it3 != m_edges.end()) {
			m_edges.erase(it3);
		}
		auto it4 = std::find(m_edges.begin(), m_edges.end(), n->pair);
		if (it4 != m_edges.end()) {
			m_edges.erase(it4);
		}
		delete n->pair;
		delete n;
	}

	if (pairN != nullptr) {
		int fIdx = pairN->index;
		int tIdx = pairN->pair->index;
		EdgeKey k2 = fIdx < tIdx ? EdgeKey(fIdx, tIdx) : EdgeKey(fIdx, tIdx);
		m_edgeMap.erase(k2);
		auto it3 = std::find(m_edges.begin(), m_edges.end(), pairN);
		if (it3 != m_edges.end()) {
			m_edges.erase(it3);
		}
		auto it4 = std::find(m_edges.begin(), m_edges.end(), pairN->pair);
		if (it4 != m_edges.end()) {
			m_edges.erase(it4);
		}
		delete pairN->pair;
		delete pairN;
	}

	return true;
}

// 创建顶点X并且添加相关的半边
HEVert* HEMesh::creatVertXAndAddEdges(HEVert* A, HEVert* B, HEVert* C, double t, HEHalfEdge*& XC, HEHalfEdge*& XB) {
	const Point3& pos = (B->position - A->position) * t + A->position;
	HEVert* X = findOrCreateVertex(pos);
	XC = findOrCreateHalfEdge(X, C);
	XB = findOrCreateHalfEdge(X, B);
	return X;
}

// 边分割
HEVert* HEMesh::splitEdge(HEHalfEdge* he, double t) {
	if (t < EPS_ABS || t > 1 - EPS_ABS) {
		assert(false && "splitEdge: t out of valid range (0, 1)");
		return nullptr;
	}

	HEFace* f1 = he->face;
	HEFace* f2 = he->pair->face;

	// 孤立边（两侧均无面）不允许分割
	if (!f1 && !f2) {
		assert(false && "splitEdge: isolated edge, no adjacent face");
		return nullptr;
	}

	// 确保如果只有一侧有面，则 he 指向有面一侧
	if (!f1 && f2) {
		he = he->pair;
		std::swap(f1, f2);
	}

	HEVert* A = he->vertex;
	HEVert* B = he->pair->vertex;
	HEHalfEdge* BC = he->next;
	HEHalfEdge* CA = he->next->next;
	HEVert* C = CA->vertex;

	// 校验是否是三角形
	if (CA->next != he) {
		assert(false && "splitEdge: no a Triangle3");
		return nullptr;
	}

	if (f2) {
		// 内部边
		HEHalfEdge* AD = he->pair->next;
		HEHalfEdge* DB = AD->next;
		if (DB->next != he->pair) {
			assert(false && "splitEdge: no a Triangle3");
			return nullptr;
		}
		HEVert* D = DB->vertex;
		HEHalfEdge* XC = nullptr;
		HEHalfEdge* XB = nullptr;
		//he->face = nullptr;
		//he->pair->face = nullptr;
		HEVert* X = creatVertXAndAddEdges(A, B, C, t, XC, XB);

		HEHalfEdge* XD = findOrCreateHalfEdge(X, D);

		// 新增两个面
		HEFace* newF1 = createFace(BC);
		HEFace* newF2 = createFace(AD);

		// 新增的X设置出边
		X->edge = XC;

		// 修改可能发生变化的原来的顶点
		B->edge = BC;

		// 修改可能发生变化的原来的面
		f1->edge = he;
		f2->edge = XB->pair;

		// 修改起点位置发生变化的半边(AB->AX,BA->XA)
		updateEdgeMap(he, A, X);
		he->pair->vertex = X;

		// 修改半边的面
		XC->face = f1;
		XB->pair->face = f2;
		XD->face = f2;
		XC->pair->face = newF1;
		XB->face = newF1;
		BC->face = newF1;
		XD->pair->face = newF2;
		he->pair->face = newF2;
		AD->face = newF2;

		// 修改半边的next
		// 三角形CAX
		he->next = XC;
		XC->next = CA;
		// 三角形CXB
		XC->pair->next = XB;
		XB->next = BC;
		BC->next = XC->pair;
		// 三角形DBX
		DB->next = XB->pair;
		XB->pair->next = XD;
		XD->next = DB;
		// 三角形DXA
		XD->pair->next = he->pair;
		he->pair->next = AD;
		AD->next = XD->pair;
		return X;
	}
	else {
		// 边界边
		HEHalfEdge* XC = nullptr;
		HEHalfEdge* XB = nullptr;
		HEVert* X = creatVertXAndAddEdges(A, B, C, t, XC, XB);

		// 新增一个面
		HEFace* newF1 = createFace(BC);

		// 修改可能发生变化的原来的顶点
		B->edge = BC;

		// 修改可能发生变化的原来的面
		f1->edge = he;

		// 修改起点位置发生变化的半边(AB->AX,BA->XA)
		updateEdgeMap(he, A, X);
		he->pair->vertex = X;

		// 修改半边的面
		XC->face = f1;
		XC->pair->face = newF1;
		XB->face = newF1;
		BC->face = newF1;

		// 修改半边的next
		// 三角形CAX
		he->next = XC;
		XC->next = CA;
		// 三角形CXB
		XC->pair->next = XB;
		XB->next = BC;
		BC->next = XC->pair;
		return X;

	}

}

// 辅助：从三角形面提取三个顶点
void getFaceVertices(const HEFace* face, Point3& A, Point3& B, Point3& C) {
	HEHalfEdge* he = face->edge;
	A = he->vertex->position;
	B = he->next->vertex->position;
	C = he->next->next->vertex->position;
}

// 计算三角形的面积
double HEMesh::triangleArea(const HEFace* face) const {
	if (!face) return 0.0;
	Point3 A, B, C;
	getFaceVertices(face, A, B, C);
	Vector3 AB = B - A;
	Vector3 AC = C - A;
	return 0.5 * std::abs(AB.cross(AC).length());
}

// 最长边与最短边的比值
double HEMesh::triangleAspectRatio(const HEFace* face) const {
	if (!face) return 0.0;
	Point3 A, B, C;
	getFaceVertices(face, A, B, C);
	double ABLength = (B - A).length();
	double ACLength = (C - A).length();
	double BCLength = (C - B).length();
	double minLen = std::min({ ABLength, ACLength, BCLength });
	if (minLen < EPS_ABS) return std::numeric_limits<double>::infinity();
	double maxLen = std::max({ ABLength, ACLength, BCLength });
	return maxLen / minLen;
}


// 最小内角
double HEMesh::minAngle(const HEFace* face) const {
	if (!face) return 0.0;
	Point3 A, B, C;
	getFaceVertices(face, A, B, C);
	double c = (B - A).length();
	double b = (C - A).length();
	double a = (C - B).length();

	// 余弦定理求角度
	auto angle = [](double opp, double adj1, double adj2) -> double {
		double denom = 2.0 * adj1 * adj2;
		if (denom < EPS_ABS) return 0.0;
		double cosVal = (adj1 * adj1 + adj2 * adj2 - opp * opp) / denom;
		cosVal = std::max(-1.0, std::min(1.0, cosVal));
		return std::acos(cosVal);
		};

	double angleA = angle(a, b, c);
	double angleB = angle(b, a, c);
	double angleC = angle(c, a, c);
	return std::min({ angleA, angleB, angleC });
}


// 网格质量统计报告
void HEMesh::meshQualityReport() const {

	int totalFaces = numFaces();
	if (totalFaces == 0)
	{
		cout << "No faces in mesh.\n" << endl;
		return;
	}

	double totalArea = 0.0;
	double minArea = std::numeric_limits<double>::max();
	double maxArea = 0.0;
	double minAngleGlobal = std::numeric_limits<double>::max();
	double maxAngleGlobal = 0.0;
	int degenerateCount = 0;

	for (HEFace* face : getFaces()) {
		double area = triangleArea(face);

		auto angle = [](double opp, double adj1, double adj2) -> double {
			double denom = 2.0 * adj1 * adj2;
			if (denom < EPS_ABS) return 0.0;
			double cosVal = (adj1 * adj1 + adj2 * adj2 - opp * opp) / denom;
			cosVal = std::max(-1.0, std::min(1.0, cosVal));
			return std::acos(cosVal);
			};
		Point3 A, B, C;
		getFaceVertices(face, A, B, C);
		double c = (B - A).length();
		double b = (C - A).length();
		double a = (C - B).length();

		double angleA = angle(a, b, c);
		double angleB = angle(b, a, c);
		double angleC = angle(c, a, b);

		double minAngle = std::min({ angleA ,angleB ,angleC });
		double maxAngle = std::max({ angleA ,angleB ,angleC });

		totalArea += area;
		minArea = std::min(minArea, area);
		maxArea = std::max(maxArea, area);
		minAngleGlobal = std::min(minAngleGlobal, minAngle);
		maxAngleGlobal = std::max(maxAngleGlobal, maxAngle);

		// 退化判断：面积过小或最小角过小
		if (minAngle < EPS_REL || area < EPS_ABS) {
			++degenerateCount;
		}

	}

	double avgArea = totalArea / totalFaces;

	// 转换为度便于阅读
	auto rad2deg = [](double rad) { return rad * 180.0 / PI; };

	std::cout << "========== Mesh Quality Report ==========\n";
	std::cout << "Number of faces: " << totalFaces << "\n";
	std::cout << "Average area: " << avgArea << "\n";
	std::cout << "Minimum area: " << minArea << "\n";
	std::cout << "Maximum area: " << maxArea << "\n";
	std::cout << "Minimum interior angle: " << minAngleGlobal << " rad (" << rad2deg(minAngleGlobal) << "°)\n";
	std::cout << "Maximum interior angle: " << maxAngleGlobal << " rad (" << rad2deg(maxAngleGlobal) << "°)\n";
	std::cout << "Number of degenerate faces: " << degenerateCount << "\n";
	std::cout << "=========================================\n";

}
