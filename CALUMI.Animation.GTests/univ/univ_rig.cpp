#include <gtest/gtest.h>
#include <AnimUniv>
#include "../Common.h"

using namespace CALUMI;

#define GTEST(x) TEST(UnivRigInterface, x)

GTEST(UnivRigCreation)
{
    const UNIV::SkeletonRig rig("MyTestRig");

    EXPECT_STREQ("MyTestRig", rig.name());

    const auto root  = rig.root();

    EXPECT_STRCASEEQ(root->name(), "root");
    root->setLocalTransform(
        Math::Vector3(0.0f,0.0f,0.0f),
        {0.0f,0.0f,static_cast<float>(Math::ToRadians(90.0f)), Math::EulerDefinition::EulerOrder::XYZ}
        );

    const UNIV::SkeletonBone* child01 = root->addChildBone(
                                                     "child01",
                                                     Math::Vector3(0.0f, 0.0f, 1.0f),
                                                     Math::Quaternion(
                                                       Math::EulerDefinition(0.0f,
                                                                             0.0f,
                                                                             static_cast<float>(Math::ToRadians(30.0f)),
                                                                             Math::EulerDefinition::EulerOrder::XYZ)));

    EXPECT_EQ(child01, rig.bone(child01->name()));
    EXPECT_EQ(root, rig.bone(root->name()));

    EXPECT_EQ(rig.boneCount(), 2);
    EXPECT_QUATNEAR(child01->globalTransform().rotation(),Math::Quaternion(0.0f,0.0f,0.866025f,0.5f),0.000001f);
    EXPECT_QUATNEAR(child01->localTransform().rotation(),Math::Quaternion(0.0f,0.0f,0.258819f,0.965926f),0.000001f);
    EXPECT_QUATNEAR(root->globalTransform().rotation(),Math::Quaternion(0.0f,0.0f,0.707107f,0.707107f),0.000001f);
    EXPECT_QUATNEAR(root->localTransform().rotation(),Math::Quaternion(0.0f,0.0f,0.707107f,0.707107f),0.000001f);

    //Attempting unallowed duplicate
    const auto rootDup = child01->addChildBone(root->name(), Math::Vector3(), Math::Quaternion());
    EXPECT_FALSE(rootDup);

    //Checking if duplicate affected current line up
    EXPECT_EQ(rig.boneCount(), 2);
    EXPECT_QUATNEAR(child01->globalTransform().rotation(),Math::Quaternion(0.0f,0.0f,0.866025f,0.5f),0.000001f);
    EXPECT_QUATNEAR(child01->localTransform().rotation(),Math::Quaternion(0.0f,0.0f,0.258819f,0.965926f),0.000001f);
    EXPECT_QUATNEAR(root->globalTransform().rotation(),Math::Quaternion(0.0f,0.0f,0.707107f,0.707107f),0.000001f);
    EXPECT_QUATNEAR(root->localTransform().rotation(),Math::Quaternion(0.0f,0.0f,0.707107f,0.707107f),0.000001f);

    UNIV::SkeletonBone::DeleteBone(child01);
    EXPECT_EQ(rig.boneCount(), 1);
    EXPECT_FALSE(child01);
}

GTEST(UnivRigCreationC)
{

}