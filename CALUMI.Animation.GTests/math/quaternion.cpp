#include <gtest/gtest.h>
#include <AnimMath>
#include "../Common.h"

using namespace CALUMI::Math;

#define GTEST(x) TEST(QuaternionSuite, x)

GTEST(Construction)
{
    const Quaternion q1;
    const Quaternion q2(q1); // NOLINT(*-unnecessary-copy-initialization)
    EXPECT_TRUE(q1.w() == 1.0f && q1.x() == 0.0f && q1.y() == 0.0f && q1.z() == 0.0f);
    EXPECT_TRUE(q2.w() == q1.w() && q2.x() == q1.x() && q2.y() == q1.y() && q2.z() == q1.z());
    EXPECT_TRUE(q1 == q1.Identity);
    EXPECT_FALSE(q1 != q2);

    float buffer[4] = {0.0f,1.0f,0.0f,1.0f};
    const Quaternion qMem(buffer);
    EXPECT_TRUE(qMem.w() == static_cast<float>(CLA_SQRT1_2) && qMem.y() == qMem.w() && qMem.x() == 0.0f && qMem.z() == 0.0f);

    const Quaternion qAxis1(Vector3(1.0f,2.0f,3.0f), static_cast<float>(ToRadians(50.0)));
    const Quaternion qAxis1D(Vector3D(1.0,2.0,3.0), static_cast<float>(ToRadians(50.0)));
    const Quaternion qAxis1Exp(0.1129495, 0.225899, 0.3388484, 0.9063078);
    EXPECT_TRUE(qAxis1.areEqual(qAxis1Exp,0.0000001f));
    EXPECT_TRUE(qAxis1 == qAxis1D);

    const Quaternion qNumeric(1.0f,0.0f,1.0f,0.0f);
    const Quaternion qNumericD(1.0,0.0,1.0,0.0);
    EXPECT_TRUE(qNumeric.z() == static_cast<float>(CLA_SQRT1_2) && qNumeric.x() == qNumeric.z() && qNumeric.y() == 0.0f && qNumeric.w() == 0.0f);
    EXPECT_TRUE(qNumericD.z() == static_cast<float>(CLA_SQRT1_2) && qNumericD.x() == qNumericD.z() && qNumericD.y() == 0.0f && qNumericD.w() == 0.0f);
}

GTEST(Addition)
{
    const Quaternion quaternion1(1.0, 1.5, 1.0, 1.0);
    const Quaternion quaternion2(1.01, 1.0, 0.0, 2.0);
    const Quaternion quaternion3(0.8480776, 1.0622199, 0.4364358, 1.2515682);
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
    const Quaternion quaternion2 = -quaternion1;
    const Quaternion quaternion3 = quaternion1.conjugate();
    EXPECT_TRUE(quaternion1.areSameRotation(quaternion2, 0.0000001f));
    EXPECT_FALSE(quaternion1.areSameRotation(quaternion3, 0.0000001f));
}

GTEST(Multiplication)
{
    const Quaternion qU1;
    const Quaternion qU2;
    const Quaternion q707(1.0f,0.0f,0.0f,1.0f);
    const Quaternion q1234(1.0, 2.0, 3.0, 4.0);

    EXPECT_TRUE(qU1 == qU1 * qU2);
    EXPECT_TRUE(q1234.areEqual(qU1 * q1234, 0.0000001f));
    EXPECT_TRUE(q1234.areSameRotation(qU1 * -q1234, 0.0000001f));
    EXPECT_FALSE(q1234.areEqual(q707 * q1234, 0.0000001f));
    EXPECT_FALSE(q1234.areEqual(q707 * -q1234, 0.0000001f));
}

GTEST(Division)
{
    const Quaternion quaternion1;
    const Quaternion quaternion2(1.0, 2.0, 3.0, 4.0);
    EXPECT_TRUE(quaternion1.areEqual(quaternion2 / quaternion2, 0.0000001f));
}

