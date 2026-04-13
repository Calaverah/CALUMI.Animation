// ReSharper disable CppUnusedIncludeDirective
// ReSharper disable CppTooWideScope
// ReSharper disable CppDFATimeOver
// ReSharper disable CppTemplateArgumentsCanBeDeduced
#include <gtest/gtest.h>
#include <iostream>
#include <AnimMath>
#include <AnimUniv>
#include <AnimStarfield>

using namespace CALUMI;

#define GTEST(x) TEST(MockStarfield, x)

GTEST(Rig00)
{
	std::vector<char> stdEOH = { '\0','\0','C','A','L','U','M','I','D','V','R','S','J','O','J','O' };
	SFBGS::SkeletonRig rig00;

    auto result00 = rig00.readFromFile("assets/00/skeleton.rig");

	//Read Check
	EXPECT_TRUE(!result00.hasError());

	if (result00.hasError())
	{
		ADD_FAILURE() << "Error Found In File Read -> " << result00.toString().c_str();
	}

	//File version
    EXPECT_EQ(rig00.versionNumber(), 5);

	//File Size
    EXPECT_EQ(rig00.fileSize(), 720);

	//Header Size
    EXPECT_EQ(rig00.headerSize(), 0x50);

	//Map Offset
    EXPECT_EQ(rig00.boneMapOffset(), 368);

	//Bone Counts
    EXPECT_EQ(rig00.boneCount(), 3);
    EXPECT_EQ(rig00.boneCountAnimated(), 3);
    EXPECT_EQ(rig00.boneEntries().size(), rig00.boneCount());

    EXPECT_TRUE(rig00.checkAssumedHeaderEntries() == 0);

#ifdef DEBUG_BUILD
	{
		//End Of Header
        auto rig00EOH = rig00.endOfHeader();

		EXPECT_EQ(rig00EOH.size(), stdEOH.size());
		if (rig00EOH.size() == stdEOH.size())
		{
			for (size_t i = 0; i < rig00EOH.size(); i++)
			{
				EXPECT_EQ(rig00EOH.at(i), stdEOH.at(i));
			}
		}
	}
#endif


	//Precision
    EXPECT_EQ(rig00.highPrecision(), 1.0f / 4000.0f);
    EXPECT_EQ(rig00.lowPrecision(), 1.0f / 32.000f);

	//Matching 3
	auto match00 = rig00.matchingThree();
	EXPECT_EQ(match00.size(), 3);
	for (uint8_t i = 0; i < match00.size() && i < 3; i++)
	{
		EXPECT_EQ(match00.at(i), 4587439358609565533);
	}


	//Bone Entries
	float unk00Floats[3] = { 0.1f, 0.2f, 60.0f };
	Math::Quaternion q00Locals[3] = { Math::Quaternion(-0.0f,-0.0f,-0.707107f,0.707107f), Math::Quaternion(), Math::Quaternion() };

    auto& rig00Bones = rig00.boneEntries();
    EXPECT_EQ(rig00Bones.size(), rig00.boneCount());

    auto& str00 = rig00.stringArray();
	EXPECT_EQ(rig00Bones.size(), str00.size());

	uint64_t offsetSum00 = 0;

	if (!rig00Bones.empty())
		offsetSum00 = rig00Bones.at(0).getNameOffset();

	float twistW00[3] = { 0.0f,0.0f,0.67f };


	for (uint8_t i = 0; i < rig00Bones.size() && i < str00.size(); i++)
	{
		//Rotations
        EXPECT_TRUE(rig00Bones.at(i).localRotation().areEqual(q00Locals[i], 0.000001f)) << "Tested " << rig00Bones.at(i).localRotation().toString().c_str() << " \nExpected " << q00Locals[i].toString().c_str();
        EXPECT_TRUE(rig00Bones.at(i).globalRotation().areEqual(q00Locals[0], 0.000001f)) << "Tested " << rig00Bones.at(i).localRotation().toString().c_str() << " \nExpected " << q00Locals[i].toString().c_str();

		//Unks
		EXPECT_EQ(rig00Bones.at(i).getUnknownScalar(), unk00Floats[i]);
		EXPECT_EQ(rig00Bones.at(i).getTerm05(), 4);

		//Bone Info
		EXPECT_EQ(rig00Bones.at(i).getBoneType(), SFBGS::SkeletonBone::BoneType::Default);
		EXPECT_EQ(rig00Bones.at(i).getParentBoneIndex() + 1, i);
		EXPECT_EQ(rig00Bones.at(i).getTwistDriverMqnIndex(), -1);
		EXPECT_EQ(rig00Bones.at(i).getTwistDriverIndex(), -1);
		EXPECT_EQ(rig00Bones.at(i).getTwistDriverWeight(), twistW00[i]);
		EXPECT_EQ(rig00Bones.at(i).getMirrorBoneIndex(), i);

		//Padding
#ifdef DEBUG_BUILD
        EXPECT_EQ(rig00Bones.at(i).getPad01(), -1);
		EXPECT_EQ(rig00Bones.at(i).getPad02(), 0);
#endif
		//Strings
		EXPECT_EQ(offsetSum00, rig00Bones.at(i).getNameOffset());
		offsetSum00 += str00.stringLength(i, true);
	}

	//Bone Names
	if (str00.size() == 3)
	{
        EXPECT_EQ(std::string(str00.c_str(0)), "Fountain_Root");
        EXPECT_EQ(std::string(str00.c_str(1)), "PlanetObject");
        EXPECT_EQ(std::string(str00.c_str(2)), "RingObject");
	} else
    {
        ADD_FAILURE() << "Bone Name Array Size Does Not Match";
    }

	//Bone Map
	uint8_t boneMapSize = 157;
    EXPECT_EQ(rig00.boneMapArray().size(), boneMapSize);
    if (rig00.boneMapArray().size() == boneMapSize)
	{
        for (size_t i = 0; i < rig00.boneMapArray().size(); i++)
		{
			short mapVal = -1;

			if (i == 0)
				mapVal = 0;
			else if (i == 8)
				mapVal = 1;
			else
				mapVal = -1;

            EXPECT_EQ(rig00.boneMapArray().at(i), mapVal);
		}
	}

	UNIV::SkeletonRig uRig00 = rig00.convertToUniversalRig();
	SFBGS::SkeletonRig rig00COPY(uRig00);

    EXPECT_EQ(uRig00.boneEntries().size(), rig00.boneEntries().size());

    EXPECT_EQ(uRig00.boneCount(), rig00.boneCount());

#pragma region COPIED RIG00

	//File version
    EXPECT_EQ(rig00COPY.versionNumber(), rig00.versionNumber());

	//File Size
    EXPECT_EQ(rig00COPY.fileSize(), rig00.fileSize());

	//Header Size
    EXPECT_EQ(rig00COPY.headerSize(), rig00.headerSize());

	//Map Offset
    EXPECT_EQ(rig00COPY.boneMapOffset(), rig00.boneMapOffset());

	//Bone Counts
    EXPECT_EQ(rig00COPY.boneCount(), rig00.boneCount());
    EXPECT_EQ(rig00COPY.boneEntries().size(), rig00.boneEntries().size());
    EXPECT_EQ(rig00COPY.boneCountAnimated(), rig00.boneCountAnimated());

#ifdef DEBUG_BUILD
	{
		//End Of Header
        auto rig00EOH = rig00COPY.endOfHeader();

		EXPECT_EQ(rig00EOH.size(), stdEOH.size());
		if (rig00EOH.size() == stdEOH.size())
		{
			for (size_t i = 0; i < rig00EOH.size(); i++)
			{
				EXPECT_EQ(rig00EOH.at(i), stdEOH.at(i));
			}
		}
	}
#endif

	//Precision
    EXPECT_EQ(rig00.highPrecision(), rig00COPY.highPrecision());
    EXPECT_EQ(rig00.lowPrecision(), rig00COPY.lowPrecision());

    auto& rig00COPYBones = rig00COPY.boneEntries();
    EXPECT_EQ(rig00COPYBones.size(), rig00COPY.boneCount());

    auto& str00COPY = rig00COPY.stringArray();
	EXPECT_EQ(rig00COPYBones.size(), str00COPY.size());

	uint64_t offsetSum00COPY = 0;

	if (!rig00COPYBones.empty())
		//We use the source offset to begin, as we do not have that until export/serialization
		offsetSum00COPY = rig00Bones.at(0).getNameOffset();

	for (uint8_t i = 0; i < rig00COPYBones.size() && i < str00COPY.size(); i++)
	{
		//Rotations
        EXPECT_TRUE(rig00COPYBones.at(i).localRotation().areEqual(rig00Bones.at(i).localRotation(), 0.000001f)) << "Tested " << rig00COPYBones.at(i).localRotation().toString().c_str() << " \nExpected " << rig00Bones.at(i).localRotation().toString().c_str();
        EXPECT_TRUE(rig00COPYBones.at(i).globalRotation().areEqual(rig00Bones.at(i).globalRotation(), 0.000001f)) << "Tested " << rig00COPYBones.at(i).localRotation().toString().c_str() << " \nExpected " << rig00Bones.at(i).globalRotation().toString().c_str();

		//Unks
		//EXPECT_EQ(rig00COPYBones.at(i).getUnknownScalar(), unk00Floats[i]);
		//EXPECT_EQ(rig00COPYBones.at(i).getTerm05(), 4);

		//Bone Info
		EXPECT_EQ(rig00COPYBones.at(i).getBoneType(), rig00Bones.at(i).getBoneType());
		EXPECT_EQ(rig00COPYBones.at(i).getParentBoneIndex(), rig00Bones.at(i).getParentBoneIndex());
		EXPECT_EQ(rig00COPYBones.at(i).getTwistDriverMqnIndex(), rig00Bones.at(i).getTwistDriverMqnIndex());
		EXPECT_EQ(rig00COPYBones.at(i).getTwistDriverIndex(), -1); //These bones were set to default and should be different than the source on output as the values are corrected
		EXPECT_EQ(rig00COPYBones.at(i).getTwistDriverWeight(), 0.0f); //These bones were set to default and should be different than the source on output as the values are corrected
		EXPECT_EQ(rig00COPYBones.at(i).getMirrorBoneIndex(), rig00Bones.at(i).getMirrorBoneIndex());

		//Padding 
#ifdef DEBUG_BUILD
		EXPECT_EQ(rig00COPYBones.at(i).getPad01(), rig00Bones.at(i).getPad01());
		EXPECT_EQ(rig00COPYBones.at(i).getPad02(), rig00Bones.at(i).getPad02());
#endif
		//Strings
		//We use the source offset as the copy version isn't produced until export/serialization
		EXPECT_EQ(offsetSum00COPY, rig00Bones.at(i).getNameOffset());
		offsetSum00COPY += str00COPY.stringLength(i, true);
	}

	//Bone Names
	if (str00COPY.size() == 3)
	{
		EXPECT_EQ(std::string(str00COPY.c_str(0)), str00.c_str(0));
		EXPECT_EQ(std::string(str00COPY.c_str(1)), str00.c_str(1));
		EXPECT_EQ(std::string(str00COPY.c_str(2)), str00.c_str(2));
	}
	else
	{
		ADD_FAILURE() << "Copied Bone Name Array Size Does Not Match";
	}

	//Bone Map
    EXPECT_EQ(rig00.boneMapArray().size(), rig00COPY.boneMapArray().size());
    if (rig00.boneMapArray().size() == rig00COPY.boneMapArray().size())
	{
        for (size_t i = 0; i < rig00COPY.boneMapArray().size(); i++)
		{
            EXPECT_EQ(rig00COPY.boneMapArray().at(i), rig00.boneMapArray().at(i));
		}
	}

#pragma endregion

#pragma region UNIV Rig00

	//TODO: UNIV RIG00 Unit Tests

#pragma endregion

	
}

