// ReSharper disable CppUnusedIncludeDirective
// ReSharper disable CppTooWideScope
// ReSharper disable CppDFATimeOver
// ReSharper disable CppTemplateArgumentsCanBeDeduced
// ReSharper disable CppExpressionWithoutSideEffects
#include <gtest/gtest.h>
#include <iostream>
#include "../../include/AnimUniv"
#include "../../include/AnimMath"
#include "../../include/AnimStarfield"
#include <filesystem>
#include <fstream>

#include "../Common.h"

using namespace CALUMI;


class SampleSFBGSData
{
public:
	static SampleSFBGSData& get() {
		static SampleSFBGSData instance;
		return instance;
	}

	SampleSFBGSData(const SampleSFBGSData&) = delete;
	SampleSFBGSData(SampleSFBGSData&&) = delete;
	SampleSFBGSData& operator=(const SampleSFBGSData&) = delete;
	SampleSFBGSData& operator=(SampleSFBGSData&&) = delete;

	UNIV::SkeletonRig univRig00;
	UNIV::SkeletonRig uJsonRig00;
	SFBGS::SkeletonRig sfbgsRig00;
	std::vector<char> stdEOH;

	// ReSharper disable once CppDFANotInitializedField
	[[maybe_unused]] UNIV::Animation uJsonAnim00;

private:
	SampleSFBGSData() = default;
	~SampleSFBGSData() = default;
};


class SampleSFBGSTestSuite : public testing::Test
{
protected:
	static void SetUpTestSuite()
	{
		SampleSFBGSData::get().stdEOH = { '\0','\0','C','A','L','U','M','I','D','V','R','S','J','O','J','O' };

		const std::filesystem::path rigPath = "assets/00/skeleton.rig";

		if (!std::filesystem::exists(rigPath))
		{
			FAIL() << "\tmock_sfbgs.cpp SampleSFBGSTestSuite::SetUpTestSuite(); --> skeleton.rig not found";
		}

		const auto result00 = SampleSFBGSData::get().sfbgsRig00.readFromFile(rigPath.string().c_str());
		ASSERT_FALSE(result00.hasError()) << "00: Error Found In File Read -> " << result00.toString().c_str();

		SampleSFBGSData::get().univRig00 = SampleSFBGSData::get().sfbgsRig00.convertToUniversalRig();

	}

	static void TearDownTestSuite()
	{

	}
};

#define GTEST(x) TEST_F(SampleSFBGSTestSuite, x)

