#include <cmath>
#include <gtest/gtest.h>

#include "../src/Vector.h"

TEST(VectorTest, DefaultConstructor) {
    
    Vector test;
    
    ASSERT_EQ(0.0, test.x);
    ASSERT_EQ(0.0, test.y);
    ASSERT_EQ(0.0, test.z);
}

TEST(VectorTest, UserDefinedConstructor) {
    
    Vector test(2.0, -55, 3.14563);
    
    ASSERT_EQ(2.0,     test.x);
    ASSERT_EQ(-55.0,   test.y);
    ASSERT_EQ(3.14563, test.z);
}

TEST(VectorTest, OperatorAdd) {

    Vector a;
    Vector b(1.0, -2.0, 3.0);
    Vector c = a+b;

    ASSERT_EQ(1.0,  c.x);
    ASSERT_EQ(-2.0, c.y);
    ASSERT_EQ(3.0,  c.z);
}

TEST(VectorTest, MagnitudePostiveValues) {

    Vector test(1.0, 2.0, 3.0);

}

TEST(VectorTest, NormalizePositiveValues) {
    
    Vector test(2.0, 2.0, 2.0);
    Vector normal = test.normalize();

    double magn = test.magnitude();
    ASSERT_EQ(test.x/magn, normal.x);
    ASSERT_EQ(test.y/magn, normal.y);
    ASSERT_EQ(test.z/magn, normal.z);
}
