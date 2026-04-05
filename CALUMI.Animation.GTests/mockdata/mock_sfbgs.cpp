#include <gtest/gtest.h>
#include <AnimMath>
#include <AnimUniv>
#include <AnimStarfield>

using namespace CALUMI;

#define GTEST(x) TEST(Mock_Starfield, x)

GTEST(Rig_00)
{
	std::vector<char> stdEOH = { '\0','\0','C','A','L','U','M','I','D','V','R','S','J','O','J','O' };
	SFBGS::SkeletonRig rig00;

	auto result00 = rig00.ReadFromFile("assets/00/skeleton.rig");

	//Read Check
	EXPECT_TRUE(result00.has_value());

	//File version
	EXPECT_TRUE(rig00.VersionNumber() == 5);

	//File Size
	EXPECT_TRUE(rig00.FileSize() == 720);

	//Header Size
	EXPECT_TRUE(rig00.HeaderSize() == 0x50);

	//Map Offset
	EXPECT_TRUE(rig00.BoneMapOffset() == 368);

	//Bone Counts
	EXPECT_TRUE(rig00.BoneCount() == 3);
	EXPECT_TRUE(rig00.BoneCountAnimated() == 3);
	EXPECT_TRUE(rig00.BoneEntries().size() == rig00.BoneCount());

	EXPECT_TRUE(rig00.DEBUG_CheckAssumedHeaderEntries() == 0);

#ifdef _DEBUG
	//End Of Header
	auto rig00EOH = rig00.EndOfHeader();
	bool rig00EOHsize = rig00EOH.size() == stdEOH.size();
	EXPECT_TRUE(rig00EOHsize);
	if (rig00EOHsize)
	{
		for (size_t i = 0; i < rig00EOH.size(); i++)
		{
			EXPECT_TRUE(rig00EOH.at(i) == stdEOH.at(i));
		}
	}
#endif

	//Precision
	EXPECT_TRUE(rig00.HighPrecision() == 1.0f / 4000.0f);
	EXPECT_TRUE(rig00.LowPrecision() == 1.0f / 32.000f);

	//Matching 3
	auto match00 = rig00.getMatchingThree();
	EXPECT_TRUE(match00.size() == 3);
	for (uint8_t i = 0; i < match00.size(); i++)
	{
		EXPECT_TRUE(match00.at(i) == 4587439358609565533);
	}

	//Bone Entries
	float unk00Floats[3] = { 0.1f, 0.2f, 60.0f };
	Math::Quaternion q00Locals[3] = { Math::Quaternion(-0.0f,-0.0f,-0.707107f,0.707107f), Math::Quaternion(), Math::Quaternion() };

	auto& rig00Bones = rig00.BoneEntries();
	EXPECT_TRUE(rig00Bones.size() == rig00.BoneCount());

	auto& str00 = rig00.StringArray();
	EXPECT_TRUE(rig00Bones.size() == str00.size());

	uint64_t offsetSum00 = 0;

	if (!rig00Bones.empty())
		offsetSum00 = rig00Bones.at(0).getNameOffset();

	float twistW00[3] = { 0.0f,0.0f,0.67f };

	for (uint8_t i = 0; i < rig00Bones.size() && i < str00.size(); i++)
	{
		//Rotations
		EXPECT_TRUE(rig00Bones.at(i).LocalRotation().AreEqual(q00Locals[i], 0.000001f));
		EXPECT_TRUE(rig00Bones.at(i).GlobalRotation().AreEqual(q00Locals[0], 0.000001f));

		//Unks
		EXPECT_TRUE(rig00Bones.at(i).getUnknownScalar() == unk00Floats[i]);
		EXPECT_TRUE(rig00Bones.at(i).getTerm05() == 4);

		//Bone Info
		EXPECT_TRUE(rig00Bones.at(i).getBoneType() == SFBGS::SkeletonBone::BoneType::Default);
		EXPECT_TRUE(rig00Bones.at(i).getParentBoneIndex() + 1 == i);
		EXPECT_TRUE(rig00Bones.at(i).getTwistDriverMqnIndex() == -1);
		EXPECT_TRUE(rig00Bones.at(i).getTwistDriverIndex() == -1);
		EXPECT_TRUE(rig00Bones.at(i).getTwistDriverWeight() == twistW00[i]);
		EXPECT_TRUE(rig00Bones.at(i).getMirrorBoneIndex() == i);

		//Padding 
#ifdef _DEBUG
		EXPECT_TRUE(rig00Bones.at(i).getPad01() == -1);
		EXPECT_TRUE(rig00Bones.at(i).getPad02() == 0);
#endif
		//Strings
		EXPECT_TRUE(offsetSum00 == rig00Bones.at(i).getNameOffset());
		offsetSum00 += str00.at(i).length(true);
	}

	//Bone Names
	if (str00.size() == 3)
	{
		str00.at(0) == "Fountain_Root";
		str00.at(1) == "PlanetObject";
		str00.at(2) == "RingObject";
	}

	//Bone Map
	bool boneMapSize = rig00.BoneMapArray().size() == 157;
	EXPECT_TRUE(boneMapSize);
	if (boneMapSize)
	{
		for (size_t i = 0; i < rig00.BoneMapArray().size(); i++)
		{
			short mapVal = -1;

			if (i == 0)
				mapVal = 0;
			else if (i == 8)
				mapVal = 1;
			else
				mapVal = -1;

			EXPECT_TRUE(rig00.BoneMapArray().at(i) == mapVal);
		}
	}

	//TODO: Conversion Check
}