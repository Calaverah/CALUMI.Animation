#include <gtest/gtest.h>
#include <AnimMath>
#include "../Common.h"

using namespace CALUMI::Math;

#define GTEST(x) TEST(TransformSuite, x)

GTEST(TransformConstruction)
{
    Transform t1;
    {
        const Transform t0;
        EXPECT_QUATNEAR(t0.rotation(), Quaternion(), 0.000001f);
        EXPECT_VEC3NEAR(t0.position(), Vector3(), 0.000001f);
        t1 = t0;
    }
    EXPECT_QUATNEAR(t1.rotation(), Quaternion(), 0.000001f);
    EXPECT_VEC3NEAR(t1.position(), Vector3(), 0.000001f);
}

GTEST(TransformConversion)
{
    Transform root;
    root.setPosition(Vector3(1.0f,0.0f,1.0f));
    root.setRotation(Quaternion(
        EulerDefinition(0.0f, 0.0f, static_cast<float>(ToRadians(90.0)), EulerDefinition::EulerOrder::XYZ)
        ));

    const Transform child(
        Vector3(50.0f,0.0f,0.0f),
        Quaternion(0.0f,0.0f,1.0f,1.0f,true)
        );

    const Transform expectedChildGlobal(
        Vector3(1.0f,50.0f,1.0f),
        Quaternion(0.0f,0.0f,1.0f,0.0f)
        );

    const Transform childGlobal = child.global(root);

    EXPECT_QUATNEAR(childGlobal.rotation(), expectedChildGlobal.rotation(), 0.000001f);
    EXPECT_VEC3NEAR(childGlobal.position(), expectedChildGlobal.position(), 0.000002f);

    const Transform childLocal = childGlobal.local(root);

    EXPECT_QUATNEAR(childLocal.rotation(), child.rotation(), 0.000001f);
    EXPECT_VEC3NEAR(childLocal.position(), child.position(), 0.000002f);

    const Transform expectedRelative(
        Vector3(0.0f,-49.0f,-1.0f),
        Quaternion()
    );
    const Transform relative = child.local(root);

    EXPECT_QUATNEAR(relative.rotation(), expectedRelative.rotation(), 0.000001f);
    EXPECT_VEC3NEAR(relative.position(), expectedRelative.position(), 0.000005f);
}