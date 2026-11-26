//#include<gtest/gtest.h>
//#include <iostream>
//#include <iomanip>
//
//// 基础几何体生成
//#include <BRepPrimAPI_MakeBox.hxx>
//#include <BRepPrimAPI_MakeSphere.hxx>
//#include <BRepPrimAPI_MakeCylinder.hxx>
//#include <BRepPrimAPI_MakeCone.hxx>
//
//// 拓扑遍历
//#include <TopExp_Explorer.hxx>
//#include <TopAbs_ShapeEnum.hxx>
//
//// 几何属性：体积、表面积、重心
//#include <BRepGProp.hxx>
//#include <GProp_GProps.hxx>
//
//// STEP/IGES导出
//#include <STEPControl_Writer.hxx>
//#include <IGESControl_Writer.hxx>
//
//// 基础点、形状
//#include <gp_Pnt.hxx>
//#include <TopoDS_Shape.hxx>
//
//// 拓扑遍历打印顶点 / 边 / 面数量
//void PrintTopoInfo(const TopoDS_Shape& shape) {
//	int vertexCnt = 0, edgeCnt = 0, faceCnt = 0;
//	// 遍历顶点
//	for (TopExp_Explorer exp(shape, TopAbs_VERTEX); exp.More(); exp.Next())
//		++vertexCnt;
//	// 遍历边
//	for (TopExp_Explorer exp(shape, TopAbs_EDGE); exp.More(); exp.Next())
//		++edgeCnt;
//	// 遍历面
//	for (TopExp_Explorer exp(shape, TopAbs_FACE); exp.More(); exp.Next())
//		++faceCnt;
//
//	std::cout << "===== Topology Info =====\n";
//	std::cout << "Vertices: " << vertexCnt << "\n";
//	std::cout << "Edges:    " << edgeCnt << "\n";
//	std::cout << "Faces:    " << faceCnt << "\n";
//
//}
//
//// 计算并打印体积、表面积、重心
//void PrintGeoProp(const TopoDS_Shape& shape) {
//	GProp_GProps volProps;// 体积属性
//	GProp_GProps surfProps;// 表面积属性
//
//	BRepGProp::VolumeProperties(shape, volProps);
//	double vol = volProps.Mass();
//
//	BRepGProp::SurfaceProperties(shape, surfProps);
//	double area = surfProps.Mass();
//
//	gp_Pnt center = volProps.CentreOfMass();
//
//	std::cout << "===== Geometric Properties =====\n";
//	std::cout << std::fixed << std::setprecision(4);
//	std::cout << "Volume: " << vol << "\n";
//	std::cout << "Surface Area: " << area << "\n";
//	std::cout << "Center of Mass: (" << center.X() << ", " << center.Y() << ", " << center.Z() << ")\n";
//}
//
//// 导出 STEP 文件
//bool ExportSTEP(const TopoDS_Shape& shape, const std::string& filename) {
//	STEPControl_Writer writer;
//	IFSelect_ReturnStatus status = writer.Transfer(shape, STEPControl_AsIs);
//	if (status != IFSelect_RetDone) {
//		std::cerr << "Error transferring shape to STEP writer.\n";
//		return false;
//	}
//	status = writer.Write(filename.c_str());
//	if (status != IFSelect_RetDone) {
//		std::cerr << "Error writing STEP file: " << filename << "\n";
//		return false;
//	}
//	return true;
//}
//
//// 导出 IGES 文件
//bool ExportIGES(const TopoDS_Shape& shape, const std::string& filename) {
//	IGESControl_Writer writer;
//	writer.AddShape(shape);
//	return writer.Write(filename.c_str());
//}
//
//// 创建基本几何体、几何属性查询、几何数据导出
//TEST(basic_shapesTest, basic_shapesTest1) {
//	// 1.1 正方体 边长10
//	TopoDS_Shape box = BRepPrimAPI_MakeBox(10.0, 10.0, 10.0).Shape();
//	std::cout << "===== Box =====\n";
//	PrintTopoInfo(box);
//	PrintGeoProp(box);
//
//	// 1.2 球体 半径5
//	TopoDS_Shape sphere = BRepPrimAPI_MakeSphere(5.0).Shape();
//	std::cout << "===== Sphere =====\n";
//	PrintTopoInfo(sphere);
//	PrintGeoProp(sphere);
//
//	// 1.3 圆柱体 半径3，高度10
//	TopoDS_Shape cylinder = BRepPrimAPI_MakeCylinder(3.0, 10.0).Shape();
//	std::cout << "===== Cylinder =====\n";
//	PrintTopoInfo(cylinder);
//	PrintGeoProp(cylinder);
//
//	// 1.4 圆台 底部半径5，顶部半径2，高度10
//	TopoDS_Shape cone = BRepPrimAPI_MakeCone(5.0, 2.0, 10.0).Shape();
//	std::cout << "===== Cone =====\n";
//	PrintTopoInfo(cone);
//	PrintGeoProp(cone);
//
//	// 2.几何数据导出（以正方体为例） 
//	std::string stepFile = "./box.step";
//	std::string igesFile = "./box.iges";
//	if (ExportSTEP(box, stepFile))
//		std::cout << "STEP export successful: " << stepFile << std::endl;
//	else
//		std::cout << "STEP export failed" << std::endl;
//
//	if (ExportIGES(box, igesFile))
//		std::cout << "IGES export successful: " << igesFile << std::endl;
//	else
//		std::cout << "IGES export failed" << std::endl;
//}