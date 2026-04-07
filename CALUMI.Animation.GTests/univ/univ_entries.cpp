#include <gtest/gtest.h>
#include <AnimMath>
#include <AnimUniv>

using namespace CALUMI;

#define GTEST(x) TEST(UnivEntries, x)

GTEST(RotationSequence)
{
    UNIV::RotationSequence sq;

    sq.push_back(UNIV::Rotation(3, Math::Quaternion(0.0,0.0,0.0,1.0)));
    sq.push_back(UNIV::Rotation(0, Math::Quaternion(1.0,0.0,0.0,1.0)));
    sq.push_back(UNIV::Rotation(2, Math::Quaternion(0.0,1.0,0.0,1.0)));
    sq.push_back(UNIV::Rotation(1, Math::Quaternion(0.0,0.0,1.0,1.0)));

    UNIV::RotationSequence sqR = sq;

    EXPECT_EQ(sq.size(), 4);
    EXPECT_EQ(sq.at(0).frame(),3);
    EXPECT_EQ(sq.at(1).frame(),0);
    EXPECT_EQ(sq.at(2).frame(),2);
    EXPECT_EQ(sq.at(3).frame(),1);


    UNIV::SortRotationSequence(sq);
    UNIV::SortRotationSequence(sqR, true);

    EXPECT_EQ(sq.size(), 4);
    EXPECT_EQ(sq.at(0).frame(),0);
    EXPECT_TRUE(sq.at(0).rotationQuaternion().areEqual(Math::Quaternion(1.0,0.0,0.0,1.0),0.000001f));
    EXPECT_EQ(sq.at(1).frame(),1);
    EXPECT_TRUE(sq.at(1).rotationQuaternion().areEqual(Math::Quaternion(0.0,0.0,1.0,1.0),0.000001f));
    EXPECT_EQ(sq.at(2).frame(),2);
    EXPECT_TRUE(sq.at(2).rotationQuaternion().areEqual(Math::Quaternion(0.0,1.0,0.0,1.0),0.000001f));
    EXPECT_EQ(sq.at(3).frame(),3);
    EXPECT_TRUE(sq.at(3).rotationQuaternion().areEqual(Math::Quaternion(0.0,0.0,0.0,1.0),0.000001f));

    EXPECT_EQ(sqR.size(), 4);
    EXPECT_EQ(sqR.at(3).frame(),0);
    EXPECT_TRUE(sqR.at(3).rotationQuaternion().areEqual(Math::Quaternion(1.0,0.0,0.0,1.0),0.000001f));
    EXPECT_EQ(sqR.at(2).frame(),1);
    EXPECT_TRUE(sqR.at(2).rotationQuaternion().areEqual(Math::Quaternion(0.0,0.0,1.0,1.0),0.000001f));
    EXPECT_EQ(sqR.at(1).frame(),2);
    EXPECT_TRUE(sqR.at(1).rotationQuaternion().areEqual(Math::Quaternion(0.0,1.0,0.0,1.0),0.000001f));
    EXPECT_EQ(sqR.at(0).frame(),3);
    EXPECT_TRUE(sqR.at(0).rotationQuaternion().areEqual(Math::Quaternion(0.0,0.0,0.0,1.0),0.000001f));
}

GTEST(TranslationSequence)
{
    UNIV::TranslationSequence sq;

    sq.push_back(UNIV::Translation(3, Math::Vector3D(3.0,3.0,3.0)));
    sq.push_back(UNIV::Translation(0, Math::Vector3D(0.0,0.0,0.0)));
    sq.push_back(UNIV::Translation(2, Math::Vector3D(2.0,2.0,2.0)));
    sq.push_back(UNIV::Translation(1, Math::Vector3D(1.0,1.0,1.0)));

    UNIV::TranslationSequence sqR = sq;

    EXPECT_EQ(sq.size(), 4);
    EXPECT_EQ(sq.at(0).frame(),3);
    EXPECT_EQ(sq.at(1).frame(),0);
    EXPECT_EQ(sq.at(2).frame(),2);
    EXPECT_EQ(sq.at(3).frame(),1);


    UNIV::SortTranslationSequence(sq);
    UNIV::SortTranslationSequence(sqR, true);

    EXPECT_EQ(sq.size(), 4);
    EXPECT_EQ(sq.at(0).frame(),0);
    EXPECT_EQ(sq.at(0).translationVector(), Math::Vector3D(0.0,0.0,0.0));
    EXPECT_EQ(sq.at(1).frame(),1);
    EXPECT_EQ(sq.at(1).translationVector(), Math::Vector3D(1.0,1.0,1.0));
    EXPECT_EQ(sq.at(2).frame(),2);
    EXPECT_EQ(sq.at(2).translationVector(), Math::Vector3D(2.0,2.0,2.0));
    EXPECT_EQ(sq.at(3).frame(),3);
    EXPECT_EQ(sq.at(3).translationVector(), Math::Vector3D(3.0,3.0,3.0));

    EXPECT_EQ(sqR.size(), 4);
    EXPECT_EQ(sqR.at(3).frame(),0);
    EXPECT_EQ(sqR.at(3).translationVector(), Math::Vector3D(0.0,0.0,0.0));
    EXPECT_EQ(sqR.at(2).frame(),1);
    EXPECT_EQ(sqR.at(2).translationVector(), Math::Vector3D(1.0,1.0,1.0));
    EXPECT_EQ(sqR.at(1).frame(),2);
    EXPECT_EQ(sqR.at(1).translationVector(), Math::Vector3D(2.0,2.0,2.0));
    EXPECT_EQ(sqR.at(0).frame(),3);
    EXPECT_EQ(sqR.at(0).translationVector(), Math::Vector3D(3.0,3.0,3.0));
}

