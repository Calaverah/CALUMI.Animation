#include <gtest/gtest.h>
#include <AnimMath>

using namespace CALUMI::Math;

#define GTEST(x) TEST(Vector3Suite, x)

GTEST(Construction)
{
    Vector3 vec1;
    EXPECT_TRUE(vec1.getX() == 0 && vec1.getY() == 0 && vec1.getZ() == 0);

    Vector3 vec2(1.0f,1.0f,1.0f);
    EXPECT_TRUE(vec2.getX() == 1.0f && vec2.getY() == 1.0f && vec2.getZ() == 1.0f);

    Vector3 vec3(1.0f, 1.0f, 1.0f);
    EXPECT_TRUE(vec3.getX() == 1.0 && vec3.getY() == 1.0 && vec3.getZ() == 1.0);

    Vector3 vec4(1.0f, 1.0f, 1.0f);
    Quaternion quaternion1(vec4, static_cast<float>(ToRadians(50.0)));
    EXPECT_TRUE(vec4.getX() == 1.0 && vec4.getY() == 1.0 && vec4.getZ() == 1.0);
}

//TODO: More tests