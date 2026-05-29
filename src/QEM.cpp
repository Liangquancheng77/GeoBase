#include "../include/GeoBase/QEM.h"

// 零矩阵
Quadric::Quadric() {
	memset(m, 0, sizeof(m));
}

// 从平面构建Q矩阵
Quadric::Quadric(const Vector3& normal, const Point3& point) : Quadric(normal.x, normal.y, normal.z, -(normal.dot(point))) {}

// 直接从平面的四个参数构建
Quadric::Quadric(double a, double b, double c, double d) {
	m[0] = a * a; m[1] = a * b; m[2] = a * c; m[3] = a * d;
	m[4] = b * b; m[5] = b * c; m[6] = b * d;
	m[7] = c * c; m[8] = c * d; m[9] = d * d;
}

Quadric& Quadric::operator+=(const Quadric& other) {
	for (int i = 0; i < 10; i++)
		m[i] += other.m[i];
	return *this;
}

Quadric Quadric::operator+(const Quadric& other) const {
	Quadric ret = *this;
	for (int i = 0; i < 10; i++)
	{
		ret.m[i] += other.m[i];
	}
	return ret;
}

// 计算在点p处的误差：p^T * Q * p
double Quadric::evaluate(const Point3& p) const {
	double x = p.x, y = p.y, z = p.z;
	return x * x * m[0] + 2 * x * y * m[1] + 2 * x * z * m[2] + 2 * x * m[3]
		+ y * y * m[4] + 2 * y * z * m[5] + 2 * y * m[6]
		+ z * z * m[7] + 2 * z * m[8]
		+ m[9];
}

// 求解最优位置，返回是否可解
bool Quadric::solveOptimal(Point3& result) const {
    // A * x = -b,  A = 3x3, b = [m[3], m[6], m[8]]^T
    double A[3][3] = {
        { m[0], m[1], m[2] },
        { m[1], m[4], m[5] },
        { m[2], m[5], m[7] }
    };
    double b[3] = { -m[3], -m[6], -m[8] };

    // 求解 3x3 线性方程组 (A * x = b)
    // 使用克莱姆法则，计算行列式
    double det = A[0][0] * (A[1][1] * A[2][2] - A[1][2] * A[2][1])
        - A[0][1] * (A[1][0] * A[2][2] - A[1][2] * A[2][0])
        + A[0][2] * (A[1][0] * A[2][1] - A[1][1] * A[2][0]);

    if (std::fabs(det) < EPS_ABS)
        return false;   // 不可逆，无唯一最优解

    double invDet = 1.0 / det;
    // 计算伴随矩阵的转置与 b 的乘积
    double x = (b[0] * (A[1][1] * A[2][2] - A[1][2] * A[2][1]) +
        b[1] * (A[0][2] * A[2][1] - A[0][1] * A[2][2]) +
        b[2] * (A[0][1] * A[1][2] - A[0][2] * A[1][1])) * invDet;
    double y = (b[0] * (A[1][2] * A[2][0] - A[1][0] * A[2][2]) +
        b[1] * (A[0][0] * A[2][2] - A[0][2] * A[2][0]) +
        b[2] * (A[0][2] * A[1][0] - A[0][0] * A[1][2])) * invDet;
    double z = (b[0] * (A[1][0] * A[2][1] - A[1][1] * A[2][0]) +
        b[1] * (A[0][1] * A[2][0] - A[0][0] * A[2][1]) +
        b[2] * (A[0][0] * A[1][1] - A[0][1] * A[1][0])) * invDet;

    result = Point3(x, y, z);
    return true;
}
// 将 (row, col) 映射到上三角存储索引，row <= col
static inline int idx(int row, int col) {
    if (row > col) std::swap(row, col);
    if (row == 0) return col;
    if (row == 1) return col + 3;
    if (row == 2) return col + 5;
    return 9;
}

// 访问底层矩阵元素
double& Quadric::operator()(int row, int col) {
    return m[idx(row, col)];
}

double Quadric::operator()(int row, int col) const {
    return m[idx(row, col)];
}
