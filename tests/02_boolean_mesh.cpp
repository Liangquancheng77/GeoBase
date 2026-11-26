//#include<gtest/gtest.h>
//#include <iostream>
//#include <iomanip>
//#include <BRepPrimAPI_MakeBox.hxx>
//#include <BRepPrimAPI_MakeSphere.hxx>
//#include <BRepAlgoAPI_Fuse.hxx>      // 并集
//#include <BRepAlgoAPI_Common.hxx>    // 交集
//#include <BRepAlgoAPI_Cut.hxx>       // 差集
//#include <TopoDS_Shape.hxx>
//#include <TopoDS_Solid.hxx>
//#include <BRepTools.hxx>
//#include <TopExp_Explorer.hxx>
//#include <TopoDS_Face.hxx>               // TopoDS::Face 需要此头文件
//#include <TopAbs.hxx>
//#include <BRepMesh_IncrementalMesh.hxx>
//#include <Poly_Triangulation.hxx>
//#include <BRep_Tool.hxx>
//#include <TopLoc_Location.hxx>
//#include <gp_Trsf.hxx>
//#include <gp_Pnt.hxx>
//#include <TopoDS.hxx>
//#include <GProp_GProps.hxx>
//#include <BRepGProp.hxx>
//#include <BRepExtrema_DistShapeShape.hxx>
//
//#include "../include/GeoBase/Triangle3.h"
//
//// 辅助函数：打印形状的实体元素数量
//void printShapeStats(const std::string& name, const TopoDS_Shape& shape)
//{
//	int nbVertices = 0, nbEdges = 0, nbFaces = 0;
//	for (TopExp_Explorer exp(shape, TopAbs_VERTEX); exp.More(); exp.Next()) ++nbVertices;
//	for (TopExp_Explorer exp(shape, TopAbs_EDGE); exp.More(); exp.Next()) ++nbEdges;
//	for (TopExp_Explorer exp(shape, TopAbs_FACE); exp.More(); exp.Next()) ++nbFaces;
//	std::cout << std::left << std::setw(24) << name
//		<< " V: " << std::setw(4) << nbVertices
//		<< " E: " << std::setw(4) << nbEdges
//		<< " F: " << std::setw(4) << nbFaces << std::endl;
//}
//
//// 布尔运算
//TEST(boolean_meshTest, boolean_meshTest1) {
//
//	// 1. 创建正方体：边长 2，中心在 (0,0,0)
//	TopoDS_Solid box = BRepPrimAPI_MakeBox(gp_Pnt(-1, -1, -1), gp_Pnt(1, 1, 1)).Solid();
//	// 2. 创建球体：半径 1.2，中心在 (0,0,0)
//	TopoDS_Shape sphere = BRepPrimAPI_MakeSphere(gp_Pnt(0, 0, 0), 1.2).Solid();
//
//	// 打印原始数据
//	std::cout << "----- Original Shapes -----\n";
//	printShapeStats("Box", box);
//	printShapeStats("Sphere", sphere);
//
//	// 3. 布尔并集 (Fuse / Union)
//	BRepAlgoAPI_Fuse fuseOp(box, sphere);
//	if (fuseOp.IsDone())
//	{
//		TopoDS_Shape fusedShape = fuseOp.Shape();
//		printShapeStats("Fuse (Box ∪ Sphere)", fusedShape);
//	}
//	else {
//		std::cout << "Fuse failed.\n";
//	}
//
//	// 4. 布尔交集 (Common / Intersection)
//	BRepAlgoAPI_Common commonOp(box, sphere);
//	if (commonOp.IsDone())
//	{
//		TopoDS_Shape commonShape = commonOp.Shape();
//		Standard_Boolean isEmpty = commonShape.IsNull();
//		GProp_GProps prop;
//		BRepGProp::VolumeProperties(commonShape, prop);
//		Standard_Real intersectVol = prop.Mass();
//		printShapeStats("Common (Box ∩ Sphere)", commonShape);
//	}
//	else {
//		std::cout << "Common failed.\n";
//	}
//
//	BRepExtrema_DistShapeShape dist(box, sphere);
//	if (dist.IsDone() && dist.Value() <= Precision::Confusion()) {
//		// 相交
//		std::cout << "------------";
//	}
//
//	// 5. 布尔差集 (Cut: box - sphere)
//	BRepAlgoAPI_Cut cutOp1(box, sphere);
//	if (cutOp1.IsDone())
//	{
//		TopoDS_Shape cutShape1 = cutOp1.Shape();
//		printShapeStats("Cut (Box - Sphere)", cutShape1);
//	}
//	else {
//		std::cout << "Cut (Box - Sphere) failed.\n";
//	}
//
//	// 6. 布尔差集 (Cut: sphere - box)
//	BRepAlgoAPI_Cut cutOp2(sphere, box);
//	if (cutOp2.IsDone())
//	{
//		TopoDS_Shape cutShape2 = cutOp2.Shape();
//		printShapeStats("Cut (Sphere - Box)", cutShape2);
//	}
//	else {
//		std::cout << "Cut (Sphere - Box) failed.\n";
//	}
//
//
//
//}
//
///**
// * 从 OCC 拓扑形状中提取三角形网格
// * @param shape      输入形状（必须是有效拓扑体）
// * @param triangles  输出三角形列表
// * @param deflection 网格化线性挠度（默认 0.1）
// * @param angle      网格化角度公差（弧度，默认 0.5）
// */
//void extractTriangles(const TopoDS_Shape& shape,
//	std::vector<Triangle3>& triangles,
//	double deflection = 0.1,
//	double angle = 0.5)
//{
//	// 合法性检查
//	if (shape.IsNull()) return;
//	triangles.clear();
//
//	// 1. 创建并执行网格剖分
//	BRepMesh_IncrementalMesh mesh(shape, deflection, Standard_False, angle);
//	mesh.Perform();                 // 真正执行网格化
//	if (!mesh.IsDone()) return;     // 剖分失败则返回
//
//	// 2. 遍历所有面
//	for (TopExp_Explorer exp(shape, TopAbs_FACE); exp.More(); exp.Next())
//	{
//		const TopoDS_Face& face = TopoDS::Face(exp.Current());
//
//		// 获取面的三角剖分数据及其局部变换
//		TopLoc_Location loc;
//		Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(face, loc);
//		if (triangulation.IsNull()) continue;
//
//		// 面的方向：若为反向则需要翻转三角形顶点顺序
//		Standard_Boolean isReversed = (face.Orientation() == TopAbs_REVERSED);
//
//		// 合成总的坐标变换：网格局部变换 × 面自身的位置变换
//		gp_Trsf faceTrsf = face.Location().Transformation();
//		gp_Trsf meshTrsf = loc.Transformation();
//		gp_Trsf totalTrsf = meshTrsf * faceTrsf;   // 注意乘法顺序
//
//		//const TColgp_Array1OfPnt& nodes = triangulation->Nodes();
//		//const auto& nodes = triangulation->InternalNodes();
//		//const Poly_Array1OfTriangle& triIndices = triangulation->Triangles();
//
//		for (Standard_Integer i = 1; i <= triangulation->NbTriangles(); ++i)
//		{
//			Standard_Integer idx1, idx2, idx3;
//			triangulation->Triangle(i).Get(idx1, idx2, idx3);
//
//			// 处理反向面：交换 idx1 和 idx3 保持法向一致
//			if (isReversed)
//				std::swap(idx1, idx3);
//
//			gp_Pnt p1 = triangulation->Node(idx1).Transformed(totalTrsf);
//			gp_Pnt p2 = triangulation->Node(idx2).Transformed(totalTrsf);
//			gp_Pnt p3 = triangulation->Node(idx3).Transformed(totalTrsf);
//
//			Point3 v1(p1.X(), p1.Y(), p1.Z());
//			Point3 v2(p2.X(), p2.Y(), p2.Z());
//			Point3 v3(p3.X(), p3.Y(), p3.Z());
//
//			triangles.emplace_back(v1, v2, v3);
//		}
//	}
//}
//
//// OCC网格化
//TEST(boolean_meshTest, boolean_meshTest2) {
//	// 创建正方体和球体
//	TopoDS_Solid box = BRepPrimAPI_MakeBox(
//		gp_Pnt(-1.0, -1.0, -1.0), gp_Pnt(1.0, 1.0, 1.0)).Solid();
//	TopoDS_Solid sphere = BRepPrimAPI_MakeSphere(
//		gp_Pnt(0.0, 0.0, 0.0), 1.2).Solid();
//
//	// 执行布尔并集
//	BRepAlgoAPI_Fuse fuse(box, sphere);
//	if (fuse.IsDone())
//	{
//		TopoDS_Shape result = fuse.Shape();
//		std::vector<Triangle3> triangles;
//		extractTriangles(result, triangles, 0.1);
//
//		std::cout << "Extracted " << triangles.size() << " triangles.\n";
//		if (!triangles.empty())
//		{
//			const Triangle3& tri = triangles[0];
//			std::cout << "First triangle: "
//				<< tri.v0.x << " " << tri.v0.y << " " << tri.v0.z << " ; "
//				<< tri.v1.x << " " << tri.v1.y << " " << tri.v1.z << " ; "
//				<< tri.v2.x << " " << tri.v2.y << " " << tri.v2.z << "\n";
//		}
//	}
//	else
//	{
//		std::cerr << "Boolean fuse failed.\n";
//	}
//}