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