GTEST(Length)
{
    //Length should always be 1
    const Quaternion quaternionN(1.0, 2.0, 3.0, 4.0);
    //Length should be 5.4772255750f
    const Quaternion quaternionL(1.0, 2.0, 3.0, 4.0, false);

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
    const Quaternion q1(1.0, 2.0, 3.0, 4.0, false);
    const Quaternion q2(2.251, 2.0, 3.0, 4.0, false);
    EXPECT_NEAR(q1.dot(q1), q1.lengthSquared(), 0.0000001f);
    EXPECT_NEAR(q2.dot(Quaternion().Identity), 4.0f, 0.0000001f);
}

GTEST(AxisAngleMultiplication)
{
    const Quaternion quaternion1(Vector3D(0.0, 0.0, 1.0), ToRadians(180.0)); //0,0,1,0
    const Quaternion quaternion2(Vector3D(0.0, 1.0, 0.0), ToRadians(30.0)); //0,0.258819,0,0.9659258
    const Quaternion expectedOut1(-0.258819, 0.0, 0.965926, 0.0); //180 on Z axis
    EXPECT_TRUE(expectedOut1.areSameRotation(quaternion1 * quaternion2, 0.0000001f));

    const Quaternion quaternion3(Vector3D(0.0, 0.0, 1.0), ToRadians(180.0)); //0,0,1,0
    Quaternion quaternion4(Vector3D(0.0, 1.0, 0.0), ToRadians(30.0)); //0,0.258819,0,0.9659258
    const Quaternion expectedOut2 = quaternion3 * quaternion4;
    Quaternion result;
    RotateQuaternionByAxisAngleC(&quaternion4, &result, 0.0, 0.0, 1.0, static_cast<float>(ToRadians(180.0)));
    EXPECT_TRUE(expectedOut2.areSameRotation(result, 0.0000001f));
}

GTEST(AngleDistance)
{
    const Quaternion quaternion1;
    const Quaternion quaternion2(1.0f, 0.0f, 0.0f, 0.0f);
    const float diff1 = quaternion1.angularDistance(quaternion2);

    EXPECT_EQ(static_cast<float>(ToRadians(180.0f)), diff1);

    const Quaternion quaternion3;
    const Quaternion quaternion4(-1.0f, 0.0f, 0.0f, 0.0f);
    const float diff2 = quaternion3.angularDistance(quaternion4);

    EXPECT_EQ(static_cast<float>(ToRadians(180.0f)), diff2);

    const Quaternion quaternion5;
    const Quaternion quaternion6(Vector3D(1.0, 1.0, 0.5), ToRadians(90.0));
    const float diff3 = quaternion5.angularDistance(quaternion6);

    EXPECT_EQ(static_cast<float>(ToRadians(90.0f)), diff3);
}

GTEST(Slerp)
{
    const Quaternion q1;
    const Quaternion q2(Vector3D(0.0, 1.0, 0.0), ToRadians(90.0));
    const Quaternion expected1(Vector3D(0.0, 1.0, 0.0), ToRadians(45.0));
    const Quaternion result1 = q1.sLerp(q2, 0.5f);

    EXPECT_TRUE(expected1.areEqual(result1,0.001f));

    const Quaternion q3;
    const Quaternion q4(Vector3D(0.0, 1.0, 0.0), ToRadians(178.0));
    const Quaternion expected2(Vector3D(0.0, 1.0, 0.0), ToRadians(89.0));
    const Quaternion result2 = q3.sLerp(q4, 0.5f);

    EXPECT_TRUE(expected2.areEqual(result2, 0.001f));
}

