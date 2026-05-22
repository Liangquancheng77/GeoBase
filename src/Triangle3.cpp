#include "../include/GeoBase/Triangle3.h"
#include <cassert>
#include "../include/GeoBase/Plane.h"

// 构造函数
Triangle3::Triangle3(const Point3& v0, const Point3& v1, const Point3& v2)
	: v0(v0), v1(v1), v2(v2){
	//// 断言三角形不退化（面积大于一个小阈值）
	//assert(getArea() > EPS_ABS && "Triangle vertices must not be collinear.");
}

// 访问三角形的顶点
Point3 Triangle3::getVertex(int index) const
{
	assert(index >= 0 && index < 3 && "Vertex index must be 0, 1, or 2.");
	if (index == 0) return v0;
	if (index == 1) return v1;
	return v2;
}	

// 获取三角形的单位法线向量
Vector3 Triangle3::getNormal() const {
	Vector3 e1 = v1 - v0;
	Vector3 e2 = v2 - v0;
	Vector3 normal = e1.cross(e2);

	// 退化三角形返回零向量，避免除以0
	double len = normal.length();
	if (len < EPS_ABS) {
		return Vector3(0, 0, 0);
	}
	return normal / len;
}

// 计算三角形的面积
double Triangle3::getArea() const {
	return 0.5 * (v1 - v0).cross(v2 - v0).length();
}

//// 计算三角形的有向面积
//double Triangle3::getSignedArea() const {
//	Vector3 e1 = v1 - v0;
//	Vector3 e2 = v2 - v0;
//	Vector3 cross = e1.cross(e2);
//
//	return cross.length() * 0.5 * (cross.dot(getNormal()) > 0 ? 1.0 : -1.0);
//}

// 计算点p在三角形内的重心坐标(u, v, w)
Vector3 Triangle3::getBarycentric(const Point3& p) const {
	Vector3 v0p = p - v0;
	Vector3 v0v1 = v1 - v0;
	Vector3 v0v2 = v2 - v0;

	double d00 = v0v1.dot(v0v1);
	double d01 = v0v1.dot(v0v2);
	double d11 = v0v2.dot(v0v2);
	double d20 = v0p.dot(v0v1);
	double d21 = v0p.dot(v0v2);

	double denom = d00 * d11 - d01 * d01;
	if (std::abs(denom) < EPS_ABS) {
		// 退化三角形
		return Vector3(1.0 / 3.0, 1.0 / 3.0, 1.0 / 3.0);
	}

	double v = (d11 * d20 - d01 * d21) / denom;
	double w = (d00 * d21 - d01 * d20) / denom;
	double u = 1.0 - v - w;

	return Vector3(u, v, w);
}

// 判断点p是否在三角形内
bool Triangle3::contains(const Point3& p) const {
	Plane pPlane = Plane::fromPoints(v0, v1, v2);
	if (!pPlane.contains(p)) {
		return false;
	}
	Vector3 barycentric = getBarycentric(p);
	return barycentric.x + EPS_ABS >= 0 && barycentric.y + EPS_ABS >= 0 && barycentric.z + EPS_ABS >= 0;
}
// 获取三角形的轴对齐包围盒（AABB）
AABB Triangle3::getBoundingBox() const{
	AABB aabb(v0, v1);
	aabb.expand(v2);
	return aabb;
}

// 射线-三角形求交（Möller-Trumbore）
bool Triangle3::intersect(const Ray& ray, double& t, double& u, double& v, bool cullBackface) const {
	
	Vector3 A1 = v1 - v0;
	Vector3 A2 = v2 - v0;
	Vector3 S = ray.origin - v0;
	Vector3 Q = ray.direction.cross(A2);
	Vector3 R = S.cross(A1);

	double det = Q.dot(A1); 

	// 如果不剔除背面，det接近0表示平行或在三角形所在平面上；如果剔除背面，det小于等于0表示平行或背面
	if ((cullBackface && det < EPS_ABS) || (!cullBackface && std::abs(det) < EPS_ABS)) return false;
	
	double invDet = 1.0 / det;

	t = R.dot(A2) * invDet;
	u = Q.dot(S) * invDet;
	v = R.dot(ray.direction) * invDet;

	return t >= -EPS_ABS && u >= -EPS_ABS && v >= -EPS_ABS && (u + v) <= 1 + EPS_ABS;
}

// 带HitInfo的重载版本（通用接口）
bool Triangle3::intersect(const Ray& ray, HitInfo& info, bool cullBackface) const {
	double t, u, v;
	if (!intersect(ray, t, u, v, cullBackface)) {
		return false;
	}

	// 给HitInfo赋值
	info.t = t;
	info.point = ray.pointAt(t);
	info.normal = getNormal().normalized();

	// 根据射线方向修正法线朝向，保证法线始终朝向射线入射方向
	// 这样后面做光照、碰撞响应时，法线方向永远正确，不用额外判断
	if (ray.direction.dot(info.normal) > 0) {
		info.normal = Vector3(-info.normal.x, -info.normal.y, -info.normal.z);
	}

	return true;
}

