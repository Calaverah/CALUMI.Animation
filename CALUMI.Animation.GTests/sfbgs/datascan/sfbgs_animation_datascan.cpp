#include "sfbgs_common.h"


#define GTEST(x) TEST_F(StarfieldAnimationDataScan, x)

using namespace CALUMI;


	class StarfieldAnimationDataScan : public testing::Test
	{
	public:
	
	protected:
		static void SetUpTestSuite() 
		{
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
			std::cout << "--- STARFIELD ANIM SCAN COMPLETE ---" << std::endl;
		}
	};

	

	GTEST(AkilaDoor01Animation)
	{
		const std::filesystem::path animPath = SFBGS_Data::get().dataPath.string() + "/meshes/architecture/city/akila/animated/doors/ak_ext_bld_walla_doora_01/animations/open.af";
		ASSERT_TRUE(std::filesystem::exists(animPath)) << "FILE DOES NOT EXIST" << std::endl;
		ASSERT_TRUE(std::filesystem::is_regular_file(animPath)) << "FILE IS NOT REGULAR FILE" << std::endl;

		SFBGS::Animation anim;

		ASSERT_FALSE(anim.readFromFile(animPath.string().c_str()).hasError()) << "ERROR ON FILE READ" << std::endl;

		const SFBGS::AnimationBlock animBlock = anim.animationBlocks().at(1);

		Math::Quaternion rotation = SFBGS::GetUniversalRotation(animBlock.rotationPrefixEntries().at(animBlock.rotationPrefixCount() - 1), animBlock.rotationEntries().at(animBlock.rotationCount() - 1));
		Math::Quaternion expected(Math::Vector3(0.0, 0.0, 1.0), Math::ToRadians(-89.99301));

		ASSERT_TRUE(rotation.areEqual(expected, 0.000000000000000000000000000000000000000000001f)) << "Rotation: " << rotation.toString().c_str() << " Expected: " << expected.toString().c_str() << std::endl;
	}

	GTEST(LodgeDoor01Animation)
	{
		const std::filesystem::path animPath = SFBGS_Data::get().dataPath.string() + "/meshes/architecture/city/newatlantis/lodge/na_lodgedoor02/animations/open.af";
		ASSERT_TRUE(std::filesystem::exists(animPath)) << "FILE DOES NOT EXIST" << std::endl;
		ASSERT_TRUE(std::filesystem::is_regular_file(animPath)) << "FILE IS NOT REGULAR FILE" << std::endl;

		SFBGS::Animation anim;

		ASSERT_FALSE(anim.readFromFile(animPath.string().c_str()).hasError()) << "ERROR ON FILE READ" << std::endl;

		const SFBGS::AnimationBlock animBlockLeft = anim.animationBlocks().at(1);
		const SFBGS::AnimationBlock animBlockRight = anim.animationBlocks().at(4);

		Math::Quaternion rotationLeft = SFBGS::GetUniversalRotation(animBlockLeft.rotationPrefixEntries().at(animBlockLeft.rotationPrefixCount() - 1), animBlockLeft.rotationEntries().at(animBlockLeft.rotationCount() - 1));
		Math::Quaternion expectedLeft(Math::Vector3(0.0, 0.0, 1.0), Math::ToRadians(90.0f));
		Math::Quaternion rotationRight = SFBGS::GetUniversalRotation(animBlockRight.rotationPrefixEntries().at(animBlockRight.rotationPrefixCount() - 1), animBlockRight.rotationEntries().at(animBlockRight.rotationCount() - 1));
		Math::Quaternion expectedRight(Math::Vector3(0.0, 0.0, 1.0), Math::ToRadians(-89.99301f));

		EXPECT_TRUE(rotationLeft.areEqual(expectedLeft, 0.000001f)) << "Rotation Left: " << rotationLeft.toString().c_str() << " Expected Left: " << expectedLeft.toString().c_str() << std::endl;
		EXPECT_TRUE(rotationRight.areEqual(expectedRight, 0.000001f)) << "Rotation Right: " << rotationRight.toString().c_str() << " Expected Right: " << expectedRight.toString().c_str() << std::endl;
	}

