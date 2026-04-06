#include <gtest/gtest.h>
#include <AnimMath>
#include <AnimUniv>

#define GTEST(x) TEST(SequenceCompression, x)

#define ROTATIONENTRY(input, f, x,y,z,deg) {CALUMI::Math::Quaternion rotation(Vector3(x, y, z), ToRadians(deg)); uint16_t frame = f; \
CALUMI::UNIV::Rotation entry(frame,rotation);\
    input.AddRotationEntry(entry);}

#define TRANSLATIONENTRY(input, f, x,y,z) { CALUMI::Math::Vector3D vec(x,y,z); uint16_t frame = f; \
CALUMI::UNIV::Translation entry(frame,vec); \
    input.AddTranslationEntry(entry); }

#define SCALARENTRY(input, f, s) { uint16_t frame = f; float scalar = s; \
CALUMI::UNIV::Scalar entry(frame,scalar);\
    input.AddScalarEntry(entry); }

using namespace CALUMI::Math;

GTEST(RDP_Rotation)
{
    CALUMI::UNIV::AnimationBlock input1;

    //source sq input
    ROTATIONENTRY(input1, 0, 1.0f, 0.0f, 0.0f, 0.0);
    ROTATIONENTRY(input1, 1, 1.0f, 0.0f, 0.0f, 0.0); //should be culled
    ROTATIONENTRY(input1, 2, 1.0f, 0.0f, 0.0f, 0.0); //""
    ROTATIONENTRY(input1, 3, 1.0f, 0.0f, 0.0f, 0.0); //""
    ROTATIONENTRY(input1, 4, 1.0f, 0.0f, 0.0f, 0.0); //""
    ROTATIONENTRY(input1, 5, 1.0f, 0.0f, 0.0f, 0.0);

    auto first1 = input1.rotationSequence().at(0);
    auto last1 = input1.rotationSequence().at(input1.rotationSequence().size() - 1);

    input1.ExecuteRDPReduction_Rotation();

    EXPECT_TRUE(input1.rotationSequence().size() == 2 && first1.IsIdentical(input1.rotationSequence().at(0)) && last1.IsIdentical(input1.rotationSequence().at(input1.rotationSequence().size()-1)));


    //GTEST(RDP_Rotation2)
    CALUMI::UNIV::AnimationBlock input2;

    //source sq input
    ROTATIONENTRY(input2, 0, 1.0f, 0.0f, 0.0f, 0.0);
    ROTATIONENTRY(input2, 1, 1.0f, 0.0f, 0.0f, 0.0);
    ROTATIONENTRY(input2, 2, 1.0f, 0.0f, 0.0f, 90.0);
    ROTATIONENTRY(input2, 3, 1.0f, 0.0f, 0.0f, 0.0);
    ROTATIONENTRY(input2, 4, 1.0f, 0.0f, 0.0f, 0.0); //should be culled
    ROTATIONENTRY(input2, 5, 1.0f, 0.0f, 0.0f, 0.0);

    auto first2 = input2.rotationSequence().at(0);
    auto last2 = input2.rotationSequence().at(input2.rotationSequence().size() - 1);

    input2.ExecuteRDPReduction_Rotation();

    EXPECT_TRUE(input2.rotationSequence().size() == 5 && first2.IsIdentical(input2.rotationSequence().at(0)) && last2.IsIdentical(input2.rotationSequence().at(input2.rotationSequence().size() - 1)));


    //GTEST(RDP_Rotation3)
    CALUMI::UNIV::AnimationBlock input3;

    //source sq input
    ROTATIONENTRY(input3, 0, 1.0f, 0.0f, 0.0f, 0.0);
    ROTATIONENTRY(input3, 1, 1.0f, 0.0f, 0.0f, 90.0);  //Should NOT be culled
    ROTATIONENTRY(input3, 2, 1.0f, 0.0f, 0.0f, 180.0);
    ROTATIONENTRY(input3, 3, 1.0f, 0.0f, 0.0f, 120.0);
    ROTATIONENTRY(input3, 4, 1.0f, 0.0f, 0.0f, 60.0); //should be culled
    ROTATIONENTRY(input3, 5, 1.0f, 0.0f, 0.0f, 0.0);

    auto first3 = input3.rotationSequence().at(0);
    auto last3 = input3.rotationSequence().at(input3.rotationSequence().size() - 1);

    input3.ExecuteRDPReduction_Rotation();

    EXPECT_TRUE(input3.rotationSequence().size() == 5 && first3.IsIdentical(input3.rotationSequence().at(0)) && last3.IsIdentical(input3.rotationSequence().at(input3.rotationSequence().size() - 1)));


    //GTEST(RDP_Rotation4)
    CALUMI::UNIV::AnimationBlock input4;

    //source sq input
    ROTATIONENTRY(input4, 0, 1.0f, 0.0f, 0.0f, 0.0);
    ROTATIONENTRY(input4, 1, 1.0f, 0.0f, 0.0f, 60.0);  //Should NOT be culled
    ROTATIONENTRY(input4, 3, 1.0f, 0.0f, 0.0f, 180.0);

    auto first4 = input4.rotationSequence().at(0);
    auto last4 = input4.rotationSequence().at(input4.rotationSequence().size() - 1);

    input4.ExecuteRDPReduction_Rotation();

    EXPECT_TRUE(input4.rotationSequence().size() == 3 && first4.IsIdentical(input4.rotationSequence().at(0)) && last4.IsIdentical(input4.rotationSequence().at(input4.rotationSequence().size() - 1)));


    //GTEST(RDP_Rotation5)
    CALUMI::UNIV::AnimationBlock input5;

    //source sq input
    ROTATIONENTRY(input5, 0, 1.0f, 0.0f, 0.0f, 0.0);
    ROTATIONENTRY(input5, 1, 1.0f, 0.0f, 0.0f, 89.95);  //Should be culled
    ROTATIONENTRY(input5, 2, 1.0f, 0.0f, 0.0f, 179.9);

    auto first5 = input5.rotationSequence().at(0);
    auto last5 = input5.rotationSequence().at(input5.rotationSequence().size() - 1);

    input5.ExecuteRDPReduction_Rotation();

    EXPECT_TRUE(input5.rotationSequence().size() == 2 && first5.IsIdentical(input5.rotationSequence().at(0)) && last5.IsIdentical(input5.rotationSequence().at(input5.rotationSequence().size() - 1)));


    //GTEST(RDP_Rotation6)
    CALUMI::UNIV::AnimationBlock input6;

    //source sq input
    ROTATIONENTRY(input6, 0, 1.0f, 0.0f, 0.0f, 0.0);
    ROTATIONENTRY(input6, 2, 1.0f, 0.0f, 0.0f, 90.0);
    ROTATIONENTRY(input6, 4, 1.0f, 0.0f, 0.0f, 180.0);
    ROTATIONENTRY(input6, 6, 1.0f, 0.0f, 0.0f, 270.0);
    ROTATIONENTRY(input6, 8, 1.0f, 0.0f, 0.0f, 360.0);

    auto first6 = input6.rotationSequence().at(0);
    auto last6 = input6.rotationSequence().at(input6.rotationSequence().size() - 1);

    input6.ExecuteRDPReduction_Rotation();

    EXPECT_TRUE(input6.rotationSequence().size() == 5 && first6.IsIdentical(input6.rotationSequence().at(0)) && last6.IsIdentical(input6.rotationSequence().at(input6.rotationSequence().size() - 1)));


    //GTEST(RDP_Rotation7)
    CALUMI::UNIV::AnimationBlock input7;

    //source sq input
    ROTATIONENTRY(input7, 0, 1.0f, 0.0f, 0.0f, 0.0);
    ROTATIONENTRY(input7, 1, 1.0f, 0.0f, 0.0f, 45.0); //culled
    ROTATIONENTRY(input7, 2, 1.0f, 0.0f, 0.0f, 90.0);
    ROTATIONENTRY(input7, 3, 1.0f, 0.0f, 0.0f, 135.0); //culled
    ROTATIONENTRY(input7, 4, 1.0f, 0.0f, 0.0f, 180.0);
    ROTATIONENTRY(input7, 5, 1.0f, 0.0f, 0.0f, 225.0); //culled
    ROTATIONENTRY(input7, 6, 1.0f, 0.0f, 0.0f, 270.0);
    ROTATIONENTRY(input7, 7, 1.0f, 0.0f, 0.0f, 315.0); //culled
    ROTATIONENTRY(input7, 8, 1.0f, 0.0f, 0.0f, 360.0);

    auto first7 = input7.rotationSequence().at(0);
    auto last7 = input7.rotationSequence().at(input7.rotationSequence().size() - 1);

    input7.ExecuteRDPReduction_Rotation();

    EXPECT_TRUE(input7.rotationSequence().size() == 5 && first7.IsIdentical(input7.rotationSequence().at(0)) && last7.IsIdentical(input7.rotationSequence().at(input7.rotationSequence().size() - 1)));


    //GTEST(RDP_Rotation8)
    CALUMI::UNIV::AnimationBlock input8;

    //source sq input
    ROTATIONENTRY(input8, 3, 1.0f, 0.0f, 0.0f, 0.0);
    ROTATIONENTRY(input8, 4, 1.0f, 0.0f, 0.0f, 360.0 * 1 / 7); //culled
    ROTATIONENTRY(input8, 5, 1.0f, 0.0f, 0.0f, 360.0 * 2 / 7);
    ROTATIONENTRY(input8, 6, 1.0f, 0.0f, 0.0f, 360.0 * 3 / 7); //culled
    ROTATIONENTRY(input8, 7, 1.0f, 0.0f, 0.0f, 360.0 * 4 / 7);
    ROTATIONENTRY(input8, 8, 1.0f, 0.0f, 0.0f, 360.0 * 5 / 7); //culled
    ROTATIONENTRY(input8, 9, 1.0f, 0.0f, 0.0f, 360.0 * 6 / 7); //culled
    ROTATIONENTRY(input8,10, 1.0f, 0.0f, 0.0f, 360.0);
    ROTATIONENTRY(input8,11, 1.0f, 0.0f, 0.0f, 360.0); //culled
    ROTATIONENTRY(input8,12, 1.0f, 0.0f, 0.0f, 360.0);
    ROTATIONENTRY(input8,13, 1.0f, 0.0f, 0.0f, 361.0);
    ROTATIONENTRY(input8,14, 1.0f, 0.0f, 0.0f, 360.666666667); //culled
    ROTATIONENTRY(input8,15, 1.0f, 0.0f, 0.0f, 360.333333333); //culled
    ROTATIONENTRY(input8,16, 1.0f, 0.0f, 0.0f, 360.0);

    auto first8 = input8.rotationSequence().at(0);
    auto last8 = input8.rotationSequence().at(input8.rotationSequence().size() - 1);

    input8.ExecuteRDPReduction_Rotation();

    EXPECT_TRUE(input8.rotationSequence().size() == 7 && first8.IsIdentical(input8.rotationSequence().at(0)) && last8.IsIdentical(input8.rotationSequence().at(input8.rotationSequence().size() - 1)));
}

