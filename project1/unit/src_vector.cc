#include <cmath>
#include <gtest/gtest.h>

#include "../src/Vector.h"

TEST(VectorTest, DefaultConstructor) {
    
    Vector test;
    
    EXPECT_EQ(0.0, test.x);
    EXPECT_EQ(0.0, test.y);
    EXPECT_EQ(0.0, test.z);
}

TEST(VectorTest, UserDefinedConstructor) {
    
    Vector test(2.0, -55, 3.14563);
    
    EXPECT_EQ(2.0,     test.x);
    EXPECT_EQ(-55.0,   test.y);
    EXPECT_EQ(3.14563, test.z);
}

TEST(VectorTest, OperatorAdd) {

    Vector a;
    Vector b(1.0, -2.0, 3.0);
    Vector c = a+b;

    EXPECT_EQ(1.0,  c.x);
    EXPECT_EQ(-2.0, c.y);
    EXPECT_EQ(3.0,  c.z);
}

TEST(VectorTest, OperatorSubtract) {

    Vector a;
    Vector b(0.0, -2.0, 3.0);
    Vector c = a-b;

    EXPECT_EQ(0.0,  c.x);
    EXPECT_EQ(2.0, c.y);
    EXPECT_EQ(-3.0,  c.z);
}

TEST(VectorTest, NegateVector) {

    Vector b(0.0, -2.0, 3.0);
    Vector c = -b;

    EXPECT_EQ(0.0,  c.x);
    EXPECT_EQ(2.0, c.y);
    EXPECT_EQ(-3.0,  c.z);
}

TEST(VectorTest, ScaleVectorPrefix) {

    Vector b(0.0, -2.0, 3.0);
    Vector c = b*2.0;

    EXPECT_EQ(0.0,  c.x);
    EXPECT_EQ(-4.0, c.y);
    EXPECT_EQ(6.0,  c.z);
}

TEST(VectorTest, DISABLED_ScaleVectorPostfix) {

    /*
    Vector b(0.0, -2.0, 3.0);
    Vector c = 2.0*b;

    EXPECT_EQ(0.0,  c.x);
    EXPECT_EQ(-4.0, c.y);
    EXPECT_EQ(6.0,  c.z);
    */
}
TEST(VectorTest, MagnitudePositiveValues) {

    Vector test(1.0, 2.0, 3.0);
    double actual = test.magnitude();
    double expected = sqrt(pow(test.x,2) + pow(test.y,2) + pow(test.z,2));

    ASSERT_EQ(expected, actual);
}

TEST(VectorTest, MagnitudeNegativeValues) {

    Vector test(-1.0, -2.0, -3.0);
    double actual = test.magnitude();
    double expected = sqrt(pow(test.x,2) + pow(test.y,2) + pow(test.z,2));

    ASSERT_EQ(expected, actual);
}

TEST(VectorTest, MagnitudeMixedValues) {

    Vector test(1.0, 0, -3.0);
    double actual = test.magnitude();
    double expected = sqrt(pow(test.x,2) + pow(test.z,2));

    ASSERT_EQ(expected, actual);
}

TEST(VectorTest, NormalizePositiveValues) {
    
    Vector test(2.0, 2.0, 2.0);
    Vector normal = test.normalize();

    double magn = test.magnitude();
    EXPECT_EQ(test.x/magn, normal.x);
    EXPECT_EQ(test.y/magn, normal.y);
    EXPECT_EQ(test.z/magn, normal.z);
}

TEST(VectorTest, NormalizeNegativeValues) {
    
    Vector test(-2.0, -2.0, -2.0);
    Vector normal = test.normalize();

    double magn = test.magnitude();
    EXPECT_EQ(test.x/magn, normal.x);
    EXPECT_EQ(test.y/magn, normal.y);
    EXPECT_EQ(test.z/magn, normal.z);
}


TEST(VectorTest, NormalizeMixedValues) {
    
    Vector test(-2.0, 0, 2.0);
    Vector normal = test.normalize();

    double magn = test.magnitude();
    EXPECT_EQ(test.x/magn, normal.x);
    EXPECT_EQ(test.y/magn, normal.y);
    EXPECT_EQ(test.z/magn, normal.z);
}