GTEST(Euler)
{
    //XYZ
    Quaternion q1(static_cast<float>(ToRadians(30)), static_cast<float>(ToRadians(60)), static_cast<float>(ToRadians(45)), EulerDefinition::EulerOrder::XYZ);
    Quaternion expected1(0.3919038f, 0.3604234f, 0.4396797f, 0.7233174f);
    EXPECT_TRUE(expected1.areEqual(q1, 0.001));

    auto xyzDef = q1.toEuler(EulerDefinition::EulerOrder::XYZ);
    EXPECT_NEAR(xyzDef.first(), static_cast<float>(ToRadians(30)), 0.00001f);
    EXPECT_NEAR(xyzDef.second(), static_cast<float>(ToRadians(60)), 0.00001f);
    EXPECT_NEAR(xyzDef.third(), static_cast<float>(ToRadians(45)), 0.00001f);

    //GTEST(EulerXZY)
    Quaternion q2(static_cast<float>(ToRadians(30)), static_cast<float>(ToRadians(60)), static_cast<float>(ToRadians(45)), EulerDefinition::EulerOrder::XZY);
    Quaternion expected2(0.02226f, 0.200562f, 0.531976f, 0.8223632f);
    EXPECT_TRUE(expected2.areEqual(q2, 0.001));

    auto xzyDef = q2.toEuler(EulerDefinition::EulerOrder::XZY);
    EXPECT_NEAR(xzyDef.first(), static_cast<float>(ToRadians(30)), 0.00001f);
    EXPECT_NEAR(xzyDef.second(), static_cast<float>(ToRadians(60)), 0.00001f);
    EXPECT_NEAR(xzyDef.third(), static_cast<float>(ToRadians(45)), 0.00001f);

    //GTEST(EulerYXZ)
    Quaternion q3(static_cast<float>(ToRadians(30)), static_cast<float>(ToRadians(60)), static_cast<float>(ToRadians(45)), EulerDefinition::EulerOrder::YXZ);
    Quaternion expected3(0.531976f, 0.02226f, 0.200562f, 0.8223632f);
    EXPECT_TRUE(expected3.areEqual(q3, 0.001));

    auto yxzDef = q3.toEuler(EulerDefinition::EulerOrder::YXZ);
    EXPECT_NEAR(yxzDef.first(), static_cast<float>(ToRadians(30)), 0.00001f);
    EXPECT_NEAR(yxzDef.second(), static_cast<float>(ToRadians(60)), 0.00001f);
    EXPECT_NEAR(yxzDef.third(), static_cast<float>(ToRadians(45)), 0.00001f);

    //GTEST(EulerYZX)
    Quaternion q4(static_cast<float>(ToRadians(30)), static_cast<float>(ToRadians(60)), static_cast<float>(ToRadians(45)), EulerDefinition::EulerOrder::YZX);
    Quaternion expected4(0.439680f, 0.391904f, 0.360423f, 0.723317f);
    EXPECT_TRUE(expected4.areEqual(q4, 0.001));

    auto yzxDef = q4.toEuler(EulerDefinition::EulerOrder::YZX);
    EXPECT_NEAR(yzxDef.first(), static_cast<float>(ToRadians(30)), 0.00001f);
    EXPECT_NEAR(yzxDef.second(), static_cast<float>(ToRadians(60)), 0.00001f);
    EXPECT_NEAR(yzxDef.third(), static_cast<float>(ToRadians(45)), 0.00001f);

    //GTEST(EulerZXY)
    Quaternion q5(static_cast<float>(ToRadians(30)), static_cast<float>(ToRadians(60)), static_cast<float>(ToRadians(45)), EulerDefinition::EulerOrder::ZXY);
    Quaternion expected5(0.360423f, 0.439680f, 0.391904f, 0.723317f);
    EXPECT_TRUE(expected5.areEqual(q5, 0.001));

    auto zxyDef = q5.toEuler(EulerDefinition::EulerOrder::ZXY);
    EXPECT_NEAR(zxyDef.first(), static_cast<float>(ToRadians(30)), 0.00001f);
    EXPECT_NEAR(zxyDef.second(), static_cast<float>(ToRadians(60)), 0.00001f);
    EXPECT_NEAR(zxyDef.third(), static_cast<float>(ToRadians(45)), 0.00001f);

    //GTEST(EulerZYX)
    Quaternion q6(static_cast<float>(ToRadians(30)), static_cast<float>(ToRadians(60)), static_cast<float>(ToRadians(45)), EulerDefinition::EulerOrder::ZYX);
    Quaternion expected6(0.200562f, 0.531976f, 0.022260f, 0.822363f);
    EXPECT_TRUE(expected6.areEqual(q6, 0.001));

    auto zyxDef = q6.toEuler(EulerDefinition::EulerOrder::ZYX);
    EXPECT_NEAR(zyxDef.first(), static_cast<float>(ToRadians(30)), 0.00001f);
    EXPECT_NEAR(zyxDef.second(), static_cast<float>(ToRadians(60)), 0.00001f);
    EXPECT_NEAR(zyxDef.third(), static_cast<float>(ToRadians(45)), 0.00001f);

    //GTEST(EulerXYX)
    Quaternion q7(static_cast<float>(ToRadians(30)), static_cast<float>(ToRadians(60)), static_cast<float>(ToRadians(45)), EulerDefinition::EulerOrder::XYX);
    Quaternion expected7(0.527203f, 0.495722f, -0.065263f, 0.687064f);
    EXPECT_TRUE(expected7.areEqual(q7, 0.001));

    auto xyxDef = q7.toEuler(EulerDefinition::EulerOrder::XYX);
    EXPECT_NEAR(xyxDef.first(), static_cast<float>(ToRadians(30)), 0.00001f);
    EXPECT_NEAR(xyxDef.second(), static_cast<float>(ToRadians(60)), 0.00001f);
    EXPECT_NEAR(xyxDef.third(), static_cast<float>(ToRadians(45)), 0.00001f);

    //GTEST(EulerXZX)
    Quaternion q8(static_cast<float>(ToRadians(30)), static_cast<float>(ToRadians(60)), static_cast<float>(ToRadians(45)), EulerDefinition::EulerOrder::XZX);
    Quaternion expected8(0.527203f, 0.065263f, 0.495722f, 0.687064f);
    EXPECT_TRUE(expected8.areEqual(q8, 0.001));

    auto xzxDef = q8.toEuler(EulerDefinition::EulerOrder::XZX);
    EXPECT_NEAR(xzxDef.first(), static_cast<float>(ToRadians(30)), 0.00001f);
    EXPECT_NEAR(xzxDef.second(), static_cast<float>(ToRadians(60)), 0.00001f);
    EXPECT_NEAR(xzxDef.third(), static_cast<float>(ToRadians(45)), 0.00001f);

    //GTEST(EulerYXY)
    Quaternion q9(static_cast<float>(ToRadians(30)), static_cast<float>(ToRadians(60)), static_cast<float>(ToRadians(45)), EulerDefinition::EulerOrder::YXY);
    Quaternion expected9(0.495722f, 0.527203f, 0.065263f, 0.687064f);
    EXPECT_TRUE(expected9.areEqual(q9, 0.001));

    auto yxyDef = q9.toEuler(EulerDefinition::EulerOrder::YXY);
    EXPECT_NEAR(yxyDef.first(), static_cast<float>(ToRadians(30)), 0.00001f);
    EXPECT_NEAR(yxyDef.second(), static_cast<float>(ToRadians(60)), 0.00001f);
    EXPECT_NEAR(yxyDef.third(), static_cast<float>(ToRadians(45)), 0.00001f);

    //GTEST(EulerYZY)
    Quaternion q10(static_cast<float>(ToRadians(30)), static_cast<float>(ToRadians(60)), static_cast<float>(ToRadians(45)), EulerDefinition::EulerOrder::YZY);
    Quaternion expected10(-0.065263f, 0.527203f, 0.495722f, 0.687064f);
    EXPECT_TRUE(expected10.areEqual(q10, 0.001));

    auto yzyDef = q10.toEuler(EulerDefinition::EulerOrder::YZY);
    EXPECT_NEAR(yzyDef.first(), static_cast<float>(ToRadians(30)), 0.00001f);
    EXPECT_NEAR(yzyDef.second(), static_cast<float>(ToRadians(60)), 0.00001f);
    EXPECT_NEAR(yzyDef.third(), static_cast<float>(ToRadians(45)), 0.00001f);

    //GTEST(EulerZXZ)
    Quaternion q11(static_cast<float>(ToRadians(30)), static_cast<float>(ToRadians(60)), static_cast<float>(ToRadians(45)), EulerDefinition::EulerOrder::ZXZ);
    Quaternion expected11(0.495722f, -0.065263f, 0.527203f, 0.687064f);
    EXPECT_TRUE(expected11.areEqual(q11, 0.001));

    auto zxzDef = q11.toEuler(EulerDefinition::EulerOrder::ZXZ);
    EXPECT_NEAR(zxzDef.first(), static_cast<float>(ToRadians(30)), 0.00001f);
    EXPECT_NEAR(zxzDef.second(), static_cast<float>(ToRadians(60)), 0.00001f);
    EXPECT_NEAR(zxzDef.third(), static_cast<float>(ToRadians(45)), 0.00001f);

    //GTEST(EulerZYZ)
    Quaternion q12(static_cast<float>(ToRadians(30)), static_cast<float>(ToRadians(60)), static_cast<float>(ToRadians(45)), EulerDefinition::EulerOrder::ZYZ);
    Quaternion expected12(0.065263f, 0.495722f, 0.527203f, 0.687064f);
    EXPECT_TRUE(expected12.areEqual(q12, 0.001));

    auto zyzDef = q12.toEuler(EulerDefinition::EulerOrder::ZYZ);
    EXPECT_NEAR(zyzDef.first(), static_cast<float>(ToRadians(30)), 0.00001f);
    EXPECT_NEAR(zyzDef.second(), static_cast<float>(ToRadians(60)), 0.00001f);
    EXPECT_NEAR(zyzDef.third(), static_cast<float>(ToRadians(45)), 0.00001f);
}