GTEST(RDP_Translation)
{
    CALUMI::UNIV::AnimationBlock block1;

    TRANSLATIONENTRY(block1, 0, 0.0, 0.0, 0.0);
    TRANSLATIONENTRY(block1, 1, 0.0, 0.0, 0.0);
    TRANSLATIONENTRY(block1, 2, 0.0, 0.0, 0.0);
    TRANSLATIONENTRY(block1, 3, 0.0, 0.0, 0.0);

    auto first1 = block1.translationSequence().at(0);
    auto last1 = block1.translationSequence().at(block1.translationSequence().size() - 1);

    block1.ExecuteRDPReduction_Translation();

    EXPECT_TRUE(block1.translationSequence().size() == 2 && first1.IsIdentical(block1.translationSequence().at(0)) && last1.IsIdentical(block1.translationSequence().at(block1.translationSequence().size()-1)));


    //GTEST(RDP_2)
    CALUMI::UNIV::AnimationBlock block2;

    TRANSLATIONENTRY(block2, 0, -2.0, -0.2, -1.0);
    TRANSLATIONENTRY(block2, 3, 1.0, 0.1, 0.5);
    TRANSLATIONENTRY(block2, 4, 2.0, 0.2, 1.0);
    TRANSLATIONENTRY(block2, 5, 3.0, 0.3, 1.5);

    auto first2 = block2.translationSequence().at(0);
    auto last2 = block2.translationSequence().at(block2.translationSequence().size() - 1);

    block2.ExecuteRDPReduction_Translation();

    EXPECT_TRUE(block2.translationSequence().size() == 2 && first2.IsIdentical(block2.translationSequence().at(0)) && last2.IsIdentical(block2.translationSequence().at(block2.translationSequence().size() - 1)));
}

GTEST(RDP_Scalar)
{
    CALUMI::UNIV::AnimationBlock input1;

    //source sq input
    SCALARENTRY(input1, 0, 1.0f);
    SCALARENTRY(input1, 1, 0.5f);
    SCALARENTRY(input1, 2, 0.75f);
    SCALARENTRY(input1, 3, 1.0f);
    SCALARENTRY(input1, 6, 1.0f);
    SCALARENTRY(input1, 8, 1.0f);
    SCALARENTRY(input1, 9, 1.0f);
    SCALARENTRY(input1, 10, 1.0f);

    auto first1 = input1.scalarSequence().at(0);
    auto last1 = input1.scalarSequence().at(input1.scalarSequence().size() - 1);

    input1.ExecuteRDPReduction_Scalar();

    EXPECT_TRUE(input1.scalarSequence().size() == 4 && first1.IsIdentical(input1.scalarSequence().at(0)) && last1.IsIdentical(input1.scalarSequence().at(input1.scalarSequence().size() - 1)));
}