GTEST(ScratchAnimation)
{
	UNIV::Animation uAnim;
	uAnim.setAnimationTitle("TestAnim");

	//Sanity Checks
	EXPECT_STREQ(uAnim.animationTitle(), "TestAnim");
	EXPECT_EQ(uAnim.animationBlockCount(), 0);
	EXPECT_EQ(uAnim.animationBlockCount(), uAnim.animationBlocks().size());
	EXPECT_NO_THROW(uAnim.getPackageManager());
	EXPECT_EQ(uAnim.getPackageManager().packageCount(), 0);
	EXPECT_EQ(uAnim.frameCount(), 0);
	EXPECT_NO_THROW(uAnim.clearAnimationBlocks());
	EXPECT_NO_THROW(uAnim.setAnimationTitle("NewTitle"));
	EXPECT_STREQ(uAnim.animationTitle(), "NewTitle");

	//Block 1
	{
		UNIV::AnimationBlock block;
		block.setBoneName("Base");
		EXPECT_STREQ(block.boneName(), "Base");

		UNIV::Rotation r5(5, Math::Quaternion(0.0, 0.0, 1.0, 1.0));
		UNIV::Rotation r0(0, Math::Quaternion(1.0, 0.0, 0.0, 0.0));
		UNIV::Rotation r2(2, Math::Quaternion(0.0, 1.0, 0.0, 1.0));
		UNIV::Rotation r2n(2, Math::Quaternion(1.0, 0.0, 0.0, 1.0));
		
		EXPECT_TRUE(block.addRotationEntry(r5));
		EXPECT_TRUE(block.addRotationEntry(r0));
		EXPECT_TRUE(block.addRotationEntry(r2));

		EXPECT_EQ(block.rotationEntryCount(), 3);
		
		EXPECT_EQ(block.rotationSequence().at(0).frame(), 0);
		EXPECT_EQ(block.rotationSequence().at(1).frame(), 2);
		EXPECT_EQ(block.rotationSequence().at(2).frame(), 5);

		EXPECT_EQ(block.rotationSequence().at(0).rotationQuaternion(), Math::Quaternion(1.0, 0.0, 0.0, 0.0));
		EXPECT_EQ(block.rotationSequence().at(1).rotationQuaternion(), Math::Quaternion(0.0, 1.0, 0.0, 1.0));
		EXPECT_EQ(block.rotationSequence().at(2).rotationQuaternion(), Math::Quaternion(0.0, 0.0, 1.0, 1.0));

		EXPECT_TRUE(block.addRotationEntry(r2n));

		EXPECT_EQ(block.rotationEntryCount(), 3);

		EXPECT_EQ(block.rotationSequence().at(0).rotationQuaternion(), Math::Quaternion(1.0, 0.0, 0.0, 0.0));
		EXPECT_EQ(block.rotationSequence().at(1).rotationQuaternion(), Math::Quaternion(1.0, 0.0, 0.0, 1.0));
		EXPECT_EQ(block.rotationSequence().at(2).rotationQuaternion(), Math::Quaternion(0.0, 0.0, 1.0, 1.0));

		EXPECT_TRUE(uAnim.addAnimationBlock(block));
	}

	EXPECT_EQ(uAnim.animationBlockCount(), 1);

	//Block 2
	{
		UNIV::AnimationBlock block;
		block.setBoneName("Table");
		EXPECT_STREQ(block.boneName(), "Table");

		UNIV::Rotation r0(0, Math::Quaternion(0.0, 0.0, 0.0, 1.0));
		UNIV::Rotation r1(15, Math::Quaternion(0.0, 0.0, 0.5, 1.0));
		UNIV::Rotation r2(30, Math::Quaternion(0.0, 0.0, 1.0, 1.0));

		EXPECT_TRUE(block.addRotationEntry(r0));
		EXPECT_TRUE(block.addRotationEntry(r1));
		EXPECT_TRUE(block.addRotationEntry(r2));

		EXPECT_TRUE(uAnim.addAnimationBlock(block));
	}

	EXPECT_EQ(uAnim.animationBlockCount(), 2);
	EXPECT_EQ(uAnim.frameCount(), 31);
}

