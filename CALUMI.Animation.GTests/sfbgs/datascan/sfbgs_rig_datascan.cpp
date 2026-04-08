#include "sfbgs_common.h"


#define GTEST(x) TEST_F(StarfieldRigDataScan, x)

using namespace CALUMI;

union ErrorStatus {
	struct ErrorFlag
	{
		bool wrongVersion : 1 = false;
		bool wrongFileSize : 1 = false;
		bool wrongHeaderSize : 1 = false;
		bool nonEmptyPadding01 : 1 = false;
		bool wrongMapOffset : 1 = false;
		bool nonEmptyPadding02 : 1 = false;
		bool wrongMatchingThree : 1 = false;
		bool wrongPrecision : 1 = false;
		bool unknownPrecisionSet : 1 = false;
		bool wrongBoneCount : 1 = false;
		bool wrongBoneCountAnimated : 1 = false;
		bool boneCountLessThanAnimated : 1 = false;
		bool nonEmptyPadding03 : 1 = false;


	} flags;

	uint32_t raw;
};



class StarfieldRigDataScan : public ::testing::Test
{
public:

protected:
	static void SetUpTestSuite()
	{
		SFBGS_Data::get().inputExists = std::filesystem::exists(SFBGS_Data::get().inputPath);
		ASSERT_TRUE(SFBGS_Data::get().inputExists);

		if (!SFBGS_Data::get().inputExists)
			GTEST_SKIP() << "Input File: sfbgs_exdatapath.txt Not Found";

		std::ifstream dataDirInput(SFBGS_Data::get().inputPath);

		if (dataDirInput.is_open())
		{
			std::stringstream buffer;
			buffer << dataDirInput.rdbuf();

			std::string content = buffer.str();

			dataDirInput.close();

			ASSERT_FALSE(content.empty());

			SFBGS_Data::get().dataPath = content;
		}

		if (!std::filesystem::exists(SFBGS_Data::get().dataPath))
			GTEST_SKIP() << "Data Path Not Found";

		if (!std::filesystem::is_directory(SFBGS_Data::get().dataPath))
			GTEST_SKIP() << "Data Path Is Not Directory";

		std::cout << "--- STARFIELD RIG SCAN STARTING ---" << std::endl;
	}

	static void TearDownTestSuite()
	{
		std::cout << "--- STARFIELD RIG SCAN COMPLETE ---" << std::endl;
	}
};

GTEST(RigScan)
{

	///The purpose of this test is to scan and verify assumptions based on local file data
	uint64_t filesFound = 0;
	uint64_t filesScanned = 0;
	uint64_t filesUnexpected = 0;
	std::filesystem::path meshesPath = SFBGS_Data::get().dataPath.string() + "/meshes";

	std::cout << "Scanning " << _YELLOW(meshesPath.string()) << " for .rig files" << std::endl;

	for (const auto& entry : std::filesystem::recursive_directory_iterator(meshesPath))
	{
		if (!std::filesystem::is_regular_file(entry.path()) || entry.path().extension() != SFBGS_Data::get().rigExt) continue;

		filesFound++;

		SFBGS::SkeletonRig rig;

		ASSERT_FALSE(rig.readFromFile(entry.path().c_str()).hasError());

		filesScanned++;

		ErrorStatus status = { 0 };

		status.flags.wrongVersion = !(ISKNOWNVERSION(rig.versionNumber()));

		status.flags.wrongFileSize = rig.fileSize() != std::filesystem::file_size(entry);

		status.flags.wrongHeaderSize = rig.headerSize() != 0x50;

		auto headerAssert = rig.checkAssumedHeaderEntries();

		status.flags.nonEmptyPadding01 = (headerAssert & 0b1) > 0;
		status.flags.nonEmptyPadding02 = (headerAssert & 0b10) > 0;
		status.flags.nonEmptyPadding03 = (headerAssert & 0b100) > 0;

		status.flags.wrongMatchingThree = (headerAssert & 0b1000) > 0;
		status.flags.wrongBoneCountAnimated = (headerAssert & 0b1000000) > 0;
		status.flags.boneCountLessThanAnimated = rig.boneCount() < rig.boneCountAnimated();

		status.flags.unknownPrecisionSet = true;
		SFBGS::PrecisionSet pSet(rig.highPrecision(), rig.lowPrecision());

		if (pSet == SFBGS::PrecisionSet::DefaultPrecision())
			status.flags.unknownPrecisionSet = false;

		if (pSet == SFBGS::PrecisionSet::FirstPersonPrecision())
			status.flags.unknownPrecisionSet = false;

		if (pSet == SFBGS::PrecisionSet::ShipPrecision())
			status.flags.unknownPrecisionSet = false;



		//End Of File Process Results
		if (status.raw == 0) continue;

		filesUnexpected++;

		ADD_FAILURE() << _YELLOW(entry.path().string()) << std::endl;

		if (status.flags.wrongVersion)
			std::cout << "UNKNOWN VERSION: " << _BRED(rig.versionNumber()) << std::endl;

		if (status.flags.wrongFileSize)
			std::cout << "INCORRECT FILE SIZE: " << _BRED(rig.fileSize()) << " EXPECTED: " << _YELLOW(std::filesystem::file_size(entry)) << std::endl;

		if (status.flags.wrongHeaderSize)
			std::cout << "INCORRECT HEADER SIZE: " << _BRED(rig.headerSize()) << " EXPECTED: " << _YELLOW(0x50) << std::endl;

		if (status.flags.nonEmptyPadding01)
			std::cout << "NON EMPTY PADDING 01: " << status.flags.nonEmptyPadding01 << " 02: " << status.flags.nonEmptyPadding02 << " 03: " << status.flags.nonEmptyPadding03 << std::endl;

		if (status.flags.wrongMatchingThree)
			std::cout << "MATCHING THREE ERROR: " << rig.getMatchingThree().at(0) << ", " << rig.getMatchingThree().at(1) << ", " << rig.getMatchingThree().at(2) << std::endl;

		if (status.flags.wrongBoneCountAnimated)
			std::cout << "BONE COUNT [ANIMATED]: SIZE NOT EXPECTED" << std::endl;

		if (status.flags.boneCountLessThanAnimated)
			std::cout << "BONE COUNT LESS THAN BONE COUNT [ANIMATED]: " << rig.boneCount() << " < " << rig.boneCountAnimated() << std::endl;

		if (status.flags.unknownPrecisionSet)
			std::cout << "UNKNOWN PRECISION SET: (high) " << rig.highPrecision() << " and (low) " << rig.lowPrecision() << std::endl;

	}

	//====================================== End File Loop ===================================================

	std::cout << "FILES FOUND: " << _BMAGENTA(filesFound) << std::endl;
	std::cout << "FILES SCANNED: " << _GREEN(filesScanned) << std::endl;
	std::string incAssumpt = filesUnexpected == 0 ? _GREEN(filesUnexpected).c_str() : _BRED(filesUnexpected).c_str();
	std::cout << "FILES WITH INCORRECT ASSUMPTIONS: " << incAssumpt << std::endl;
}