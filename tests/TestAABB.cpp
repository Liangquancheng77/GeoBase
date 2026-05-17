#include <gtest/gtest.h>
#include "../include/GeoBase/AABB.h"

// 创建 (0,0,0)-(2,2,2) 的AABB，测试点 (1,1,1) 在内，(3,3,3) 在外
TEST(AABBTest, ContainsPoint) {
	AABB box(Point3(0, 0, 0), Point3(2, 2, 2));
	EXPECT_TRUE(box.contains(Point3(1, 1, 1))); // 内部点
	EXPECT_FALSE(box.contains(Point3(3, 3, 3))); // 外部点
}

// 合并两个分离的AABB，验证新AABB尺寸
TEST(AABBTest, AABBTest1) {

	AABB box1(Point3(0, 0, 0), Point3(2, 2, 2));

	AABB box2(Point3(3, 3, 3), Point3(4, 4, 4));

	box1.merge(box2);

	EXPECT_DOUBLE_EQ(box1.getMin().x, 0.0);
	EXPECT_DOUBLE_EQ(box1.getMin().y, 0.0);
	EXPECT_DOUBLE_EQ(box1.getMin().z, 0.0);
	EXPECT_DOUBLE_EQ(box1.getMax().x, 4.0);
	EXPECT_DOUBLE_EQ(box1.getMax().y, 4.0);
	EXPECT_DOUBLE_EQ(box1.getMax().z, 4.0);

}

// 两个相交的AABB验证 intersects 为 true
TEST(AABBTest, Intersects) {
	AABB box1(Point3(0, 0, 0), Point3(2, 2, 2));
	AABB box2(Point3(1, 1, 1), Point3(3, 3, 3));
	EXPECT_TRUE(box1.intersects(box2)); // 相交
}

// 验证平移矩阵变换和逆矩阵变换后AABB近似相等
TEST(AABBTest, Transform) {
	AABB box1(Point3(1, 1, 1), Point3(3, 3, 3));
	Matrix4 mat;
	mat.setTranslation(1,2,3);
	AABB box2 = box1.transform(mat);
	Matrix4 mat2 = mat.inverseAffine();
	AABB box3 = box2.transform(mat2);
	EXPECT_NEAR(box1.getMin().x, box3.getMin().x, EPS_ABS);
	EXPECT_NEAR(box1.getMin().y, box3.getMin().y, EPS_ABS);
	EXPECT_NEAR(box1.getMin().z, box3.getMin().z, EPS_ABS);
	EXPECT_NEAR(box1.getMax().x, box3.getMax().x, EPS_ABS);
	EXPECT_NEAR(box1.getMax().y, box3.getMax().y, EPS_ABS);
	EXPECT_NEAR(box1.getMax().z, box3.getMax().z, EPS_ABS);
}

// 对AABB应用绕Z轴45度旋转，验证新AABB体积变大
TEST(AABBTest, TransformRotation) {
	AABB box(Point3(1, 1, 1), Point3(3, 3, 3));
	Matrix4 mat;
	mat.setRotationZ(PI / 4); // 绕Z轴旋转45度
	AABB rotatedBox = box.transform(mat);
	double originalVolume = (box.getMax().x - box.getMin().x) *
		(box.getMax().y - box.getMin().y) *
		(box.getMax().z - box.getMin().z);
	double rotatedVolume = (rotatedBox.getMax().x - rotatedBox.getMin().x) *
		(rotatedBox.getMax().y - rotatedBox.getMin().y) *
		(rotatedBox.getMax().z - rotatedBox.getMin().z);
	EXPECT_GT(rotatedVolume, originalVolume); // 旋转后体积变大
}

// 零尺寸 AABB：AABB(Point3(1,1,1), Point3(1,1,1))，验证 contains 对 (1,1,1) 返回 true，对其他点返回 false；intersects 与自身相交。
TEST(AABBTest, ZeroSizeAABB) {
	AABB box(Point3(1, 1, 1), Point3(1, 1, 1));
	EXPECT_TRUE(box.contains(Point3(1, 1, 1))); // 包含自身点
	EXPECT_FALSE(box.contains(Point3(0, 0, 0))); // 不包含其他点
	EXPECT_TRUE(box.intersects(box)); // 与自身相交
}






