GTEST(Rig00Input)
{
	//File version
	EXPECT_EQ(SampleSFBGSData::get().sfbgsRig00.versionNumber(), 5);

	//File Size
	EXPECT_EQ(SampleSFBGSData::get().sfbgsRig00.fileSize(), 720);

	//Header Size
	EXPECT_EQ(SampleSFBGSData::get().sfbgsRig00.headerSize(), 0x50);

	//Map Offset
	EXPECT_EQ(SampleSFBGSData::get().sfbgsRig00.boneMapOffset(), 368);

	//Bone Counts
	EXPECT_EQ(SampleSFBGSData::get().sfbgsRig00.boneCount(), 3);
	EXPECT_EQ(SampleSFBGSData::get().sfbgsRig00.boneCountAnimated(), 3);
	EXPECT_EQ(SampleSFBGSData::get().sfbgsRig00.boneEntries().size(), SampleSFBGSData::get().sfbgsRig00.boneCount());

#ifdef DEBUG_BUILD
	EXPECT_TRUE(SampleSFBGSData::get().sfbgsRig00.checkAssumedHeaderEntries() == 0);

	{
		//End Of Header
		auto rig00EOH = SampleSFBGSData::get().sfbgsRig00.endOfHeader();

		EXPECT_EQ(rig00EOH.size(), SampleSFBGSData::get().stdEOH.size());
		if (rig00EOH.size() == SampleSFBGSData::get().stdEOH.size())
		{
			for (size_t i = 0; i < rig00EOH.size(); i++)
			{
				EXPECT_EQ(rig00EOH.at(i), SampleSFBGSData::get().stdEOH.at(i));
			}
		}
	}
#endif


	//Precision
	EXPECT_EQ(SampleSFBGSData::get().sfbgsRig00.highPrecision(), 1.0f / 4000.0f);
	EXPECT_EQ(SampleSFBGSData::get().sfbgsRig00.lowPrecision(), 1.0f / 32.000f);

	//Matching 3
	auto match00 = SampleSFBGSData::get().sfbgsRig00.matchingThree();
	EXPECT_EQ(match00.size(), 3);
	for (uint8_t i = 0; i < match00.size() && i < 3; i++)
	{
		EXPECT_EQ(match00.at(i), 4587439358609565533);
	}


	//Bone Entries
	Math::Quaternion q00Locals[3] = { Math::Quaternion(-0.0f,-0.0f,-0.707107f,0.707107f), Math::Quaternion(), Math::Quaternion() };

	auto& rig00Bones = SampleSFBGSData::get().sfbgsRig00.boneEntries();
	EXPECT_EQ(rig00Bones.size(), SampleSFBGSData::get().sfbgsRig00.boneCount());

	auto& str00 = SampleSFBGSData::get().sfbgsRig00.stringArray();
	EXPECT_EQ(rig00Bones.size(), str00.size());

	uint64_t offsetSum00 = 0;

	if (!rig00Bones.empty())
		offsetSum00 = rig00Bones.at(0).nameOffset();

	float twistW00[3] = { 0.0f,0.0f,0.67f };


	for (uint8_t i = 0; i < rig00Bones.size() && i < str00.size(); i++)
	{
		//Rotations
		EXPECT_TRUE(rig00Bones.at(i).localRotation().areEqual(q00Locals[i], 0.000001f)) << "Tested " << rig00Bones.at(i).localRotation().toString().c_str() << " \nExpected " << q00Locals[i].toString().c_str();
		EXPECT_TRUE(rig00Bones.at(i).globalRotation().areEqual(q00Locals[0], 0.000001f)) << "Tested " << rig00Bones.at(i).localRotation().toString().c_str() << " \nExpected " << q00Locals[i].toString().c_str();

#ifdef DEBUG_BUILD
		float unk00Floats[3] = { 0.1f, 0.2f, 60.0f };
		//Unknown
		EXPECT_EQ(rig00Bones.at(i).unknownScalar(), unk00Floats[i]);
#endif

		//Bone Info
		EXPECT_EQ(rig00Bones.at(i).boneType(), SFBGS::SkeletonBone::BoneType::Default);
		EXPECT_EQ(rig00Bones.at(i).parentBoneIndex() + 1, i);
		EXPECT_EQ(rig00Bones.at(i).twistDriverMqnIndex(), -1);
		EXPECT_EQ(rig00Bones.at(i).twistDriverIndex(), -1);
		EXPECT_EQ(rig00Bones.at(i).twistDriverWeight(), twistW00[i]);
		EXPECT_EQ(rig00Bones.at(i).mirrorBoneIndex(), i);
		EXPECT_EQ(rig00Bones.at(i).levelOfDetail(), 4);

#ifdef DEBUG_BUILD
		//Padding
		EXPECT_EQ(rig00Bones.at(i).pad01(), -1);
		EXPECT_EQ(rig00Bones.at(i).pad02(), 0);
#endif
		//Strings
		EXPECT_EQ(offsetSum00, rig00Bones.at(i).nameOffset());
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
	EXPECT_EQ(SampleSFBGSData::get().sfbgsRig00.boneMapArray().size(), boneMapSize);
	if (SampleSFBGSData::get().sfbgsRig00.boneMapArray().size() == boneMapSize)
	{
		for (size_t i = 0; i < SampleSFBGSData::get().sfbgsRig00.boneMapArray().size(); i++)
		{
			short mapVal = -1;

			if (i == 0)
				mapVal = 0;
			else if (i == 8)
				mapVal = 1;

			EXPECT_EQ(SampleSFBGSData::get().sfbgsRig00.boneMapArray().at(i), mapVal);
		}
	}
}

GTEST(Rig00Conversion)
{
	EXPECT_EQ(SampleSFBGSData::get().univRig00, SampleSFBGSData::get().sfbgsRig00.convertToUniversalRig());

	const UNIV::SkeletonRig rig = SampleSFBGSData::get().sfbgsRig00.convertToUniversalRig();
	[[maybe_unused]] const auto& a = UNIV::RigMirrorPackage::GetPackage(rig);
	[[maybe_unused]] const auto& b = UNIV::RigManifestPackage::GetPackage(rig);
	[[maybe_unused]] const auto& c = SFBGS::SFBGS_RigPackage::GetPackage(rig);

	EXPECT_STRCASEEQ(b.bone(0), "Fountain_Root");
	EXPECT_STRCASEEQ(b.bone(1), "PlanetObject");
	EXPECT_STRCASEEQ(b.bone(2), "RingObject");

	{
		SFBGS::SkeletonRig rSFRig;
		rSFRig.readFromFile("assets/00/reverse_skeleton.rig");
		const UNIV::SkeletonRig revRig = rSFRig.convertToUniversalRig();

		const auto& manifest = UNIV::RigManifestPackage::GetPackage(revRig);
		EXPECT_STRCASEEQ(manifest.bone(0), "Fountain_Root");
		EXPECT_STRCASEEQ(manifest.bone(1), "PlanetObject");
		EXPECT_STRCASEEQ(manifest.bone(2), "RingObject");

		const auto revRing = revRig.root()->childBone("RingObject", false);
		EXPECT_FALSE(revRig.root()->childBone("PlanetObject", false));
		ASSERT_TRUE(revRing);
		EXPECT_TRUE(revRig.root()->childBoneCount() == 1);
		ASSERT_TRUE(revRing->childBone("PlanetObject", false));
		EXPECT_TRUE(revRing->childBone("PlanetObject")->boneCount() == 0);
	}
}

GTEST(Rig00Reconversion)
{
	SFBGS::SkeletonRig sfbgsRig00Copy(SampleSFBGSData::get().univRig00);

	//File version
	EXPECT_EQ(sfbgsRig00Copy.versionNumber(), SampleSFBGSData::get().sfbgsRig00.versionNumber());

	//File Size
	EXPECT_EQ(sfbgsRig00Copy.fileSize(), SampleSFBGSData::get().sfbgsRig00.fileSize());

	//Header Size
	EXPECT_EQ(sfbgsRig00Copy.headerSize(), SampleSFBGSData::get().sfbgsRig00.headerSize());

	//Map Offset
	EXPECT_EQ(sfbgsRig00Copy.boneMapOffset(), SampleSFBGSData::get().sfbgsRig00.boneMapOffset());

	//Bone Counts
	EXPECT_EQ(sfbgsRig00Copy.boneCount(), SampleSFBGSData::get().sfbgsRig00.boneCount());
	EXPECT_EQ(sfbgsRig00Copy.boneEntries().size(), SampleSFBGSData::get().sfbgsRig00.boneEntries().size());
	EXPECT_EQ(sfbgsRig00Copy.boneCountAnimated(), SampleSFBGSData::get().sfbgsRig00.boneCountAnimated());

#ifdef DEBUG_BUILD
	{
		//End Of Header
		auto rig00EOH = sfbgsRig00Copy.endOfHeader();

		EXPECT_EQ(rig00EOH.size(), SampleSFBGSData::get().stdEOH.size());
		if (rig00EOH.size() == SampleSFBGSData::get().stdEOH.size())
		{
			for (size_t i = 0; i < rig00EOH.size(); i++)
			{
				EXPECT_EQ(rig00EOH.at(i), SampleSFBGSData::get().stdEOH.at(i));
			}
		}
	}
#endif

	//Precision
	EXPECT_EQ(SampleSFBGSData::get().sfbgsRig00.highPrecision(), sfbgsRig00Copy.highPrecision());
	EXPECT_EQ(SampleSFBGSData::get().sfbgsRig00.lowPrecision(), sfbgsRig00Copy.lowPrecision());

	auto& rig00COPYBones = sfbgsRig00Copy.boneEntries();
	EXPECT_EQ(rig00COPYBones.size(), sfbgsRig00Copy.boneCount());

	auto& str00COPY = sfbgsRig00Copy.stringArray();
	EXPECT_EQ(rig00COPYBones.size(), str00COPY.size());

	auto& sfbgsRig00Bones = SampleSFBGSData::get().sfbgsRig00.boneEntries();
	uint64_t offsetSum00COPY = 0;

	if (!rig00COPYBones.empty())
		//We use the source offset to begin, as we do not have that until export/serialization
			offsetSum00COPY = sfbgsRig00Bones.at(0).nameOffset();

	for (uint8_t i = 0; i < rig00COPYBones.size() && i < str00COPY.size(); i++)
	{
		//Rotations
		EXPECT_QUATNEAR(rig00COPYBones.at(i).localRotation(), sfbgsRig00Bones.at(i).localRotation(), 0.000001f);
		EXPECT_QUATNEAR(rig00COPYBones.at(i).globalRotation(), sfbgsRig00Bones.at(i).globalRotation(), 0.000001f);
		//Unknowns
		//EXPECT_EQ(rig00COPYBones.at(i).getUnknownScalar(), unk00Floats[i]);
		//EXPECT_EQ(rig00COPYBones.at(i).getTerm05(), 4);

		//Bone Info
		EXPECT_EQ(rig00COPYBones.at(i).boneType(), sfbgsRig00Bones.at(i).boneType());
		EXPECT_EQ(rig00COPYBones.at(i).parentBoneIndex(), sfbgsRig00Bones.at(i).parentBoneIndex());
		EXPECT_EQ(rig00COPYBones.at(i).twistDriverMqnIndex(), sfbgsRig00Bones.at(i).twistDriverMqnIndex());
		EXPECT_EQ(rig00COPYBones.at(i).twistDriverIndex(), -1); //These bones were set to default and should be different from the source on output as the values are corrected
		EXPECT_EQ(rig00COPYBones.at(i).twistDriverWeight(), 0.0f); //These bones were set to default and should be different from the source on output as the values are corrected
		EXPECT_EQ(rig00COPYBones.at(i).mirrorBoneIndex(), sfbgsRig00Bones.at(i).mirrorBoneIndex());

		//Padding
#ifdef DEBUG_BUILD
		EXPECT_EQ(rig00COPYBones.at(i).pad01(), sfbgsRig00Bones.at(i).pad01());
		EXPECT_EQ(rig00COPYBones.at(i).pad02(), sfbgsRig00Bones.at(i).pad02());
#endif
		//Strings
		//We use the source offset as the copy version isn't produced until export/serialization
		EXPECT_EQ(offsetSum00COPY, sfbgsRig00Bones.at(i).nameOffset());
		offsetSum00COPY += str00COPY.stringLength(i, true);
	}

	auto& sfbgsRig00Strings = SampleSFBGSData::get().sfbgsRig00.stringArray();

	//Bone Names
	if (str00COPY.size() == 3)
	{
		EXPECT_EQ(std::string(str00COPY.c_str(0)), sfbgsRig00Strings.c_str(0));
		EXPECT_EQ(std::string(str00COPY.c_str(1)), sfbgsRig00Strings.c_str(1));
		EXPECT_EQ(std::string(str00COPY.c_str(2)), sfbgsRig00Strings.c_str(2));
	}
	else
	{
		ADD_FAILURE() << "Copied Bone Name Array Size Does Not Match";
	}

	//Bone Map
	EXPECT_EQ(SampleSFBGSData::get().sfbgsRig00.boneMapArray().size(), sfbgsRig00Copy.boneMapArray().size());
	if (SampleSFBGSData::get().sfbgsRig00.boneMapArray().size() == sfbgsRig00Copy.boneMapArray().size())
	{
		for (size_t i = 0; i < sfbgsRig00Copy.boneMapArray().size(); i++)
		{
			EXPECT_EQ(sfbgsRig00Copy.boneMapArray().at(i), SampleSFBGSData::get().sfbgsRig00.boneMapArray().at(i));
		}
	}
}

GTEST(Rig00Json)
{
	const std::filesystem::path json00Path = "assets/00/skeleton.json";
	if (!std::filesystem::exists(json00Path))
		GTEST_SKIP() << "Json file for Rig00 not found\n";

	std::string uRig00StringInput;
	std::ifstream iStream(json00Path, std::ios::binary | std::ios::ate);

	if (!iStream.is_open())
		GTEST_SKIP() << "Json file for Rig00 not able to be read\n";

	const std::streamsize size = iStream.tellg();
	iStream.seekg(0, std::ios::beg);

	uRig00StringInput.resize(size, '\0');

	if (!iStream.read(uRig00StringInput.data(), size))
	{
		GTEST_SKIP() << "Json file for Rig00 not able to be read into buffer\n";
	}

	ASSERT_NO_THROW(
		SampleSFBGSData::get().uJsonRig00 = UNIV::SkeletonRig(Utilities::JsonObject::Deserialize(uRig00StringInput.c_str())
		));

	EXPECT_EQ(SampleSFBGSData::get().uJsonRig00, SampleSFBGSData::get().univRig00);
}

GTEST(ScratchAnimation)
{
	UNIV::Animation uAnim;
	uAnim.setAnimationTitle("TestAnim");

	//Sanity Checks
	EXPECT_STREQ(uAnim.animationTitle(), "TestAnim");
	EXPECT_EQ(uAnim.animationBlockCount(), 0);
	EXPECT_EQ(uAnim.animationBlockCount(), uAnim.animationBlocks().size());
	EXPECT_EQ(uAnim.packageManager().packageCount(), 0);
	EXPECT_EQ(uAnim.frameCount(), 0);
	EXPECT_NO_THROW(uAnim.clearAnimationBlocks());
	EXPECT_NO_THROW(uAnim.setAnimationTitle("NewTitle"));
	EXPECT_STREQ(uAnim.animationTitle(), "NewTitle");

	//Block 1
	{
		UNIV::AnimationBlock block;
		block.setBoneName("Base");
		EXPECT_STREQ(block.boneName(), "Base");

		UNIV::RotationFrame r5(5, Math::Quaternion(0.0, 0.0, 1.0, 1.0));
		UNIV::RotationFrame r0(0, Math::Quaternion(1.0, 0.0, 0.0, 0.0));
		UNIV::RotationFrame r2(2, Math::Quaternion(0.0, 1.0, 0.0, 1.0));
		UNIV::RotationFrame r2n(2, Math::Quaternion(1.0, 0.0, 0.0, 1.0));
		
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

		UNIV::RotationFrame r0(0, Math::Quaternion(0.0, 0.0, 0.0, 1.0));
		UNIV::RotationFrame r1(15, Math::Quaternion(0.0, 0.0, 0.5, 1.0));
		UNIV::RotationFrame r2(30, Math::Quaternion(0.0, 0.0, 1.0, 1.0));

		EXPECT_TRUE(block.addRotationEntry(r0));
		EXPECT_TRUE(block.addRotationEntry(r1));
		EXPECT_TRUE(block.addRotationEntry(r2));

		EXPECT_TRUE(uAnim.addAnimationBlock(block));

		auto& amendPackage = SFBGS::SFBGS_AnimationPackage::GetPackage(uAnim);
		auto aBlock = UNIV::AnimationBlock();

		aBlock.setBoneName("Nif");

		aBlock.addScalarEntry({0,0.9});

		aBlock.addRotationEntry({9,{0.0,1.0,1.0,0.0}});
		aBlock.addRotationEntry({0,{0.0,0.5,1.0,0.0}});

		aBlock.addTranslationEntry({0,{0.0,0.0,0.0}});
		aBlock.addTranslationEntry({1,{0.0,0.0,100.0}});

		amendPackage.addAmendedBlock(aBlock);
	}

	EXPECT_EQ(uAnim.animationBlockCount(), 2);
	EXPECT_EQ(uAnim.frameCount(), 31);

	{
		const std::filesystem::path json00Path = "assets/00/animation.json";
		if (!std::filesystem::exists(json00Path))
			GTEST_SKIP() << "Json file for Anim00 not found\n";

		std::string uAnim00StringInput;
		std::ifstream iStream(json00Path, std::ios::binary | std::ios::ate);

		if (!iStream.is_open())
			GTEST_SKIP() << "Json file for Anim00 not able to be read\n";

		const std::streamsize size = iStream.tellg();
		iStream.seekg(0, std::ios::beg);

		uAnim00StringInput.resize(size, '\0');

		if (!iStream.read(uAnim00StringInput.data(), size))
		{
			GTEST_SKIP() << "Json file for Anim00 not able to be read into buffer\n";
		}

		UNIV::Animation jAnim;

		jAnim = UNIV::Animation(Utilities::JsonObject::Deserialize(uAnim00StringInput.c_str()));

		EXPECT_EQ(uAnim, jAnim);
	}

}

GTEST(RigInputExternC)
{
	UNIV::SkeletonRig* rig = LoadSFBGSSkeletonRigFromFileC(L"assets/00/skeleton.rig");

	ASSERT_TRUE(rig);

	EXPECT_EQ(rig->boneCount(), 3);

	DeleteSkeletonRigC(&rig);
}