#include <gtest/gtest.h>
#include <AnimMath>
#include <AnimUniv>
#include "../Common.h"

using namespace CALUMI;

#define GTEST(x) TEST(UnivRigInterface, x)

GTEST(UnivRigCreation)
{
    const UNIV::SkeletonRig rig("MyTestRig");

    EXPECT_STREQ("MyTestRig", rig.name());

    rig.addBoneToRig({0.0f,0.0f,static_cast<float>(Math::ToRadians(90.0f)),Math::Quaternion::EulerOrder::XYZ},Math::Vector3(0.0f,0.0f,0.0f),"root","",true);
    rig.addBoneToRig({0.0f,0.0f,static_cast<float>(Math::ToRadians(30.0f)),Math::Quaternion::EulerOrder::XYZ},Math::Vector3(0.0f,0.0f,1.0f),"child01","root",true);

    EXPECT_STRCASEEQ(rig.root().name(), "root");
    rig.root().setLocalTransform(
        Math::Vector3(0.0f,0.0f,0.0f),
        {0.0f,0.0f,static_cast<float>(Math::ToRadians(90.0f)), Math::EulerDefinition::EulerOrder::XYZ}
        );



    EXPECT_EQ(rig.boneCount(), 2);
    EXPECT_QUATNEAR(rig.boneRotation("child01",false),Math::Quaternion(0.0f,0.0f,0.866025f,0.5f),0.000001f);
    EXPECT_QUATNEAR(rig.boneRotation("child01",true),Math::Quaternion(0.0f,0.0f,0.258819f,0.965926f),0.000001f);
    EXPECT_QUATNEAR(rig.boneRotation("root",false),Math::Quaternion(0.0f,0.0f,0.707107f,0.707107f),0.000001f);
    EXPECT_QUATNEAR(rig.boneRotation("root",true),Math::Quaternion(0.0f,0.0f,0.707107f,0.707107f),0.000001f);

    //Attempting unallowed duplicate
    const auto rootDup = rig.addBoneToRig({0.0f,0.0f,0.0f,Math::Quaternion::EulerOrder::XYZ},Math::Vector3(0.0f,0.0f,0.0f),"root","",true);
    EXPECT_FALSE(rootDup);

    //Checking if duplicate affected current line up
    EXPECT_EQ(rig.boneCount(), 2);
    EXPECT_QUATNEAR(rig.boneRotation("child01",false),Math::Quaternion(0.0f,0.0f,0.866025f,0.5f),0.000001f);
    EXPECT_QUATNEAR(rig.boneRotation("child01",true),Math::Quaternion(0.0f,0.0f,0.258819f,0.965926f),0.000001f);
    EXPECT_QUATNEAR(rig.boneRotation("root",false),Math::Quaternion(0.0f,0.0f,0.707107f,0.707107f),0.000001f);
    EXPECT_QUATNEAR(rig.boneRotation("root",true),Math::Quaternion(0.0f,0.0f,0.707107f,0.707107f),0.000001f);

    //renaming root and adding new root
    EXPECT_TRUE(rig.renameBone("root", "child00"));
    EXPECT_TRUE(rig.addBoneToRig({0.0f,0.0f,static_cast<float>(Math::ToRadians(10.0f)),Math::Quaternion::EulerOrder::XYZ},Math::Vector3(0.0f,0.0f,0.0f),"root","",true));
    EXPECT_EQ(rig.boneCount(), 3);

    EXPECT_TRUE(rig.setRoot("root"));
    EXPECT_TRUE(rig.setBoneParent("child00","root"));
    {
        const auto bone = rig.bone("child01");
        EXPECT_TRUE(bone);
        if (bone)
            EXPECT_TRUE(std::string(bone->parentBone()) == "child00");
    }
    {
        const auto bone = rig.bone("child00");
        EXPECT_TRUE(bone);
        if (bone)
            EXPECT_TRUE(std::string(bone->parentBone()) == "root");
    }
    //Check to make sure list is still intact
    EXPECT_EQ(rig.boneCount(), 3);
    EXPECT_QUATNEAR(rig.boneRotation("child01",false),Math::Quaternion(0.0f,0.0f,0.906308f,0.422618f),0.000001f);
    EXPECT_QUATNEAR(rig.boneRotation("child01",true),Math::Quaternion(0.0f,0.0f,0.258819f,0.965926f),0.000001f);
    EXPECT_QUATNEAR(rig.boneRotation("child00",false),Math::Quaternion(0.0f,0.0f,0.766044f,0.642788f),0.000001f);
    EXPECT_QUATNEAR(rig.boneRotation("child00",true),Math::Quaternion(0.0f,0.0f,0.707107f,0.707107f),0.000001f);
    EXPECT_QUATNEAR(rig.boneRotation("root",false),Math::Quaternion(0.0f,0.0f,0.087156f,0.996195f),0.000001f);
    EXPECT_QUATNEAR(rig.boneRotation("root",true),Math::Quaternion(0.0f,0.0f,0.087156f,0.996195f),0.000001f);

    EXPECT_TRUE(rig.addBoneToRig({0.0f,0.0f,static_cast<float>(Math::ToRadians(180.0f)),Math::Quaternion::EulerOrder::XYZ},Math::Vector3(0.0f,0.0f,0.0f),"base","",true));

    Math::Quaternion expChild01 = rig.boneRotation("child01",false);
    expChild01.rotateBy(Math::Quaternion(0.0f,0.0f,1.0f,0.0f));
    Math::Quaternion expChild00 = rig.boneRotation("child00",false);
    expChild00.rotateBy(Math::Quaternion(0.0f,0.0f,1.0f,0.0f));
    Math::Quaternion expChild000 = rig.boneRotation("root",false);
    expChild000.rotateBy(Math::Quaternion(0.0f,0.0f,1.0f,0.0f));

    EXPECT_TRUE(rig.setRoot("base"));
    EXPECT_TRUE(rig.setBoneParent("root","base"));
    EXPECT_TRUE(rig.renameBone("root","child000"));

    EXPECT_EQ(rig.boneCount(), 4);
    EXPECT_QUATNEAR(rig.boneRotation("child01",false),expChild01,0.000001f);
    EXPECT_QUATNEAR(rig.boneRotation("child01",true),Math::Quaternion(0.0f,0.0f,0.258819f,0.965926f),0.000001f);

    EXPECT_QUATNEAR(rig.boneRotation("child00",false),expChild00,0.000001f);
    EXPECT_QUATNEAR(rig.boneRotation("child00",true),Math::Quaternion(0.0f,0.0f,0.707107f,0.707107f),0.000001f);

    EXPECT_QUATNEAR(rig.boneRotation("child000",false),expChild000,0.000001f);
    EXPECT_QUATNEAR(rig.boneRotation("child000",true),Math::Quaternion(0.0f,0.0f,0.087156f,0.996195f),0.000001f);

    EXPECT_QUATNEAR(rig.boneRotation("base",false),Math::Quaternion(0.0f,0.0f,1.0f,0.0f),0.000001f);
    EXPECT_QUATNEAR(rig.boneRotation("base",true),Math::Quaternion(0.0f,0.0f,1.0f,0.0f),0.000001f);
}

GTEST(UnivRigCreationC)
{

}