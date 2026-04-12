#include <gtest/gtest.h>
#include <AnimMath>

using namespace CALUMI::Math;

#define GTEST(x) TEST(Vector3Suite, x)

GTEST(Construction)
{
    Vector3 vec1;
    EXPECT_TRUE(vec1.x() == 0 && vec1.y() == 0 && vec1.z() == 0);

    Vector3 vec2(1.0f,1.0f,1.0f);
    EXPECT_TRUE(vec2.x() == 1.0f && vec2.y() == 1.0f && vec2.z() == 1.0f);

    Vector3 vec3(1.0f, 1.0f, 1.0f);
    EXPECT_TRUE(vec3.x() == 1.0 && vec3.y() == 1.0 && vec3.z() == 1.0);

    Vector3 vec4(1.0f, 1.0f, 1.0f);
    Quaternion quaternion1(vec4, static_cast<float>(ToRadians(50.0)));
    EXPECT_TRUE(vec4.x() == 1.0 && vec4.y() == 1.0 && vec4.z() == 1.0);
}

//TODO: More tests