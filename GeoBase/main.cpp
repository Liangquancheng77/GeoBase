#include <iostream>
#include<gtest/gtest.h>
#include "Vector3.h"



TEST(Vector3Test, Addition) {

    const Vector3 a(1, 2, 3);
    const Vector3 b(4, 5, 6);
    Vector3 c = a + b;
    EXPECT_DOUBLE_EQ(c.x, 5);
    EXPECT_DOUBLE_EQ(c.y, 7);
    EXPECT_DOUBLE_EQ(c.z, 9);


}

TEST(Vector3Test, Subtraction) {
    Vector3 a(7,8,9);
    Vector3 b(1,4,6);
    Vector3 c = a - b;
    EXPECT_DOUBLE_EQ(c.x, 6);
    EXPECT_DOUBLE_EQ(c.y, 4);
    EXPECT_DOUBLE_EQ(c.z, 3);
}

TEST(Vector3Test, ScalarMultiplication) {
    Vector3 a(7, 8, 9);
    Vector3 c = a * 1.6;
    EXPECT_DOUBLE_EQ(c.x, 11.2);
    EXPECT_DOUBLE_EQ(c.y, 12.8);
    EXPECT_DOUBLE_EQ(c.z, 14.4);
}

TEST(Vector3Test, ScalarDivision) {
    Vector3 a(7, 8, 9);
    Vector3 c = a / 5;
    EXPECT_DOUBLE_EQ(c.x, 1.4);
    EXPECT_DOUBLE_EQ(c.y, 1.6);
    EXPECT_DOUBLE_EQ(c.z, 1.8);
}

TEST(Vector3Test, DotProduct) {
    Vector3 a(0,1,3);
    Vector3 b(1, 4, 6);
    double c = a.dot(b);
    EXPECT_DOUBLE_EQ(c, 22);
}

TEST(Vector3Test, CrossProduct) {
    Vector3 a(1, 0, 0);
    Vector3 b(0, 1, 0);
    Vector3 c = a.cross(b);
    EXPECT_DOUBLE_EQ(c.x, 0);
    EXPECT_DOUBLE_EQ(c.y, 0);
    EXPECT_DOUBLE_EQ(c.z, 1);
}


TEST(Vector3Test, Length) {
    Vector3 a(3, 4, 12);
    double len = a.length();
    EXPECT_DOUBLE_EQ(len, 13);
}

TEST(Vector3Test, Normalized) {
    Vector3 a(3, 4, 12);
    Vector3 c = a.normalized();
    EXPECT_DOUBLE_EQ(c.x, 3.0/13.0);
    EXPECT_DOUBLE_EQ(c.y, 4.0/13.0);
    EXPECT_DOUBLE_EQ(c.z, 12.0/13.0);
}




int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