GTEST(Offset)
{
    Quaternion q1(0.0f, 0.0f, static_cast<float>(ToRadians(30.0f)), EulerDefinition::EulerOrder::XYZ);
    const Quaternion q2(0.0f, 0.0f, static_cast<float>(ToRadians(60.0f)), EulerDefinition::EulerOrder::XYZ);
    const Quaternion qExpect(0.0f, 0.0f, static_cast<float>(ToRadians(90.0f)), EulerDefinition::EulerOrder::XYZ);

    q1.rotateBy(q2);
    EXPECT_QUATNEAR(q1, qExpect, 0.000001f);

    const auto qDist = Quaternion::rotationOffset(q1, q2);
    const Quaternion qDistExpect(0.0f, 0.0f, static_cast<float>(ToRadians(-30.0f)), EulerDefinition::EulerOrder::XYZ);
    const auto qNone = Quaternion::rotationOffset(q1, qExpect);

    EXPECT_QUATNEAR(qDist, qDistExpect, 0.000001f);
    EXPECT_QUATNEAR(qNone, Quaternion(), 0.000001f);

    q1.rotateBy(qDist);
    EXPECT_QUATNEAR(q1, q2, 0.000001f);
}

GTEST(Json)
{
    const CALUMI::Utilities::JsonObject q0 = Quaternion().toJson();
    EXPECT_EQ(0.0f, q0["x"].toFloat());
    EXPECT_EQ(0.0f, q0["y"].toFloat());
    EXPECT_EQ(0.0f, q0["z"].toFloat());
    EXPECT_EQ(1.0f, q0["w"].toFloat());

    const CALUMI::Utilities::JsonObject q1 = Quaternion(1.0f,0.0f,0.0f,1.0f,true).toJson();
    EXPECT_EQ(static_cast<float>(CLA_SQRT1_2), q1["x"].toFloat());
    EXPECT_EQ(0.0f, q1["y"].toFloat());
    EXPECT_EQ(0.0f, q1["z"].toFloat());
    EXPECT_EQ(static_cast<float>(CLA_SQRT1_2), q1["w"].toFloat());

    const CALUMI::Utilities::JsonObject qP;
    CALUMI::Utilities::JsonArray q01Array;

    q01Array.push_back(q0);
    q01Array.push_back(q1);
    q01Array.push_back(100);

    qP["-"] = q01Array;

    std::cout << qP.serialize(0, true).c_str() << std::endl;
}