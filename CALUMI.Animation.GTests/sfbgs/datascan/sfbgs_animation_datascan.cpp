#include "sfbgs_common.h"


#define GTEST(x) TEST_F(StarfieldAnimationDataScan, x)

using namespace CALUMI;


class StarfieldAnimationDataScan : public testing::Test
{
public:

protected:
	static void SetUpTestSuite()
	{
		if (!ShouldRunScans)
			GTEST_SKIP();

		SFBGS_Data::get().inputExists = std::filesystem::exists(SFBGS_Data::get().inputPath);
		ASSERT_TRUE(SFBGS_Data::get().inputExists);

		if (!SFBGS_Data::get().inputExists)
			GTEST_SKIP() << "Input File: sfbgs_exdatapath.txt Not Found";

		if (std::ifstream dataDirInput(SFBGS_Data::get().inputPath); dataDirInput.is_open())
		{
			std::stringstream buffer;
			buffer << dataDirInput.rdbuf();

			const std::string content = buffer.str();

			dataDirInput.close();

			ASSERT_FALSE(content.empty());

			SFBGS_Data::get().dataPath = content;
		}

		if (!std::filesystem::exists(SFBGS_Data::get().dataPath))
			GTEST_SKIP() << "Data Path Not Found";

		if (!std::filesystem::is_directory(SFBGS_Data::get().dataPath))
			GTEST_SKIP() << "Data Path Is Not Directory";

		std::cout << "--- STARFIELD ANIM SCAN STARTING ---" << std::endl;
	}

	static void TearDownTestSuite()
	{
		if (ShouldRunScans)
			std::cout << "--- STARFIELD ANIM SCAN COMPLETE ---" << std::endl;
	}
};



GTEST(AkilaDoor01Animation)
{
	const std::filesystem::path animPath = SFBGS_Data::get().dataPath.string() + "/meshes/architecture/city/akila/animated/doors/ak_ext_bld_walla_doora_01/animations/open.af";
	ASSERT_TRUE(std::filesystem::exists(animPath)) << "\tFILE DOES NOT EXIST" << std::endl;
	ASSERT_TRUE(std::filesystem::is_regular_file(animPath)) << "\tFILE IS NOT REGULAR FILE" << std::endl;

	SFBGS::Animation anim;

	ASSERT_FALSE(anim.readFromFile(animPath.string().c_str()).hasError()) << "\tERROR ON FILE READ" << std::endl;

	const SFBGS::AnimationBlock animBlock = anim.animationBlocks().at(1);

	const Math::Quaternion rotation = SFBGS::CompressedRotation(animBlock.rotationPrefixEntries().at(animBlock.rotationPrefixCount() - 1), animBlock.rotationEntries().at(animBlock.rotationCount() - 1)).toQuaternion();
	const Math::Quaternion expected(Math::Vector3(0.0, 0.0, 1.0), Math::ToRadians(-89.99301));

	ASSERT_TRUE(rotation.areEqual(expected, 0.000000000000000000000000000000000000000000001f)) << "Rotation: " << rotation.toString().c_str() << " Expected: " << expected.toString().c_str() << std::endl;
}

GTEST(LodgeDoor01Animation)
{
	const std::filesystem::path animPath = SFBGS_Data::get().dataPath.string() + "/meshes/architecture/city/newatlantis/lodge/na_lodgedoor02/animations/open.af";
	ASSERT_TRUE(std::filesystem::exists(animPath)) << "\tFILE DOES NOT EXIST" << std::endl;
	ASSERT_TRUE(std::filesystem::is_regular_file(animPath)) << "\tFILE IS NOT REGULAR FILE" << std::endl;

	SFBGS::Animation anim;

	ASSERT_FALSE(anim.readFromFile(animPath.string().c_str()).hasError()) << "\tERROR ON FILE READ" << std::endl;

	const SFBGS::AnimationBlock animBlockLeft = anim.animationBlocks().at(1);
	const SFBGS::AnimationBlock animBlockRight = anim.animationBlocks().at(4);

	const Math::Quaternion rotationLeft = SFBGS::CompressedRotation(animBlockLeft.rotationPrefixEntries().at(animBlockLeft.rotationPrefixCount() - 1), animBlockLeft.rotationEntries().at(animBlockLeft.rotationCount() - 1)).toQuaternion();
	const Math::Quaternion expectedLeft(Math::Vector3(0.0, 0.0, 1.0), Math::ToRadians(90.0f));
	const Math::Quaternion rotationRight = SFBGS::CompressedRotation(animBlockRight.rotationPrefixEntries().at(animBlockRight.rotationPrefixCount() - 1), animBlockRight.rotationEntries().at(animBlockRight.rotationCount() - 1)).toQuaternion();
	const Math::Quaternion expectedRight(Math::Vector3(0.0, 0.0, 1.0), Math::ToRadians(-89.99301f));

	EXPECT_TRUE(rotationLeft.areEqual(expectedLeft, 0.000001f)) << "\tRotation Left: " << rotationLeft.toString().c_str() << " Expected Left: " << expectedLeft.toString().c_str() << std::endl;
	EXPECT_TRUE(rotationRight.areEqual(expectedRight, 0.000001f)) << "\tRotation Right: " << rotationRight.toString().c_str() << " Expected Right: " << expectedRight.toString().c_str() << std::endl;
}

