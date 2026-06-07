#include "../include/GeoBase/GJK.h"
#include <array>
#include "../include/GeoBase/Plane.h"

    // 支持函数：返回形状在direction方向上投影最大的顶点
    Point3 GJK::support(const std::vector<Point3>& shape, const Vector3& dir) {
        if (shape.empty()) return Point3();
        Point3 bestVertex;
		double bestDot = std::numeric_limits<double>::lowest();
        for (const Point3& point : shape)
        {
			double dot = point.dot(dir);
            if (dot > bestDot) {
				bestDot = dot;
				bestVertex = point;
            }
        }
        return bestVertex;
    }

    // Minkowski差的支持函数
    GJK::Vertex GJK::supportMinkowski(const std::vector<Point3>& shapeA,
        const std::vector<Point3>& shapeB,
        const Vector3& dir) {
        Point3 A = support(shapeA, dir);
        Point3 B = support(shapeB, dir * (-1));
        return Vertex(A - B, A, B);
    }

    //// 判断原点是否在线段/三角形内
    //bool GJK::containsOrigin(std::vector<Point3>& simplex, Vector3& direction) {

    //}

    // 单纯形进化（2D）
    bool GJK::updateSimplex2D(std::vector<Vertex>& simplex, Vector3& direction) {
        // 2D单纯形可能是点、线段或三角形
        if (simplex.size() == 2)
        {
            // 线段情况
			//Point3& A = simplex[1]; // 最新添加的点
			//Point3& B = simplex[0];
			//Vector3 AB = B - A;
			//Vector3 AO = A * (-1);
   //         if (AB.dot(AO) <= 0) {
			//	// 最近点为 A，单纯形缩减为 {A}，方向 -A
			//	simplex.erase(simplex.begin());
			//	direction = AO; // 方向指向原点
   //         }

			//else if (AB.dot(AO) >= AB.dot(AB)) {
			//	// 最近点为 B，单纯形缩减为 {B}，方向 -B
			//	simplex.erase(simplex.begin() + 1);
			//	direction = (B * (-1)); // 方向指向原点
			//}

   //         else {
   //             // 原点在AB线段上，方向垂直于AB指向原点
   //             direction = AB.cross(AO).cross(AB);
   //         }

            direction = (simplex[1].point - simplex[0].point).cross(simplex[1].point).cross(simplex[0].point - simplex[1].point);
		}
        else if (simplex.size() == 3)
        {
			// 三角形情况
			Point3& A = simplex[2].point; // 最新添加的点
			Point3& B = simplex[1].point;
			Point3& C = simplex[0].point;
			Vector3 BA = A - B;
			Vector3 CA = A - C;
			Vector3 ABCNormal = BA.cross(CA);
			Vector3 AO = A * (-1);
			Vector3 newDirection;
            if ((newDirection = ABCNormal.cross(BA)).dot(AO) > 0)
            {
                // 原点在BA外侧
                direction = newDirection;
				// 移除点C
				simplex.erase(simplex.begin());
            }
            else if ((newDirection = CA.cross(ABCNormal)).dot(AO) > 0) {
                // 原点在CA外侧
                direction = newDirection;
				// 移除点B
				simplex.erase(simplex.begin() + 1);
            }
            else {
                // 原点在三角形内部，说明相交
                return true;
            }
        }
		return false; // 需要继续迭代

    }

    // 2D版本：判断两个凸多边形是否相交
    bool GJK::intersect2D(const std::vector<Point3>& shapeA, const std::vector<Point3>& shapeB) {
        // 1.初始方向可以任意选择，这里选择从shapeA的第一个顶点指向shapeB的第一个顶点
		Vector3 direction = shapeB[0] - shapeA[0];
        if (direction.lengthSquared() < EPS_ABS * EPS_ABS) direction = Vector3(1, 0, 0);// 两点重叠，任选一非零向量
        // 2.获取Minkowski差的支持点
        std::vector<Vertex> simplex;
        simplex.push_back(supportMinkowski(shapeA, shapeB, direction));
		// 3.更新方向指向原点
		direction = simplex[0].point * (-1);
        while (true) {
            Vertex newPoint = supportMinkowski(shapeA, shapeB, direction);
			if (newPoint.point.dot(direction) < 0) return false; // 没有越过原点，说明不相交
			simplex.push_back(newPoint);
            if (updateSimplex2D(simplex, direction)) return true; 
        }
    }

	// 单纯形进化（3D）
	bool GJK::updateSimplex3D(std::vector<Vertex>& simplex, Vector3& direction) {
        // 3D单纯形可能是点、线段、三角形或四面体
        if (simplex.size() == 2)
        {
            // 线段情况
            //Point3& A = simplex[1]; // 最新添加的点
            //Point3& B = simplex[0];
            //Vector3 AB = B - A;
            //Vector3 AO = A * (-1);
   //         if (AB.dot(AO) <= 0) {
            //	// 最近点为 A，单纯形缩减为 {A}，方向 -A
            //	simplex.erase(simplex.begin());
            //	direction = AO; // 方向指向原点
   //         }

            //else if (AB.dot(AO) >= AB.dot(AB)) {
            //	// 最近点为 B，单纯形缩减为 {B}，方向 -B
            //	simplex.erase(simplex.begin() + 1);
            //	direction = (B * (-1)); // 方向指向原点
            //}

   //         else {
   //             // 原点在AB线段上，方向垂直于AB指向原点
   //             direction = AB.cross(AO).cross(AB);
   //         }
            direction = (simplex[1].point - simplex[0].point).cross(simplex[1].point).cross(simplex[0].point - simplex[1].point);
			if (direction.lengthSquared() < EPS_ABS * EPS_ABS) return true; // 0向量，说明原点在线段上，视为相交
        }
        else if (simplex.size() == 3)
        {
			// 三角形情况
            Point3& A = simplex[2].point; // 最新添加的点
            Point3& B = simplex[1].point;
            Point3& C = simplex[0].point;
            Vector3 BA = A - B;
            Vector3 CA = A - C;
            Vector3 ABCNormal = BA.cross(CA);
            Vector3 AO = A * (-1);
            Vector3 newDirection;
            if ((newDirection = ABCNormal.cross(BA)).dot(AO) > 0)
            {
				if (newDirection.lengthSquared() < EPS_ABS * EPS_ABS) return true;  // 0向量，说明原点在BA边上，视为相交
                // 原点在BA外侧
                direction = newDirection;
                // 移除点C
                simplex.erase(simplex.begin());
            }
            else if ((newDirection = CA.cross(ABCNormal)).dot(AO) > 0) {
				if (newDirection.lengthSquared() < EPS_ABS * EPS_ABS) return true; // 0向量，说明原点在CA边上，视为相交
                // 原点在CA外侧
                direction = newDirection;
                // 移除点B
                simplex.erase(simplex.begin() + 1);
            }
        }

		else if (simplex.size() == 4)
		{
			// 四面体情况
			Point3& A = simplex[3].point; // 最新添加的点
			Point3& B = simplex[2].point;
			Point3& C = simplex[1].point;
			Point3& D = simplex[0].point;
			Vector3 AO = A * (-1);
			Vector3 AB = B - A;
			Vector3 AC = C - A;
			Vector3 AD = D - A;
			Vector3 ABCNormal = AB.cross(AC);
			Vector3 ACDNormal = AC.cross(AD);
			Vector3 ADBNormal = AD.cross(AB);
			if (ABCNormal.dot(AO) > 0) {
				// 原点在ABC外侧
				direction = ABCNormal;
				simplex.erase(simplex.begin());
			}
			else if (ACDNormal.dot(AO) > 0) {
				// 原点在ACD外侧
				direction = ACDNormal;
				simplex.erase(simplex.begin() + 1);
			}
			else if (ADBNormal.dot(AO) > 0) {
				// 原点在ADB外侧
				direction = ADBNormal;
				simplex.erase(simplex.begin() + 2);
			}
			else {
				// 原点在四面体内部，说明相交
				return true;
			}
		}

        return false; // 需要继续迭代
	}

	// 3D版本：判断两个凸多面体是否相交
	bool GJK::intersect3D(const std::vector<Point3>& shapeA, const std::vector<Point3>& shapeB, std::vector<Vertex>& simplex) {
        // 1.初始方向可以任意选择，这里选择从shapeA的第一个顶点指向shapeB的第一个顶点
        Vector3 direction = shapeB[0] - shapeA[0];
        if (direction.lengthSquared() < EPS_ABS * EPS_ABS) direction = Vector3(1, 0, 0);// 两点重叠，任选一非零向量
        // 2.获取Minkowski差的支持点
        simplex.clear();
        simplex.push_back(supportMinkowski(shapeA, shapeB, direction));
        // 3.更新方向指向原点
        direction = simplex[0].point * (-1);
        while (true) {
            Vertex newPoint = supportMinkowski(shapeA, shapeB, direction);
            if (newPoint.point.dot(direction) < 0) return false; // 没有越过原点，说明不相交
            simplex.push_back(newPoint);
            if (updateSimplex3D(simplex, direction)) return true;
        }
	}

	// 如果相交，单纯形扩展到四面体
	void GJK::expandToTetrahedron(std::vector<Vertex>& simplex,
		const std::vector<Point3>& shapeA,
		const std::vector<Point3>& shapeB) {
        if (simplex.size() == 4 || simplex.size() == 1) return;
        if (simplex.size() == 2) {
            Point3 C = simplex[0].point;
			Point3 B = simplex[1].point;
			Vector3 BC = C - B;
            Vertex A = supportMinkowski(shapeA, shapeB, Vector3(1, 0, 0));
			// 判断A是否在BC线段上，如果是，选择一个垂直于BC的方向
			if ((A.point - B).cross(BC).lengthSquared() < EPS_ABS * EPS_ABS) {
				// A在BC线段上
				A = supportMinkowski(shapeA, shapeB, Vector3(0, 1, 0)); 
                if ((A.point - B).cross(BC).lengthSquared() >= EPS_ABS * EPS_ABS) simplex.push_back(A);
            }
            else {
                simplex.push_back(A);
            }
            
		}
        if (simplex.size() == 2) return; // 闵可夫斯基差集只有两个点或者是一条线段，无法继续扩展

        Point3 D = simplex[0].point;
        Point3 C = simplex[1].point;
        Point3 B = simplex[2].point;
        Vector3 DC = D - C;
        Vector3 DB = D - B;
        Vector3 normal = DC.cross(DB);
        // 候选方向列表
        std::vector<Vector3> dirs;
        dirs.push_back(DC.cross(DB)); // 面BCD的法线
        dirs.push_back(Vector3(0, 0, 1));
        dirs.push_back(Vector3(1, 0, 0));
        dirs.push_back(Vector3(0, 1, 0));
        dirs.push_back(Vector3(0, 0, -1));
        dirs.push_back(Vector3(-1, 0, 0));
        dirs.push_back(Vector3(0, -1, 0));

        for (const Vector3& dir : dirs) {
            Vertex A = supportMinkowski(shapeA, shapeB, dir);
            // 判断A是否在BCD所在平面上，如果是，选择下一个方向
            if ((A.point - B).dot(normal) < EPS_ABS) {
                // A在BCD所在平面上，继续尝试下一个方向
                continue;
            }

            else {
                // 找到一个不在BCD上的点A，加入单纯形
                simplex.push_back(A);
                break;
            }
        }

	}

    // 计算两个相交凸体的穿透深度与分离法线
    bool GJK::computePenetration(const std::vector<Point3>& shapeA,
        const std::vector<Point3>& shapeB,
        std::vector<Vertex>& simplex,
        PenetrationInfo& info) {
        if (simplex.size() < 4) return false;
        const int MAX_ITER = 64;
        std::vector<std::array<int, 3>> faces;
        std::vector<Vector3> normals;
        // 1、初始化
        //auto addFace = [&](int a, int b, int c) {
        //    Vector3 AB = simplex[b].point - simplex[a].point;
        //    Vector3 BC = simplex[c].point - simplex[b].point;
        //    Vector3 n = AB.cross(BC);
        //    if (n.lengthSquared() < EPS_ABS * EPS_ABS) return; // 退化三角形，忽略
        //    n = n.normalized();
        //    // 确保法线指向外部：原点在四面体内部，面上一点 P，应有 dot(n, P) > 0
        //    if (n.dot(simplex[a].point) < 0.0) {
        //        n = n * (-1);
        //        std::swap(b, c); // 保持顶点绕序一致
        //    }
        //    faces.push_back({ a, b, c });
        //    normals.push_back(n);
        //};

        Point3 centroid = (simplex[0].point + simplex[1].point + simplex[2].point + simplex[3].point) * 0.25;

        auto addFace = [&](int a, int b, int c) {
            Vector3 AB = simplex[b].point - simplex[a].point;
            Vector3 BC = simplex[c].point - simplex[b].point;
            Vector3 n = AB.cross(BC);
            if (n.lengthSquared() < EPS_ABS * EPS_ABS) return;
            n = n.normalized();

            // 判断法线是否朝外
            double sign = n.dot(simplex[a].point);
            if (std::fabs(sign) < EPS_ABS) {
                // 原点在面上，用质心辅助判断
                sign = n.dot(simplex[a].point - centroid);
                // sign < 0 说明法线指向质心（朝内），需要翻转
                if (sign < 0.0) {
                    n = n * (-1);
                    std::swap(b, c);
                }
            }
            else if (sign < 0.0) {
                // 原点在负侧，法线朝内，翻转
                n = n * (-1);
                std::swap(b, c);
            }
            // 否则 sign > 0，法线已经朝外，不处理

            faces.push_back({ a, b, c });
            normals.push_back(n);
            };

        addFace(0, 1, 2);
        addFace(0, 1, 3);
        addFace(0, 2, 3);
        addFace(1, 2, 3);

        // 2、主循环
        for (int iter = 0; iter  < MAX_ITER; ++iter)
        {
            // 2.1找最近面
            int closet = -1;
            double minDist = std::numeric_limits<double>::max();
            for (int i = 0; i < faces.size(); ++i)
            {
                double dist = simplex[faces[i][0]].point.dot(normals[i]);
                if (dist < minDist) {
                    minDist = dist;
                    closet = i;
                }
            }
            std::array<int, 3>& face = faces[closet];
            Vector3& n = normals[closet];
            // 2.2找新的支撑点
            Vertex newVert = supportMinkowski(shapeA, shapeB, n);

            //if (minDist < EPS_ABS) {
            //    for (Vertex& v : simplex) {
            //        //if (v.point == Point3(0, 0, 0)) {
            //        //    info.depth = minDist;
            //        //    info.normal = n;
            //        //    info.contactPointA = simplex[face[0]].pointA;
            //        //    info.contactPointB = simplex[face[0]].pointB;
            //        //    return true;
            //        //}
            //        if (v.point == newVert.point) {
            //            Vertex newVert2 = supportMinkowski(shapeA, shapeB, n * (-1));
            //            v.point = newVert2.point;
            //            v.pointA = newVert2.pointA;
            //            v.pointB = newVert2.pointB;
            //            // 重新调整面
            //            std::vector<std::array<int, 3>> newFaces;
            //            std::vector<Vector3> newNormals;
            //            faces.swap(newFaces);
            //            normals.swap(newNormals);
            //            for (int i = 0; i < newFaces.size(); ++i)
            //            {
            //                addFace(newFaces[i][0], newFaces[i][1], newFaces[i][2]);
            //            }
            //            break;
            //        }
            //    }
            //}

            // 2.3计算新点距离最近面的距离
            double newDist = newVert.point.dot(n); // 新点在法线的投影
            if (newDist - minDist < EPS_ABS)  // 没有明显扩展 → 收敛
            {
                // 2.3.1找到原点在最近面（三角形）的投影点
                const Triangle3 tri(simplex[face[0]].point, simplex[face[1]].point, simplex[face[2]].point);
                const Ray ray(Point3(0, 0, 0), n);
                double u, v, w;
                if (!tri.intersect(ray, u, v, w, false)) continue;
                // 2.3.2插值接触点
                info.depth = minDist;
                info.normal = n;
                info.contactPointA = simplex[face[0]].pointA * u + simplex[face[1]].pointA * v + simplex[face[2]].pointA * w;
                info.contactPointB = simplex[face[0]].pointB * u + simplex[face[1]].pointB * v + simplex[face[2]].pointB * w;
                return true;
            }
            // 2.4有明显扩展，扩展多面体
            // 2.4.1收集可见面
            std::vector<int> visibleFaces;
            for (int i = 0; i < faces.size(); ++i)
            {
                Plane plane(normals[i], simplex[faces[i][0]].point);
                // 新点到该平面的有向距离
                if (plane.signedDistance(newVert.point) > EPS_ABS)
                {
                    visibleFaces.push_back(i);
                }
            }
            // 2.4.2提取边界边
            struct Edge { int a, b; };
            std::vector<Edge> boundaryEdges;
            for (int idx : visibleFaces)
            {
                auto& face = faces[idx];
                for (int i = 0; i < 3; ++i) {
                    int v0 = face[i];
                    int v1 = face[(i + 1) % 3];
                    int count = 0;
                    // 统计v0v1在可见面出现次数
                    for (int vf : visibleFaces) {
                        auto& of = faces[vf];
                        if ((v0 == of[0] || v0 == of[1] || v0 == of[2])
                            && (v1 == of[0] || v1 == of[1] || v1 == of[2])) ++count;
                    }
                    if (count == 1) boundaryEdges.push_back({ v0,v1 }); // 只被一个可见面使用的边
                }
            }

            // 2.4.3添加新顶点
            int newIdx = simplex.size();
            simplex.push_back(newVert);
            // 2.4.4删除所有可见面
            std::vector<std::array<int, 3>> newFaces;
            std::vector<Vector3> newNormals;
            for (int i = 0; i < faces.size(); ++i)
            {
                bool visiable = false;
                for (int idx : visibleFaces) {
                    if (idx == i) {
                        visiable = true;
                        break;
                    }
                }
                if (!visiable) {
                    newFaces.push_back(faces[i]);
                    newNormals.push_back(normals[i]);
                }
            }

            faces.swap(newFaces);
            normals.swap(newNormals);

            // 2.4.5用边界边和新顶点构造新三角形
            for (const Edge& e : boundaryEdges) {
                addFace(newIdx, e.a, e.b);
            }
        }
        return false; // 超过最大迭代仍未收敛
    }

    //bool GJK::computePenetration(const std::vector<Point3>& shapeA,
    //    const std::vector<Point3>& shapeB,
    //    const std::vector<Point3>& simplex,
    //    PenetrationInfo& info) {
    //    const double EPS = 1e-8;
    //    const int MAX_ITER = 64;

    //    // ---- 初始化多面体（要求 simplex 有 4 个点）----
    //    if (simplex.size() != 4) return false;

    //    Polytope poly;
    //    // 注意：传入的 simplex 只有 Minkowski 差点，没有原始 A/B。
    //    // 因此这里暂时将 A 设为差点的近似（不准确，但后续迭代产生的顶点会携带正确信息）。
    //    for (const Point3& p : simplex) {
    //        poly.vertices.push_back({ p, p, Point3{0,0,0} }); // 占位
    //    }
    //    // 构建 4 个面（顶点索引 0-3）
    //    // 面的法线必须朝外（远离原点），即对于面上一点 P，dot(N, P) > 0
    //    int idx[4][3] = { {0,1,2}, {0,3,1}, {0,2,3}, {1,3,2} };
    //    for (int i = 0; i < 4; ++i) {
    //        Vertex* a = &poly.vertices[idx[i][0]];
    //        Vertex* b = &poly.vertices[idx[i][1]];
    //        Vertex* c = &poly.vertices[idx[i][2]];
    //        Vector3 normal = cross(b->point - a->point, c->point - a->point);
    //        // 使法线指向外部（即 normal 与 (a->point - 原点) 点积 > 0）
    //        if (dot(normal, a->point) < 0) {
    //            normal = -normal;
    //            std::swap(b, c);
    //        }
    //        poly.faces.push_back({ {a, b, c}, normal });
    //    }

    //    // ---- EPA 主循环 ----
    //    for (int iter = 0; iter < MAX_ITER; ++iter) {
    //        // 1. 寻找离原点最近的面
    //        int closestIdx = -1;
    //        double minDist = DBL_MAX;
    //        for (int i = 0; i < poly.faces.size(); ++i) {
    //            double dist = fabs(dot(poly.faces[i].normal, poly.faces[i].v[0]->point));
    //            if (dist < minDist) {
    //                minDist = dist;
    //                closestIdx = i;
    //            }
    //        }
    //        Triangle& closestFace = poly.faces[closestIdx];

    //        // 2. 计算新支撑点（沿最近面法线）
    //        Vertex newVert = support(shapeA, shapeB, closestFace.normal);

    //        // 3. 检查新点是否已经靠近边界（收敛）
    //        double newDist = fabs(dot(closestFace.normal, newVert.point));
    //        if (newDist - minDist < EPS) {
    //            // 收敛 —— 计算穿透信息
    //            info.depth = minDist;
    //            info.normal = closestFace.normal;
    //            // 找到最近面上的精确最近点（原点投影到面上）
    //            // 用重心坐标求最近点 P，然后插值 A、B
    //            Point3 p0 = closestFace.v[0]->point;
    //            Point3 p1 = closestFace.v[1]->point;
    //            Point3 p2 = closestFace.v[2]->point;
    //            // 解三角形平面上的最近点（简单的重心坐标法）
    //            Vector3 N = closestFace.normal;
    //            // 原点投影到平面：O_proj = -dot(N, p0) * N  （因为 O 在原点）
    //            double d = dot(N, p0);
    //            Point3 proj = N * d;  // 原点沿法线投影到平面上
    //            // 计算重心坐标 (限于三角形内)
    //            Vector3 v0 = p1 - p0, v1 = p2 - p0, vp = proj - p0;
    //            double d00 = dot(v0, v0), d01 = dot(v0, v1), d11 = dot(v1, v1);
    //            double d20 = dot(vp, v0), d21 = dot(vp, v1);
    //            double denom = d00 * d11 - d01 * d01;
    //            double beta = (d11 * d20 - d01 * d21) / denom;
    //            double gamma = (d00 * d21 - d01 * d20) / denom;
    //            double alpha = 1.0 - beta - gamma;
    //            // 钳制到三角形内
    //            alpha = std::max(0.0, std::min(1.0, alpha));
    //            beta = std::max(0.0, std::min(1.0, beta));
    //            gamma = 1.0 - alpha - beta;
    //            // 插值接触点
    //            info.contactPointA = closestFace.v[0]->pointA * alpha +
    //                closestFace.v[1]->pointA * beta +
    //                closestFace.v[2]->pointA * gamma;
    //            info.contactPointB = closestFace.v[0]->pointB * alpha +
    //                closestFace.v[1]->pointB * beta +
    //                closestFace.v[2]->pointB * gamma;
    //            return true;
    //        }

    //        // 4. 收集所有可见面（新点在法线正侧）
    //        std::vector<Triangle*> visibleFaces;
    //        for (Triangle& f : poly.faces) {
    //            if (dot(f.normal, newVert.point - f.v[0]->point) > EPS) {
    //                visibleFaces.push_back(&f);
    //            }
    //        }

    //        // 5. 提取边界边（只被一个可见面使用的边）
    //        std::vector<std::pair<Vertex*, Vertex*>> boundaryEdges;
    //        for (Triangle* f : visibleFaces) {
    //            for (int k = 0; k < 3; ++k) {
    //                Vertex* a = f->v[k];
    //                Vertex* b = f->v[(k + 1) % 3];
    //                // 统计这条边在可见面集合中出现的次数
    //                int count = 0;
    //                for (Triangle* of : visibleFaces) {
    //                    if ((of->v[0] == a || of->v[1] == a || of->v[2] == a) &&
    //                        (of->v[0] == b || of->v[1] == b || of->v[2] == b))
    //                        count++;
    //                }
    //                if (count == 1) { // 边界边
    //                    boundaryEdges.push_back({ a, b });
    //                }
    //            }
    //        }

    //        // 6. 删除所有可见面
    //        poly.vertices.push_back(newVert);  // 先加入新顶点
    //        std::vector<Triangle> newFaces;
    //        for (Triangle& f : poly.faces) {
    //            bool vis = false;
    //            for (Triangle* vf : visibleFaces) if (&f == vf) { vis = true; break; }
    //            if (!vis) newFaces.push_back(f);
    //        }
    //        poly.faces.swap(newFaces);

    //        // 7. 用边界边和新顶点构造新三角形
    //        for (auto& edge : boundaryEdges) {
    //            Vertex* a = edge.first;
    //            Vertex* b = edge.second;
    //            Vertex* c = &poly.vertices.back();
    //            Vector3 normal = cross(b->point - a->point, c->point - a->point);
    //            // 调整法线指向外部（远离原点）
    //            if (dot(normal, a->point) < 0) {
    //                normal = -normal;
    //                std::swap(a, b);
    //            }
    //            poly.faces.push_back({ {a, b, c}, normal });
    //        }
    //    }

    //    return false; // 未收敛
    //}