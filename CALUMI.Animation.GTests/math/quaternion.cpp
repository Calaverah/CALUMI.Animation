#include <gtest/gtest.h>
#include <AnimMath>
#include "../Common.h"

using namespace CALUMI::Math;

#define GTEST(x) TEST(QuaternionSuite, x)

GTEST(Construction)
{
    Quaternion q1;
    Quaternion q2(q1);
    EXPECT_TRUE(q1.w() == 1.0f && q1.x() == 0.0f && q1.y() == 0.0f && q1.z() == 0.0f);
    EXPECT_TRUE(q2.w() == q1.w() && q2.x() == q1.x() && q2.y() == q1.y() && q2.z() == q1.z());
    EXPECT_TRUE(q1 == q1.Identity);
    EXPECT_FALSE(q1 != q2);

    float buffer[4] = {0.0f,1.0f,0.0f,1.0f};
    Quaternion qMem(buffer);
    EXPECT_TRUE(qMem.w() == float(CLA_SQRT1_2) && qMem.y() == qMem.w() && qMem.x() == 0.0f && qMem.z() == 0.0f);

    Quaternion qAxis1(Vector3(1.0f,2.0f,3.0f), static_cast<float>(ToRadians(50.0)));
    Quaternion qAxis1D(Vector3D(1.0,2.0,3.0), static_cast<float>(ToRadians(50.0)));
    Quaternion qAxis1Exp(0.1129495, 0.225899, 0.3388484, 0.9063078);
    EXPECT_TRUE(qAxis1.areEqual(qAxis1Exp,0.0000001f));
    EXPECT_TRUE(qAxis1 == qAxis1D);

    Quaternion qNumeric(1.0f,0.0f,1.0f,0.0f);
    Quaternion qNumericD(1.0,0.0,1.0,0.0);
    EXPECT_TRUE(qNumeric.z() == float(CLA_SQRT1_2) && qNumeric.x() == qNumeric.z() && qNumeric.y() == 0.0f && qNumeric.w() == 0.0f);
    EXPECT_TRUE(qNumericD.z() == float(CLA_SQRT1_2) && qNumericD.x() == qNumericD.z() && qNumericD.y() == 0.0f && qNumericD.w() == 0.0f);
}

GTEST(Addition)
{
    Quaternion quaternion1(1.0, 1.5, 1.0, 1.0);
    Quaternion quaternion2(1.01, 1.0, 0.0, 2.0);
    Quaternion quaternion3(0.8480776, 1.0622199, 0.4364358, 1.2515682);
    EXPECT_TRUE(quaternion3.areEqual(quaternion1 + quaternion2, 0.0000001f));
    EXPECT_FALSE(quaternion3.areEqual(quaternion1 + quaternion2 + quaternion2, 0.0000001f));
}

GTEST(conjugate)
{
    Quaternion quaternion1(1.0, 2.0, 3.0, 4.0);
    Quaternion quaternion2(-1.0, -2.0, -3.0, 4.0);
    EXPECT_FALSE(quaternion1.areEqual(quaternion1.conjugate(), 0.0000001f));
    EXPECT_TRUE(quaternion1.areEqual(quaternion2.conjugate(), 0.0000001f));
}

GTEST(EquivalentRotation)
{
    Quaternion quaternion1(1.0, 2.0, 3.0, 4.0);
    Quaternion quaternion2 = -quaternion1;
    Quaternion quaternion3 = quaternion1.conjugate();
    EXPECT_TRUE(quaternion1.areSameRotation(quaternion2, 0.0000001f));
    EXPECT_FALSE(quaternion1.areSameRotation(quaternion3, 0.0000001f));
}

GTEST(Multiplication)
{
    Quaternion qU1;
    Quaternion qU2;
    Quaternion q707(1.0f,0.0f,0.0f,1.0f);
    Quaternion q1234(1.0, 2.0, 3.0, 4.0);

    EXPECT_TRUE(qU1 == qU1 * qU2);
    EXPECT_TRUE(q1234.areEqual(qU1 * q1234, 0.0000001f));
    EXPECT_TRUE(q1234.areSameRotation(qU1 * -q1234, 0.0000001f));
    EXPECT_FALSE(q1234.areEqual(q707 * q1234, 0.0000001f));
    EXPECT_FALSE(q1234.areEqual(q707 * -q1234, 0.0000001f));
}

GTEST(Division)
{
    Quaternion quaternion1;
    Quaternion quaternion2(1.0, 2.0, 3.0, 4.0);
    EXPECT_TRUE(quaternion1.areEqual(quaternion2 / quaternion2, 0.0000001f));
}

