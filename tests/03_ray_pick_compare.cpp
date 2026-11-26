//#include<gtest/gtest.h>
//#include <Geom_Line.hxx>
//#include <GeomAPI_IntCS.hxx>
//#include <BRepAdaptor_Surface.hxx>
//#include <BRepClass_FaceClassifier.hxx>
//#include <TopExp_Explorer.hxx>
//#include <TopoDS.hxx>
//#include <TopoDS_Face.hxx>
//#include <TopoDS_Shape.hxx>
//#include <TopoDS_Solid.hxx>
//#include <BRepPrimAPI_MakeBox.hxx>
//#include <gp_Pnt.hxx>
//#include <gp_Dir.hxx>
//#include <gp_Ax1.hxx>
//#include <Geom_Curve.hxx>
//#include <Geom_Surface.hxx>
//#include <Precision.hxx>
//#include <vector>
//#include <algorithm>
//#include <BRepMesh_IncrementalMesh.hxx>
//#include <BRepAlgoAPI_Cut.hxx>
//#include <BRepPrimAPI_MakeSphere.hxx>
//#include <chrono>
//#include "../include/GeoBase/BVH.h"
//
//
//// 射线与实体求交，返回所有交点（世界坐标）
//std::vector<gp_Pnt> raySolidIntersections(const gp_Pnt& origin,
//	                                      const gp_Dir& direction,
//	                                      TopoDS_Solid& solid) {
//	std::vector<gp_Pnt> hitPoints;
//	// 创建射线
//	Handle(Geom_Line) rayLine = new Geom_Line(gp_Ax1(origin, direction));
//
//	// 遍历实体的所有面
//	for (TopExp_Explorer faceExp(solid, TopAbs_FACE); faceExp.More(); faceExp.Next())
//	{
//		const TopoDS_Face& face = TopoDS::Face(faceExp.Current());
//		// 获取面的几何曲面
//		BRepAdaptor_Surface surfaceAdaptor(face, Standard_True);
//		const Handle(Geom_Surface)& surface = surfaceAdaptor.Surface().Surface();
//		// 计算射线与曲面的交点
//		GeomAPI_IntCS intersector(rayLine, surface);
//		//intersector.Perform(rayLine, surface);
//		if (!intersector.IsDone() || intersector.NbPoints() == 0)
//			continue;
//
//		// 准备面上的分类器（判断点是否在面边界内）
//		BRepClass_FaceClassifier classifier;
//		// 容差取标准精度
//		const Standard_Real tol = Precision::Confusion();
//
//		for (Standard_Integer i = 1; i <= intersector.NbPoints(); ++i)
//		{
//			// 交点参数和坐标
//			Standard_Real u, v, w;
//			intersector.Parameters(i, u, v, w);
//			gp_Pnt pt = intersector.Point(i);
//			// 只取射线正方向（w >= 0）
//			if (w < -tol) continue;
//
//			// 将3D点转换为面上的2D参数 (u,v)
//			// GeomAPI_IntCS 返回的 u,v 就是曲面参数
//			// 分类器检查该 (u,v) 是否在面的内部或边界上
//			// 把uv封装为二维参数点
//			gp_Pnt2d paramPoint(u, v);
//			classifier.Perform(face, paramPoint, tol);
//			TopAbs_State state = classifier.State();
//			if (state == TopAbs_IN || state == TopAbs_ON)
//			{
//				// 去重检查（基于距离容差）
//				Standard_Boolean isDuplicate = Standard_False;
//				for (const auto& hitPoint : hitPoints)
//				{
//					if (pt.Distance(hitPoint) < tol)
//					{
//						isDuplicate = Standard_True;
//						break;
//					}
//				}
//				if (!isDuplicate)
//					hitPoints.emplace_back(pt);
//			}
//
//		}
//		// 按射线参数排序
//        // 但由于我们没有存储参数w，可以按沿射线方向的距离排序
//		std::sort(hitPoints.begin(), hitPoints.end(),
//			[&origin, &direction](const gp_Pnt& a, const gp_Pnt& b) {
//				Standard_Real ta = gp_Vec(origin, a).Dot(direction);
//				Standard_Real tb = gp_Vec(origin, b).Dot(direction);
//				return ta < tb;
//			});
//
//	}
//
//	return hitPoints;
//
//}
//
//
//// 一条射线和立方体求交
//TEST(ray_pick_compareTest, ray_pick_compareTest1) {
//	// 创建一个单位立方体
//	TopoDS_Solid box = BRepPrimAPI_MakeBox(gp_Pnt(-1, -1, -1), gp_Pnt(1, 1, 1)).Solid();
//	// 射线：从 (-2, 0, 0) 指向 (1, 0, 0)
//	gp_Pnt rayOrigin(-2, 0, 0);
//	gp_Dir rayDir(1, 0, 0);
//	std::vector<gp_Pnt> hits = raySolidIntersections(rayOrigin, rayDir, box);
//
//	std::cout << "Number of intersections: " << hits.size() << std::endl;
//	for (size_t i = 0; i < hits.size(); ++i)
//	{
//		std::cout << "Point " << i + 1 << ": ("
//			<< hits[i].X() << ", "
//			<< hits[i].Y() << ", "
//			<< hits[i].Z() << ")" << std::endl;
//	}
//}
//
//
//
//
//
//// 从OCC的TopoDS_Shape中提取三角形
//std::vector<Triangle3> extractTrianglesFromShape(const TopoDS_Shape& shape) {
//    std::vector<Triangle3> triangles;
//
//    // 先对形状进行网格化
//    BRepMesh_IncrementalMesh mesh(shape, 0.1);
//
//    // 遍历所有面
//    for (TopExp_Explorer exp(shape, TopAbs_FACE); exp.More(); exp.Next()) {
//        TopoDS_Face face = TopoDS::Face(exp.Current());
//        TopLoc_Location loc;
//        Handle(Poly_Triangulation) tri = BRep_Tool::Triangulation(face, loc);
//
//        if (tri.IsNull()) continue;
//
//        // 获取面的变换矩阵
//        gp_Trsf transform = loc.Transformation();
//
//        // 遍历三角形
//        for (int i = 1; i <= tri->NbTriangles(); i++) {
//            Poly_Triangle triangle = tri->Triangle(i);
//
//            // 获取三角形顶点索引
//            int idx1, idx2, idx3;
//            triangle.Get(idx1, idx2, idx3);
//
//            // 获取顶点坐标并应用变换
//            gp_Pnt p1 = tri->Node(idx1).Transformed(transform);
//            gp_Pnt p2 = tri->Node(idx2).Transformed(transform);
//            gp_Pnt p3 = tri->Node(idx3).Transformed(transform);
//
//            // 转换为GeoBase的Point3
//            Point3 v1(p1.X(), p1.Y(), p1.Z());
//            Point3 v2(p2.X(), p2.Y(), p2.Z());
//            Point3 v3(p3.X(), p3.Y(), p3.Z());
//
//            triangles.emplace_back(v1, v2, v3);
//        }
//    }
//
//    return triangles;
//}
//
//// 生成随机射线
//std::vector<Ray> generateRandomRays(int count, const AABB& bounds) {
//    std::vector<Ray> rays;
//    std::random_device rd;
//    std::mt19937 gen(rd());
//    std::uniform_real_distribution<double> dis(-1.0, 1.0);
//
//    // 在包围盒外生成射线起点，方向指向包围盒中心
//    Point3 center = bounds.getCenter();
//    Vector3 extent = bounds.getExtent();
//    double maxExtent = std::max({ extent.x, extent.y, extent.z }) * 3.0;
//
//    for (int i = 0; i < count; i++) {
//        // 在包围盒外随机生成起点
//        Point3 origin(
//            center.x + (dis(gen) * maxExtent),
//            center.y + (dis(gen) * maxExtent),
//            center.z + (dis(gen) * maxExtent)
//        );
//
//        // 方向指向包围盒中心附近
//        //Vector3 direction(
//        //    center.x - origin.x + dis(gen) * extent.x,
//        //    center.y - origin.y + dis(gen) * extent.y,
//        //    center.z - origin.z + dis(gen) * extent.z
//        //);
//        Vector3 direction(
//            center.x - origin.x + dis(gen) * extent.x,
//            center.y - origin.y + dis(gen) * extent.y,
//            center.z - origin.z + dis(gen) * extent.z
//        );
//        direction = direction.normalized();
//
//        rays.emplace_back(origin, direction);
//    }
//
//    return rays;
//}
//
//// 同一条射线，分别用OCC直接求交和用GeoBase的BVH求交（对网格化的三角形）
//TEST(ray_pick_compareTest, ray_pick_compareTest2) {
//    std::cout << "===== GeoBase vs OCC Intersection Performance Comparison =====\n\n";
//
//    // 1. 用OCC创建测试几何体（正方体减去球体，产生复杂曲面）
//    std::cout << "Creating test geometry...\n";
//    gp_Pnt origin(0.0, 0.0, 0.0);
//    BRepPrimAPI_MakeBox boxMaker(origin, 10.0, 10.0, 10.0);
//    TopoDS_Shape box = boxMaker.Shape();
//
//    gp_Pnt sphereCenter(5.0, 5.0, 5.0);
//    BRepPrimAPI_MakeSphere sphereMaker(sphereCenter, 6.0);
//    TopoDS_Shape sphere = sphereMaker.Shape();
//
//    BRepAlgoAPI_Cut cutMaker(box, sphere);
//    TopoDS_Shape resultShape = cutMaker.Shape();
//
//    // 2. 从OCC形状提取三角形，构建GeoBase的BVH
//    std::cout << "Extracting triangles and building BVH...\n";
//    auto triangles = extractTrianglesFromShape(resultShape);
//    std::cout << "Triangle count: " << triangles.size() << "\n\n";
//
//	BVHNode* bvh = buildBVH(triangles, 0, triangles.size(), 16);
//
//    // 3. 生成测试射线
//    AABB bounds = triangles[0].getBoundingBox();
//    for (const auto& tri : triangles) {
//        bounds.merge(tri.getBoundingBox());
//    }
//    auto rays = generateRandomRays(1000, bounds);
//
//    // 4. GeoBase BVH求交测试
//    std::cout << "GeoBase BVH intersection test...\n";
//    int geoBaseHits = 0;
//    auto start = std::chrono::high_resolution_clock::now();
//
//    for (const auto& ray : rays) {
//        HitInfo hit;
//        if (bvh->intersect(ray, hit, triangles)) {
//            geoBaseHits++;
//        }
//    }
//
//    auto end = std::chrono::high_resolution_clock::now();
//    auto geoBaseDuration = duration_cast<std::chrono::microseconds>(end - start).count();
//    double geoBaseAvgTime = (double)geoBaseDuration / rays.size();
//
//    std::cout << "Hits: " << geoBaseHits << "/" << rays.size() << "\n";
//    std::cout << "Total time: " << geoBaseDuration << " us\n";
//    std::cout << "Avg time: " << geoBaseAvgTime << " us\n\n";
//
//    // 暴力遍历测试
//    std::cout << "Brute force intersection test...\n";
//    int bruteHits = 0;
//    start = std::chrono::high_resolution_clock::now();
//    for (const auto& ray : rays) {
//        for (const auto& tri : triangles) {
//            double t, u, v;
//            if (tri.intersect(ray, t, u, v, false)) {
//                if (t > 0) {  // 只统计正向命中
//                    bruteHits++;
//                    break;  // 找到第一个就跳出
//                }
//            }
//        }
//    }
//    end = std::chrono::high_resolution_clock::now();
//    auto bruteDuration = duration_cast<std::chrono::microseconds>(end - start).count();
//    double bruteAvgTime = (double)bruteDuration / rays.size();
//
//    std::cout << "Hits: " << bruteHits << "/" << rays.size() << "\n";
//    std::cout << "Total time: " << bruteDuration << " us\n";
//    std::cout << "Brute force hits: " << bruteAvgTime << "/" << rays.size() << "\n";
//
//    // 5. OCC解析求交测试
//    std::cout << "OCC analytic intersection test...\n";
//    int occHits = 0;
//    start = std::chrono::high_resolution_clock::now();
//
//    for (const auto& ray : rays) {
//        // 构建OCC射线
//        gp_Pnt rayOrigin(ray.origin.x, ray.origin.y, ray.origin.z);
//        gp_Dir rayDir(ray.direction.x, ray.direction.y, ray.direction.z);
//        gp_Lin occLine(rayOrigin, rayDir);
//        Handle(Geom_Line) geomLine = new Geom_Line(occLine);
//
//        // 对每个面进行求交
//        bool hasHit = false;
//        double minDist = DBL_MAX;
//
//        for (TopExp_Explorer exp(resultShape, TopAbs_FACE); exp.More(); exp.Next()) {
//            TopoDS_Face face = TopoDS::Face(exp.Current());
//            BRepAdaptor_Surface surface(face);
//
//            GeomAPI_IntCS intersection(geomLine, surface.Surface().Surface());
//
//            if (intersection.IsDone() && intersection.NbPoints() > 0) {
//                for (int i = 1; i <= intersection.NbPoints(); i++) {
//                    gp_Pnt intPoint = intersection.Point(i);
//                    double dist = rayOrigin.Distance(intPoint);
//
//                    // 检查交点是否在射线前方
//                    gp_Vec vecToPoint(rayOrigin, intPoint);
//                    if (vecToPoint.Dot(rayDir) > 0 && dist < minDist) {
//                        minDist = dist;
//                        hasHit = true;
//                    }
//                }
//            }
//        }
//
//        if (hasHit) occHits++;
//    }
//
//    end = std::chrono::high_resolution_clock::now();
//    auto occDuration = duration_cast<std::chrono::microseconds>(end - start).count();
//    double occAvgTime = (double)occDuration / rays.size();
//
//    std::cout << "Hits: " << occHits << "/" << rays.size() << "\n";
//    std::cout << "Total time: " << occDuration << " us\n";
//    std::cout << "Avg time: " << occAvgTime << " us\n\n";
//
//    // 6. 性能对比总结
//    std::cout << "===== Performance Comparison Summary =====\n";
//    std::cout << "Triangle count: " << triangles.size() << "\n";
//    std::cout << "Rays tested: " << rays.size() << "\n\n";
//
//    std::cout << "GeoBase BVH:\n";
//    std::cout << "  Total: " << geoBaseDuration << " us\n";
//    std::cout << "  Avg: " << geoBaseAvgTime << " us/ray\n";
//    std::cout << "  Hit rate: " << (geoBaseHits * 100.0 / rays.size()) << "%\n\n";
//
//    std::cout << "OCC Analytic:\n";
//    std::cout << "  Total: " << occDuration << " us\n";
//    std::cout << "  Avg: " << occAvgTime << " us/ray\n";
//    std::cout << "  Hit rate: " << (occHits * 100.0 / rays.size()) << "%\n\n";
//
//    double speedup = (double)occDuration / geoBaseDuration;
//    std::cout << "Speedup: GeoBase BVH is " << speedup << "x faster than OCC analytic intersection\n";
//}