// 判断两个三角形在向量的投影上是否分离（辅助函数）
inline bool isSeparated(const Triangle3& tri1, const Triangle3& tri2, const Vector3& axis) {
	double min1 = std::numeric_limits<double>::infinity();
	double max1 = -std::numeric_limits<double>::infinity();
	double min2 = std::numeric_limits<double>::infinity();
	double max2 = -std::numeric_limits<double>::infinity();
	for (int i = 0; i < 3; i++) {
		double proj1 = axis.dot(tri1.getVertex(i));
		min1 = std::min(min1, proj1);
		max1 = std::max(max1, proj1);
		double proj2 = axis.dot(tri2.getVertex(i));
		min2 = std::min(min2, proj2);
		max2 = std::max(max2, proj2);
	}
	return max1 < min2 - EPS_ABS || max2 < min1 - EPS_ABS;
}

// 判断与另一个三角形是否相交（使用分离轴定理）
bool Triangle3::intersect_sat(const Triangle3& other) const {
	// 1.获取两个三角形的法线
	Vector3 n1 = getNormal();
	Vector3 n2 = other.getNormal();

	// 2. 检查前两个分离轴：两个三角形的法线
	if (isSeparated(*this, other, n1)) return false;
	if (isSeparated(*this, other, n2)) return false;

	// 3. 处理共面情况（法线平行）
	if (std::abs(n1.dot(n2)) > 1.0 - EPS_ABS) {
		// 共面时，检查平面内的边法向量轴（共6个）
		Vector3 edges1[3] = {
			v1 - v0,
			v2 - v1,
			v0 - v2
		};
		Vector3 edges2[3] = {
			other.v1 - other.v0,
			other.v2 - other.v1,
			other.v0 - other.v2
		};

		// 检查本三角形的边法向量轴
		for (int i = 0; i < 3; i++) {
			Vector3 axis = edges1[i].cross(n1);
			if (axis.lengthSquared() > EPS_ABS * EPS_ABS) {
				if (isSeparated(*this, other, axis)) {
					return false;
				}
			}
		}

		// 检查另一个三角形的边法向量轴
		for (int i = 0; i < 3; i++) {
			Vector3 axis = edges2[i].cross(n1);
			if (axis.lengthSquared() > EPS_ABS * EPS_ABS) {
				if (isSeparated(*this, other, axis)) {
					return false;
				}
			}
		}

		// 所有共面轴都不分离，相交
		return true;
	}
	// 4.如果法线不平行，三角形所在平面必定相交，使用分离轴定理（SAT）检查是否分离

	Vector3 edges1[3] = { v1 - v0, v2 - v1, v0 - v2 };
	Vector3 edges2[3] = { other.v1 - other.v0, other.v2 - other.v1, other.v0 - other.v2 };
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			Vector3 axis = edges1[i].cross(edges2[j]);
			if (axis.lengthSquared() > EPS_ABS * EPS_ABS) { // 只有非零轴才有意义
				if (isSeparated(*this, other, axis)) return false;
			}
		}
	}

	return true;
}

//// 判断与另一个三角形是否相交（效率更高的实现）
//bool Triangle3::triTriIntersect(const Triangle3& other) const {
//	// 1.获取两个三角形的法线
//	Vector3 n1 = getNormal();
//	Vector3 n2 = other.getNormal();
//
//	// 2.如果法线平行，三角形所在平面可能重叠或分离，先检查是否共面
//	if (std::abs(n1.dot(n2)) > 1.0 - EPS_ABS) {
//		// 共面：检查一个三角形的顶点是否在另一个三角形内
//		return contains(other.v0) || contains(other.v1) || contains(other.v2) ||
//			other.contains(v0) || other.contains(v1) || other.contains(v2);
//	}
//	// 3.一个三角形的三个顶点到另一个三角形所在平面的距离符号必须不全相同，否则必定分离
//	double d0 = n2.dot(v0 - other.v0);
//	double d1 = n2.dot(v1 - other.v0);
//	double d2 = n2.dot(v2 - other.v0);
//	if ((d0 > -EPS_ABS && d1 > -EPS_ABS && d2 > -EPS_ABS) || (d0 < EPS_ABS && d1 < EPS_ABS && d2 < EPS_ABS)) return false;
//	// 同理，另一个三角形的三个顶点到第一个三角形所在平面的距离符号也必须不全相同，否则必定分离
//	double d3 = n1.dot(other.v0 - v0);
//	double d4 = n1.dot(other.v1 - v0);
//	double d5 = n1.dot(other.v2 - v0);
//	if ((d3 > -EPS_ABS && d4 > -EPS_ABS && d5 > -EPS_ABS) || (d3 < EPS_ABS && d4 < EPS_ABS && d5 < EPS_ABS)) return false;
//
//	// 4.如果法线不平行，三角形所在平面必定相交，求出交线段所在的直线的方向单位向量
//	Vector3 lineDir = n1.cross(n2).normalized();
//	// 5.将两个三角形投影到交线段所在的直线上，得到两个线段，检查这两个线段是否重叠
//	return !isSeparated(*this, other, lineDir);
//
//}
//
//// 判断与另一个三角形是否相交（使用分离轴定理）并返回两个交点（如果有）
//bool Triangle3::intersect_sat(const Triangle3& other, Point3& intersectionPoint1, Point3& intersectionPoint2) const {
//	// 目前实现了是否相交的判断，返回交点需要更复杂的计算，暂时留空
//	intersectionPoint1 = Point3(0, 0, 0);
//	intersectionPoint2 = Point3(0, 0, 0);
//	return intersect_sat(other);
//}