GTEST(Length)
{
    //Length should always be 1
    Quaternion quaternionN(1.0, 2.0, 3.0, 4.0);
    //Length should be 5.4772255750f
    Quaternion quaternionL(1.0, 2.0, 3.0, 4.0, false);

    EXPECT_NEAR(1.0f,quaternionN.length(),0.0000001f);
    EXPECT_NEAR(5.4772255750f,quaternionL.length(),0.0000001f);
    EXPECT_FALSE(std::abs(1.0f - quaternionL.length()) == 0.0000001f);

    //Squared
    EXPECT_NEAR(1.0f, quaternionN.lengthSquared(), 0.000001f);
    EXPECT_NEAR(30.0f, quaternionL.lengthSquared(), 0.0000001f);
    EXPECT_FALSE(std::abs(1.0f - quaternionL.lengthSquared()) == 0.0000001f);
}

GTEST(DotProduct)
{
    Quaternion q1(1.0, 2.0, 3.0, 4.0, false);
    Quaternion q2(2.251, 2.0, 3.0, 4.0, false);
    EXPECT_NEAR(q1.dot(q1), q1.lengthSquared(), 0.0000001f);
    EXPECT_NEAR(q2.dot(Quaternion().Identity), 4.0f, 0.0000001f);
}

GTEST(AxisAngleMultiplication)
{
    Quaternion quaternion1(Vector3D(0.0, 0.0, 1.0), ToRadians(180.0)); //0,0,1,0
    Quaternion quaternion2(Vector3D(0.0, 1.0, 0.0), ToRadians(30.0)); //0,0.258819,0,0.9659258
    Quaternion expectedOut1(-0.258819, 0.0, 0.965926, 0.0); //180 on Z axis
    EXPECT_TRUE(expectedOut1.areSameRotation(quaternion1 * quaternion2, 0.0000001f));

    Quaternion quaternion3(Vector3D(0.0, 0.0, 1.0), ToRadians(180.0)); //0,0,1,0
    Quaternion quaternion4(Vector3D(0.0, 1.0, 0.0), ToRadians(30.0)); //0,0.258819,0,0.9659258
    Quaternion expectedOut2 = quaternion3 * quaternion4;
    Quaternion result;
    RotateQuaternionByAxisAngleC(&quaternion4, &result, 0.0, 0.0, 1.0, static_cast<float>(ToRadians(180.0)));
    EXPECT_TRUE(expectedOut2.areSameRotation(result, 0.0000001f));
}

GTEST(AngleDistance)
{
    Quaternion quaternion1;
    Quaternion quaternion2(1.0f, 0.0f, 0.0f, 0.0f);
    float diff1 = quaternion1.angularDistance(quaternion2);

    EXPECT_EQ(static_cast<float>(ToRadians(180.0f)), diff1);

    Quaternion quaternion3;
    Quaternion quaternion4(-1.0f, 0.0f, 0.0f, 0.0f);
    float diff2 = quaternion3.angularDistance(quaternion4);

    EXPECT_EQ(static_cast<float>(ToRadians(180.0f)), diff2);

    Quaternion quaternion5;
    Quaternion quaternion6(Vector3D(1.0, 1.0, 0.5), ToRadians(90.0));
    float diff3 = quaternion5.angularDistance(quaternion6);

    EXPECT_EQ(static_cast<float>(ToRadians(90.0f)), diff3);
}

GTEST(Slerp)
{
    Quaternion q1;
    Quaternion q2(Vector3D(0.0, 1.0, 0.0), ToRadians(90.0));
    Quaternion expected1(Vector3D(0.0, 1.0, 0.0), ToRadians(45.0));
    Quaternion result1 = q1.sLerp(q2, 0.5f);

    EXPECT_TRUE(expected1.areEqual(result1,0.001f));

    Quaternion q3;
    Quaternion q4(Vector3D(0.0, 1.0, 0.0), ToRadians(178.0));
    Quaternion expected2(Vector3D(0.0, 1.0, 0.0), ToRadians(89.0));
    Quaternion result2 = q3.sLerp(q4, 0.5f);

    EXPECT_TRUE(expected2.areEqual(result2, 0.001f));
}

