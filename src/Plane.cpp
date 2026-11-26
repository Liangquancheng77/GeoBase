#include "../include/GeoBase/Plane.h"
#include "../include/GeoBase/common.h"
#include <cassert>

// 构造函数
Plane::Plane(const Vector3& normal, const Point3& origin) {
	// 断言法向量不能是零向量
	assert(normal.length() > 1e-9 && "Plane normal vector cannot be zero.");
	this->normal = normal.normalized();  // 确保法向量是单位向量
	this->origin = origin;
}

// 三点确定一个平面
Plane Plane::fromPoints(const Point3& p0, const Point3& p1, const Point3& p2) {
	Vector3 edge1 = p1 - p0;
	Vector3 edge2 = p2 - p0;
	Vector3 normal = edge1.cross(edge2).normalized();
	return Plane(normal, p0);
}

// 归一化法线
void Plane::normalize() {
	this->normal = this->normal.normalized();
}

// 计算点到平面的有符号距离
double Plane::signedDistance(const Point3& point) const {
	// 点到平面的距离 = (点 - 平面上任意一点) · 法向量
	Vector3 vecToPoint = point - origin;
	return vecToPoint.dot(normal);
}


// ===== 新增：点到平面距离的误差传播实验 =====// 已有的 signedDistance() 一行都没改，下面四个都是新函数。
// 不把方向调成单位长，直接点乘。// 方向长度是 k，算出来的距离就是正确距离的 k 倍。
double Plane::signedDistanceWithRawNormal(const Point3& point,
	const Vector3& rawNormal) const {
	Vector3 vecToPoint = point - origin;
	return vecToPoint.dot(rawNormal);
}

// 把单位法向量绕 y 轴转 tilt 弧度，再算距离。// 转完仍是单位长，所以这里纯粹是「方向偏了一点」。
double Plane::signedDistanceWithTiltedNormal(const Point3& point,
	double tilt) const {
	Vector3 vecToPoint = point - origin;
	double ct = std::cos(tilt);
	double st = std::sin(tilt);
	// 绕 y 轴转：新的 x 是 x*ct + z*st，新的 z 是 -x*st + z*ct。
	Vector3 tilted(normal.x * ct + normal.z * st,
		normal.y,
		-normal.x * st + normal.z * ct);
	return vecToPoint.dot(tilted);
}

// 输入差一丁点，输出最多差多少倍。// 分子是点到平面基准点的距离，分母是点到平面的距离；
// 分母很小时这个倍数会很大，也就是「离基准点远、又几乎贴在平面上」最危险。
double Plane::distanceAmplification(const Point3& point) const {
	double d = signedDistance(point);
	double w = (point - origin).length();
	if (std::abs(d) < 1e-300) return 0.0;
	return w / std::abs(d);
}

// 把算错的距离当成正确答案，反推点本来该在哪。// 沿法向挪「错误距离减正确距离」那么多，再代入 signedDistance 正好得错误距离。
Point3 Plane::pointThatExplainsDistance(double wrongDistance,
	const Point3& point) const {
	double correct = signedDistance(point);
	return point + normal * (wrongDistance - correct);
}



// 计算点在平面上的投影点
Point3 Plane::projectPoint(const Point3& point) const {
	double distance = signedDistance(point);
	// 投影点 = 原点 - 距离 * 法向量
	return point - normal * distance;
}

// 判断点是否在平面上（误差范围内）
bool Plane::contains(const Point3& point, double eps) const {
	return std::fabs(signedDistance(point)) < eps;
}

// 判断点相对于平面的位置（正面/背面/在平面上）
PlaneSide Plane::getSide(const Point3& point, double eps) const {
	double distance = signedDistance(point);
	if (distance > eps) {
		return PlaneSide::FRONT;
	}
	if (distance < -eps) {
		return PlaneSide::BACK;
	}
	return PlaneSide::ON_PLANE;
}

// 判断射线是否与平面相交，返回交点参数t和交点坐标
bool Plane::intersect(const Ray& ray, double& t, Point3& hitPoint) const {
	double  denom = ray.direction.dot(normal);
	if (std::abs(denom) < EPS_ABS) {
		return false; // 射线与平面平行或者在平面上，无有效交点
	}
	t = (origin - ray.origin).dot(normal) / denom;
	if (t + EPS_ABS < 0)
	{
		return false;
	}
	hitPoint = ray.pointAt(t);
	return true;
}

// intersect重载
bool Plane::intersect(const Ray& ray, HitInfo& info) const {
	double t;
	Point3 hitPoint;
	if (!intersect(ray, t, hitPoint)) {
		return false;
	}

	info.t = t;
	info.point = hitPoint;
	info.normal = normal.normalized();

	// 同样修正法线朝向
	if (ray.direction.dot(info.normal) > 0) {
		info.normal = Vector3(-info.normal.x, -info.normal.y, -info.normal.z);
	}
	return true;
}