union ErrorStatus {
	struct ErrorFlag
	{
		bool magicNumber : 1 = false;
		bool wrongFileName : 1 = false;
		bool wrongVersion : 1 = false;
		bool wrongFileSize : 1 = false;
		bool moreThanOnePriority : 1 = false;
		bool amendedPriorityCountOverOne : 1 = false;

	} flags;

	uint32_t raw{};
};

GTEST(AnimScan)
{
	///The purpose of this test is to scan and verify assumptions based on local file data
	uint64_t filesFound = 0;
	uint64_t filesScanned = 0;
	uint64_t filesUnexpected = 0;

	const std::filesystem::path meshesPath = SFBGS_Data::get().dataPath.string() + "/meshes";

	std::cout << "\tScanning " << _YELLOW(meshesPath.string()) << " for .af files" << std::endl;

	for (const auto& entry : std::filesystem::recursive_directory_iterator(meshesPath))
	{
		if (!std::filesystem::is_regular_file(entry.path()) || entry.path().extension() != SFBGS_Data::get().afExt)
			continue;

		filesFound++;

		SFBGS::Animation anim;

		ASSERT_FALSE(anim.readFromFile(entry.path().c_str()).hasError());

		filesScanned++;

		ErrorStatus status = {};

		status.flags.magicNumber = anim.magicNumber() != 0;

		status.flags.wrongVersion = !(ISKNOWNVERSION(anim.versionNumber()));

		status.flags.wrongFileSize = anim.sourceFileSize() != std::filesystem::file_size(entry);

		status.flags.wrongFileName = entry.path().filename().string() == anim.animationFileName().c_str();


		//Blocks
		for (uint64_t i = 0; i < anim.animationBlocks().size(); i++)
		{
			const auto& block = anim.animationBlocks().at(i);

			if (block.priorityCount()>1)
				status.flags.moreThanOnePriority = true;
		}


		//Amended Blocks
		for (uint64_t i = 0; i < anim.amendedAnimationBlocks().size(); i++)
		{
			const auto& block = anim.amendedAnimationBlocks().at(i);

			if (block.priorityCount()>1)
				status.flags.moreThanOnePriority = true;
		}

		//End Of File Process Results
		if (status.raw == 0)
			continue;

		filesUnexpected++;

		ADD_FAILURE() << _YELLOW(entry.path().string()) << std::endl;

		if (status.flags.magicNumber)
			std::cout << "\tMAGIC NUMBER IS NOT ZERO: " << _BRED(anim.magicNumber()) << std::endl;

		if (status.flags.wrongVersion)
			std::cout << "\tUNKNOWN VERSION: " << _BRED(anim.versionNumber()) << std::endl;

		if (status.flags.wrongFileSize)
			std::cout << "\tINCORRECT FILE SIZE: " << _BRED(anim.sourceFileSize()) << " EXPECTED: " << _YELLOW(std::filesystem::file_size(entry)) << std::endl;

		if (status.flags.moreThanOnePriority)
			std::cout << "\tMORE THAN ONE PRIORITY: " << _YELLOW(entry.path().string()) << std::endl;

		if (status.flags.amendedPriorityCountOverOne)
			std::cout << "\tMORE THAN ONE PRIORITY IN AMENDED: " << _YELLOW(entry.path().string()) << std::endl;


	}

	//====================================== End File Loop ===================================================

	std::cout << "\tFILES FOUND: " << _BMAGENTA(filesFound) << std::endl;
	std::cout << "\tFILES SCANNED: " << _GREEN(filesScanned) << std::endl;
	const std::string incAssumption = filesUnexpected == 0 ? _GREEN(filesUnexpected).c_str() : _BRED(filesUnexpected).c_str();
	std::cout << "\tFILES WITH INCORRECT ASSUMPTIONS: " << incAssumption << std::endl;
}