GTEST(EulerXYZ)
{
    Quaternion q1(float(ToRadians(30)), float(ToRadians(60)), float(ToRadians(45)), Quaternion::EulerOrder::XYZ);
    Quaternion expected1(0.3919038f, 0.3604234f, 0.4396797f, 0.7233174f);
    EXPECT_TRUE(expected1.areEqual(q1, 0.001));

    //GTEST(EulerXZY)
    Quaternion q2(float(ToRadians(30)), float(ToRadians(60)), float(ToRadians(45)), Quaternion::EulerOrder::XZY);
    Quaternion expected2(0.02226f, 0.200562f, 0.531976f, 0.8223632f);
    EXPECT_TRUE(expected2.areEqual(q2, 0.001));

    //GTEST(EulerYXZ)
    Quaternion q3(float(ToRadians(30)), float(ToRadians(60)), float(ToRadians(45)), Quaternion::EulerOrder::YXZ);
    Quaternion expected3(0.531976f, 0.02226f, 0.200562f, 0.8223632f);
    EXPECT_TRUE(expected3.areEqual(q3, 0.001));

    //GTEST(EulerYZX)
    Quaternion q4(float(ToRadians(30)), float(ToRadians(60)), float(ToRadians(45)), Quaternion::EulerOrder::YZX);
    Quaternion expected4(0.439680f, 0.391904f, 0.360423f, 0.723317f);
    EXPECT_TRUE(expected4.areEqual(q4, 0.001));

    //GTEST(EulerZXY)
    Quaternion q5(float(ToRadians(30)), float(ToRadians(60)), float(ToRadians(45)), Quaternion::EulerOrder::ZXY);
    Quaternion expected5(0.360423f, 0.439680f, 0.391904f, 0.723317f);
    EXPECT_TRUE(expected5.areEqual(q5, 0.001));

    //GTEST(EulerZYX)
    Quaternion q6(float(ToRadians(30)), float(ToRadians(60)), float(ToRadians(45)), Quaternion::EulerOrder::ZYX);
    Quaternion expected6(0.200562f, 0.531976f, 0.022260f, 0.822363f);
    EXPECT_TRUE(expected6.areEqual(q6, 0.001));

    //GTEST(EulerXYX)
    Quaternion q7(float(ToRadians(30)), float(ToRadians(60)), float(ToRadians(45)), Quaternion::EulerOrder::XYX);
    Quaternion expected7(0.527203f, 0.495722f, -0.065263f, 0.687064f);
    EXPECT_TRUE(expected7.areEqual(q7, 0.001));

    //GTEST(EulerXZX)
    Quaternion q8(float(ToRadians(30)), float(ToRadians(60)), float(ToRadians(45)), Quaternion::EulerOrder::XZX);
    Quaternion expected8(0.527203f, 0.065263f, 0.495722f, 0.687064f);
    EXPECT_TRUE(expected8.areEqual(q8, 0.001));

    //GTEST(EulerYXY)
    Quaternion q9(float(ToRadians(30)), float(ToRadians(60)), float(ToRadians(45)), Quaternion::EulerOrder::YXY);
    Quaternion expected9(0.495722f, 0.527203f, 0.065263f, 0.687064f);
    EXPECT_TRUE(expected9.areEqual(q9, 0.001));

    //GTEST(EulerYZY)
    Quaternion q10(float(ToRadians(30)), float(ToRadians(60)), float(ToRadians(45)), Quaternion::EulerOrder::YZY);
    Quaternion expected10(-0.065263f, 0.527203f, 0.495722f, 0.687064f);
    EXPECT_TRUE(expected10.areEqual(q10, 0.001));

    //GTEST(EulerZXZ)
    Quaternion q11(float(ToRadians(30)), float(ToRadians(60)), float(ToRadians(45)), Quaternion::EulerOrder::ZXZ);
    Quaternion expected11(0.495722f, -0.065263f, 0.527203f, 0.687064f);
    EXPECT_TRUE(expected11.areEqual(q11, 0.001));

    //GTEST(EulerZYZ)
    Quaternion q12(float(ToRadians(30)), float(ToRadians(60)), float(ToRadians(45)), Quaternion::EulerOrder::ZYZ);
    Quaternion expected12(0.065263f, 0.495722f, 0.527203f, 0.687064f);
    EXPECT_TRUE(expected12.areEqual(q12, 0.001));
}

GTEST(Offset)
{
    Quaternion q1(0.0f, 0.0f, static_cast<float>(ToRadians(30.0f)), Quaternion::EulerOrder::XYZ);
    const Quaternion q2(0.0f, 0.0f, static_cast<float>(ToRadians(60.0f)), Quaternion::EulerOrder::XYZ);
    const Quaternion qExpect(0.0f, 0.0f, static_cast<float>(ToRadians(90.0f)), Quaternion::EulerOrder::XYZ);

    q1.rotateBy(q2);
    EXPECT_QUATNEAR(q1, qExpect, 0.000001f);

    const auto qDist = Quaternion::rotationOffset(q1, q2);
    const Quaternion qDistExpect(0.0f, 0.0f, static_cast<float>(ToRadians(-30.0f)), Quaternion::EulerOrder::XYZ);
    const auto qNone = Quaternion::rotationOffset(q1, qExpect);

    EXPECT_QUATNEAR(qDist, qDistExpect, 0.000001f);
    EXPECT_QUATNEAR(qNone, Quaternion(), 0.000001f);

    q1.rotateBy(qDist);
    EXPECT_QUATNEAR(q1, q2, 0.000001f);
}