GTEST(ScalarSequence)
{
    UNIV::ScalarSequence sq;

    sq.push_back(UNIV::Scalar(3, 3.0f));
    sq.push_back(UNIV::Scalar(0, 0.0f));
    sq.push_back(UNIV::Scalar(2, 2.0f));
    sq.push_back(UNIV::Scalar(1, 1.0f));

    UNIV::ScalarSequence sqR = sq;

    EXPECT_EQ(sq.size(), 4);
    EXPECT_EQ(sq.at(0).frame(),3);
    EXPECT_EQ(sq.at(1).frame(),0);
    EXPECT_EQ(sq.at(2).frame(),2);
    EXPECT_EQ(sq.at(3).frame(),1);


    UNIV::SortScalarSequence(sq);
    UNIV::SortScalarSequence(sqR, true);

    EXPECT_EQ(sq.size(), 4);
    EXPECT_EQ(sq.at(0).frame(),0);
    EXPECT_EQ(sq.at(0).scalarValue(), 0.0f);
    EXPECT_EQ(sq.at(1).frame(),1);
    EXPECT_EQ(sq.at(1).scalarValue(), 1.0f);
    EXPECT_EQ(sq.at(2).frame(),2);
    EXPECT_EQ(sq.at(2).scalarValue(), 2.0f);
    EXPECT_EQ(sq.at(3).frame(),3);
    EXPECT_EQ(sq.at(3).scalarValue(), 3.0f);

    EXPECT_EQ(sqR.size(), 4);
    EXPECT_EQ(sqR.at(3).frame(),0);
    EXPECT_EQ(sqR.at(3).scalarValue(), 0.0f);
    EXPECT_EQ(sqR.at(2).frame(),1);
    EXPECT_EQ(sqR.at(2).scalarValue(), 1.0f);
    EXPECT_EQ(sqR.at(1).frame(),2);
    EXPECT_EQ(sqR.at(1).scalarValue(), 2.0f);
    EXPECT_EQ(sqR.at(0).frame(),3);
    EXPECT_EQ(sqR.at(0).scalarValue(), 3.0f);
}

GTEST(PrioritySequence)
{
    UNIV::PrioritySequence sq;

    sq.push_back(UNIV::Priority(3, 30));
    sq.push_back(UNIV::Priority(0, 0));
    sq.push_back(UNIV::Priority(2, 20));
    sq.push_back(UNIV::Priority(1, 10));

    UNIV::PrioritySequence sqR = sq;

    EXPECT_EQ(sq.size(), 4);
    EXPECT_EQ(sq.at(0).frame(),3);
    EXPECT_EQ(sq.at(1).frame(),0);
    EXPECT_EQ(sq.at(2).frame(),2);
    EXPECT_EQ(sq.at(3).frame(),1);


    UNIV::SortPrioritySequence(sq);
    UNIV::SortPrioritySequence(sqR, true);

    EXPECT_EQ(sq.size(), 4);
    EXPECT_EQ(sq.at(0).frame(),0);
    EXPECT_EQ(sq.at(0).priorityValue(), 0);
    EXPECT_EQ(sq.at(1).frame(),1);
    EXPECT_EQ(sq.at(1).priorityValue(), 10);
    EXPECT_EQ(sq.at(2).frame(),2);
    EXPECT_EQ(sq.at(2).priorityValue(), 20);
    EXPECT_EQ(sq.at(3).frame(),3);
    EXPECT_EQ(sq.at(3).priorityValue(), 30);

    EXPECT_EQ(sqR.size(), 4);
    EXPECT_EQ(sqR.at(3).frame(),0);
    EXPECT_EQ(sqR.at(3).priorityValue(), 0);
    EXPECT_EQ(sqR.at(2).frame(),1);
    EXPECT_EQ(sqR.at(2).priorityValue(), 10);
    EXPECT_EQ(sqR.at(1).frame(),2);
    EXPECT_EQ(sqR.at(1).priorityValue(), 20);
    EXPECT_EQ(sqR.at(0).frame(),3);
    EXPECT_EQ(sqR.at(0).priorityValue(), 30);
}